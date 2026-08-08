#include <SDL.h>
#include <iostream>
#include <memory>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <chrono>
#include <thread>
#include <vector>
#include <random>
#include <set>
#include "scene.hpp"
#include "shape.hpp"
#include "pitch_mapper.hpp"
#include "audio_buffer.hpp"
#include "synesthesia_exceptions.hpp"

namespace {
    const double MIN_FREQ = 100.0;
    const double MAX_FREQ = 1000.0;
    const double SAMPLE_RATE = 44100.0;
    const double NOTE_DURATION = 0.5;
    const int TERMINAL_WIDTH = 60;

    std::mt19937 rng(std::random_device{}());

    double frequencyForKey(char key) {
        int index = key - '1'; // 0..8 for keys 1..9
        double step = (MAX_FREQ - MIN_FREQ) / 8.0;
        return MIN_FREQ + step * index;
    }

    int randomIndent() {
        std::uniform_int_distribution<int> dist(0, TERMINAL_WIDTH / 2);
        return dist(rng);
    }

    float randomSize(float minSize, float maxSize) {
        std::uniform_real_distribution<float> dist(minSize, maxSize);
        return dist(rng);
    }

    // Spawns a small burst of different shape types for one note, each
    // with a randomized size and horizontal offset, so a single keypress
    // feels like an event rather than a single static line of text.
    void spawnBurst(Scene& scene, const Color& color) {
        scene.spawn(std::make_unique<Ripple>(color, randomIndent(), randomSize(1.0f, 3.0f)));
        scene.spawn(std::make_unique<Circle>(color, randomIndent(), randomSize(2.0f, 6.0f)));
        scene.spawn(std::make_unique<Square>(color, randomIndent(), randomSize(3.0f, 8.0f)));
    }

    // Puts the terminal into raw mode so single keypresses are read
    // immediately without waiting for Enter, and restores it on exit.
    struct RawTerminal {
        termios original;
        RawTerminal() {
            termios raw;
            tcgetattr(STDIN_FILENO, &original);
            raw = original;
            raw.c_lflag &= ~(ICANON | ECHO);
            raw.c_cc[VMIN] = 0;
            raw.c_cc[VTIME] = 0;
            tcsetattr(STDIN_FILENO, TCSANOW, &raw);
            int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
            fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
        }
        ~RawTerminal() {
            tcsetattr(STDIN_FILENO, TCSANOW, &original);
        }
    };
}

int main(int argc, char** argv) {
    if (SDL_Init(SDL_INIT_AUDIO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_AudioSpec want;
    SDL_zero(want);
    want.freq = static_cast<int>(SAMPLE_RATE);
    want.format = AUDIO_F32SYS;
    want.channels = 1;
    want.samples = 1024;

    SDL_AudioDeviceID device = SDL_OpenAudioDevice(nullptr, 0, &want, nullptr, 0);
    bool audioAvailable = (device != 0);
    if (!audioAvailable) {
        std::cerr << "Audio device unavailable, continuing without sound: "
                  << SDL_GetError() << std::endl;
    } else {
        SDL_PauseAudioDevice(device, 0);
    }

    Scene scene;
    RawTerminal rawTerminal;
    bool running = true;

    std::cout << "Press keys 1-9 to play notes (hold multiple at once for a chord). "
                 "Press q to quit.\n";

    auto lastTime = std::chrono::steady_clock::now();

    // Tracks which note keys are currently held down, since a terminal
    // in raw mode reports keydown events repeatedly while a key is held
    // and gives no keyup event; a short window is used to detect "at once".
    std::set<char> pendingChordKeys;
    auto lastKeyTime = std::chrono::steady_clock::now();
    const double CHORD_WINDOW_SECONDS = 0.05;

    while (running) {
        char key = 0;
        bool gotKey = (read(STDIN_FILENO, &key, 1) > 0);

        if (gotKey) {
            if (key == 'q') {
                running = false;
            } else if (key >= '1' && key <= '9') {
                pendingChordKeys.insert(key);
                lastKeyTime = std::chrono::steady_clock::now();
            }
        }

        double sinceLastKey =
            std::chrono::duration<double>(std::chrono::steady_clock::now() - lastKeyTime).count();

        // Once no new key has arrived for a short window, treat everything
        // collected as one chord: mix their tones together and blend their
        // mapped colors with Color::operator+ before spawning shapes.
        if (!pendingChordKeys.empty() && sinceLastKey > CHORD_WINDOW_SECONDS) {
            try {
                std::size_t sampleCount = static_cast<std::size_t>(SAMPLE_RATE * NOTE_DURATION);
                AudioBuffer mixed(sampleCount);
                bool first = true;
                Color blendedColor(0, 0, 0);

                for (char k : pendingChordKeys) {
                    double freq = frequencyForKey(k);
                    Color noteColor = mapFrequency<double>(freq, MIN_FREQ, MAX_FREQ);
                    blendedColor = first ? noteColor : (blendedColor + noteColor);
                    first = false;

                    AudioBuffer tone(sampleCount);
                    fillHarmonicTone(tone, freq, SAMPLE_RATE);
                    mixed = mixBuffers(mixed, tone);
                }

                if (audioAvailable) {
                    SDL_QueueAudio(device, mixed.data(), mixed.size() * sizeof(float));
                }

                spawnBurst(scene, blendedColor);
            } catch (const InvalidFrequencyException& e) {
                std::cerr << e.what() << std::endl;
            } catch (const AudioLoadException& e) {
                std::cerr << e.what() << std::endl;
            }

            pendingChordKeys.clear();
        }

        auto currentTime = std::chrono::steady_clock::now();
        float dt = std::chrono::duration<float>(currentTime - lastTime).count();
        lastTime = currentTime;

        scene.update(dt);
        scene.draw();

        std::this_thread::sleep_for(std::chrono::milliseconds(80));
    }

    if (audioAvailable) {
        SDL_CloseAudioDevice(device);
    }
    SDL_Quit();

    return 0;
}