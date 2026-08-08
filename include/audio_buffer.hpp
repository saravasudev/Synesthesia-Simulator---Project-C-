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

// Fills an existing buffer with a pure sine wave at the given frequency.
// Kept separate from AudioBuffer itself so the class stays focused on
// raw memory ownership, not signal generation.
void fillSineWave(AudioBuffer& buffer, double frequencyHz, double sampleRate);

// Fills an existing buffer with a tone built from a fundamental frequency
// plus a few quieter harmonic overtones (2x, 3x, 4x the fundamental),
// the way a real plucked or bowed instrument sounds richer than a lab
// tone generator. Also applies a short fade-in/fade-out envelope so
// notes don't click at the start or end.
void fillHarmonicTone(AudioBuffer& buffer, double frequencyHz, double sampleRate);

// Mixes two buffers of equal size sample-by-sample, clamping to [-1, 1]
// so multiple simultaneous notes (a chord) don't distort into noise.
// Throws AudioLoadException if the buffers differ in size.
AudioBuffer mixBuffers(const AudioBuffer& a, const AudioBuffer& b);