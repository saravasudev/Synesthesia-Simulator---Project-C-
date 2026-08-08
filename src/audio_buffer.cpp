#include "audio_buffer.hpp"
#include "synesthesia_exceptions.hpp"
#include <algorithm>
#include <cmath>

namespace {
    const double PI = 3.14159265358979323846;
}

AudioBuffer::AudioBuffer(std::size_t sampleCount) : samples_(nullptr), count_(sampleCount) {
    if (sampleCount == 0) {
        throw AudioLoadException("sample count must be greater than zero");
    }
    samples_ = new float[sampleCount];
    for (std::size_t i = 0; i < sampleCount; i++) {
        samples_[i] = 0.0f;
    }
}

AudioBuffer::~AudioBuffer() {
    delete[] samples_;
}

AudioBuffer::AudioBuffer(AudioBuffer&& other) noexcept
    : samples_(other.samples_), count_(other.count_) {
    other.samples_ = nullptr;
    other.count_ = 0;
}

AudioBuffer& AudioBuffer::operator=(AudioBuffer&& other) noexcept {
    if (this != &other) {
        delete[] samples_;
        samples_ = other.samples_;
        count_ = other.count_;
        other.samples_ = nullptr;
        other.count_ = 0;
    }
    return *this;
}

void fillSineWave(AudioBuffer& buffer, double frequencyHz, double sampleRate) {
    for (std::size_t i = 0; i < buffer.size(); i++) {
        double t = static_cast<double>(i) / sampleRate;
        buffer[i] = static_cast<float>(0.3 * std::sin(2.0 * PI * frequencyHz * t));
    }
}

void fillHarmonicTone(AudioBuffer& buffer, double frequencyHz, double sampleRate) {
    // Overtone amplitudes fall off the way most real instruments' do:
    // the fundamental is loudest, each higher harmonic quieter than the last.
    const double harmonicAmplitudes[] = {1.0, 0.5, 0.3, 0.15};
    const int harmonicCount = 4;

    std::size_t n = buffer.size();
    // Fade-in/out length: about 5% of the note on each end, so the
    // waveform doesn't start or stop with an audible click.
    std::size_t fadeSamples = std::max<std::size_t>(1, n / 20);

    for (std::size_t i = 0; i < n; i++) {
        double t = static_cast<double>(i) / sampleRate;
        double sample = 0.0;
        for (int h = 0; h < harmonicCount; h++) {
            double harmonicFreq = frequencyHz * (h + 1);
            sample += harmonicAmplitudes[h] * std::sin(2.0 * PI * harmonicFreq * t);
        }
        // Normalize roughly so the sum of harmonic amplitudes doesn't clip.
        sample *= 0.3 / 1.95;

        double envelope = 1.0;
        if (i < fadeSamples) {
            envelope = static_cast<double>(i) / fadeSamples;
        } else if (i >= n - fadeSamples) {
            envelope = static_cast<double>(n - i) / fadeSamples;
        }

        buffer[i] = static_cast<float>(sample * envelope);
    }
}

AudioBuffer mixBuffers(const AudioBuffer& a, const AudioBuffer& b) {
    if (a.size() != b.size()) {
        throw AudioLoadException("cannot mix buffers of different sizes");
    }
    AudioBuffer result(a.size());
    for (std::size_t i = 0; i < a.size(); i++) {
        float sum = a[i] + b[i];
        // Clamp so overlapping notes (a chord) don't distort past full scale.
        sum = std::max(-1.0f, std::min(1.0f, sum));
        result[i] = sum;
    }
    return result;
}