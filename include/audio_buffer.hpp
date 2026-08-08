#pragma once

#include <cstddef>

// Owns a raw dynamically-allocated array of samples.
// Movable but not copyable, to make ownership of the underlying
// buffer explicit and cheap to transfer.
class AudioBuffer {
public:
    // Throws AudioLoadException if sampleCount is 0.
    explicit AudioBuffer(std::size_t sampleCount);
    ~AudioBuffer();

    AudioBuffer(const AudioBuffer&) = delete;
    AudioBuffer& operator=(const AudioBuffer&) = delete;

    AudioBuffer(AudioBuffer&& other) noexcept;
    AudioBuffer& operator=(AudioBuffer&& other) noexcept;

    float* data() { return samples_; }
    const float* data() const { return samples_; }
    std::size_t size() const { return count_; }

    float& operator[](std::size_t index) { return samples_[index]; }
    float operator[](std::size_t index) const { return samples_[index]; }

private:
    float* samples_;
    std::size_t count_;
};

// Fills an existing buffer with a sine wave at the given frequency.
// Kept separate from AudioBuffer itself so the class stays focused on
// raw memory ownership, not signal generation.
void fillSineWave(AudioBuffer& buffer, double frequencyHz, double sampleRate);