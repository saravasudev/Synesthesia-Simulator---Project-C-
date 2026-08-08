#pragma once

#include <stdexcept>
#include <string>

// Thrown when an audio buffer cannot be created or loaded.
class AudioLoadException : public std::runtime_error {
public:
    explicit AudioLoadException(const std::string& msg)
        : std::runtime_error("AudioLoadException: " + msg) {}
};

// Thrown when a frequency value passed to the pitch mapper is out of range.
class InvalidFrequencyException : public std::invalid_argument {
public:
    explicit InvalidFrequencyException(const std::string& msg)
        : std::invalid_argument("InvalidFrequencyException: " + msg) {}
};