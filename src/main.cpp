#include <SDL.h>
#include <iostream>
#include <memory>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <chrono>
#include <thread>
#include "scene.hpp"
#include "shape.hpp"
#include "pitch_mapper.hpp"
#include "audio_buffer.hpp"
#include "synesthesia_exceptions.hpp"

namespace {
    const double MIN_FREQ = 100.0;
    const double MAX_FREQ = 1000.0;
    const double SAMPLE_RATE = 44100.0;
    const double NOTE_DURATION = 0.3;

    double frequencyForKey(char key) {
        int index = key - '1'; // 0..8 for keys 1..9
        double step = (MAX_FREQ - MIN_FREQ) / 8.0;
        return MIN_FREQ + step * index;
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
    if (device == 0) {
        std::cerr << "SDL_OpenAudioDevice Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }
    SDL_PauseAudioDevice(device, 0);

    Scene scene;
    RawTerminal rawTerminal;
    bool running = true;

    std::cout << "Press keys 1-9 to play a note. Press q to quit.\n";

    auto lastTime = std::chrono::steady_clock::now();

    while (running) {
        char key = 0;
        if (read(STDIN_FILENO, &key, 1) > 0) {
            if (key == 'q') {
                running = false;
            } else if (key >= '1' && key <= '9') {
                try {
                    double freq = frequencyForKey(key);
                    Color color = mapFrequency<double>(freq, MIN_FREQ, MAX_FREQ);

                    std::size_t sampleCount = static_cast<std::size_t>(SAMPLE_RATE * NOTE_DURATION);
                    AudioBuffer buffer(sampleCount);
                    fillSineWave(buffer, freq, SAMPLE_RATE);
                    SDL_QueueAudio(device, buffer.data(), buffer.size() * sizeof(float));

                    scene.spawn(std::make_unique<Ripple>(color, 1.0f));
                } catch (const InvalidFrequencyException& e) {
                    std::cerr << e.what() << std::endl;
                } catch (const AudioLoadException& e) {
                    std::cerr << e.what() << std::endl;
                }
            }
        }

        auto currentTime = std::chrono::steady_clock::now();
        float dt = std::chrono::duration<float>(currentTime - lastTime).count();
        lastTime = currentTime;

        scene.update(dt);
        scene.draw();

        std::this_thread::sleep_for(std::chrono::milliseconds(80));
    }

    SDL_CloseAudioDevice(device);
    SDL_Quit();

    return 0;
}