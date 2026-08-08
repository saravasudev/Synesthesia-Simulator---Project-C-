#pragma once

#include "color.hpp"
#include "synesthesia_exceptions.hpp"

// Maps a frequency in [minFreq, maxFreq] to a hue that sweeps red -> violet,
// the way a synesthete might associate low pitches with red and high
// pitches with violet/blue.
// Templated so it works with float, double, or int sample rates alike.
template <typename T>
Color mapFrequency(T freq, T minFreq, T maxFreq) {
    if (freq < minFreq || freq > maxFreq) {
        throw InvalidFrequencyException("frequency out of expected range");
    }

    double t = static_cast<double>(freq - minFreq) / static_cast<double>(maxFreq - minFreq);

    // Simple linear interpolation across a red -> green -> blue gradient.
    uint8_t r, g, b;
    if (t < 0.5) {
        double localT = t / 0.5;
        r = static_cast<uint8_t>(255 * (1.0 - localT));
        g = static_cast<uint8_t>(255 * localT);
        b = 0;
    } else {
        double localT = (t - 0.5) / 0.5;
        r = 0;
        g = static_cast<uint8_t>(255 * (1.0 - localT));
        b = static_cast<uint8_t>(255 * localT);
    }
    return Color(r, g, b);
}