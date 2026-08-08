#pragma once

#include <cctype>
#include <cmath>
#include "color.hpp"

// Maps each letter of the alphabet to a fixed, consistent color, the way
// grapheme-color synesthetes report always seeing a given letter as the
// same color regardless of context. Colors are spread evenly around a
// hue wheel so adjacent letters look visually distinct.
inline Color colorForChar(char c) {
    char lower = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    if (lower < 'a' || lower > 'z') {
        // Non-letters (digits, punctuation, spaces) get a neutral gray
        // rather than a jarring default, since they carry no letter identity.
        return Color(160, 160, 160);
    }

    int index = lower - 'a'; // 0..25
    double hue = (360.0 * index) / 26.0;

    // Simple HSV-to-RGB at fixed saturation/value, hue in degrees.
    double c1 = 0.8; // chroma
    double x = c1 * (1.0 - std::fabs(std::fmod(hue / 60.0, 2.0) - 1.0));
    double m = 0.2;

    double r, g, b;
    if (hue < 60) { r = c1; g = x; b = 0; }
    else if (hue < 120) { r = x; g = c1; b = 0; }
    else if (hue < 180) { r = 0; g = c1; b = x; }
    else if (hue < 240) { r = 0; g = x; b = c1; }
    else if (hue < 300) { r = x; g = 0; b = c1; }
    else { r = c1; g = 0; b = x; }

    return Color(
        static_cast<uint8_t>((r + m) * 255),
        static_cast<uint8_t>((g + m) * 255),
        static_cast<uint8_t>((b + m) * 255)
    );
}