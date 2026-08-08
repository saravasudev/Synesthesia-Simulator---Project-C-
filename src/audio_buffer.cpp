#include "audio_buffer.hpp"
#include "synesthesia_exceptions.hpp"
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