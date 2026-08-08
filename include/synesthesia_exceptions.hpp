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
    // TODO: Implement this constructor.
    // It should call the base class (std::invalid_argument) constructor,
    // passing in a string that starts with "InvalidFrequencyException: "
    // followed by msg. Look at AudioLoadException above for the pattern.
    explicit InvalidFrequencyException(const std::string& msg);
};