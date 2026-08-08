#pragma once

#include <cstdint>
#include <ostream>
#include <string>

// Simple RGBA color used to map frequencies to visuals.
class Color {
public:
    Color();
    Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);
    ~Color();

    uint8_t getR() const { return r_; }
    uint8_t getG() const { return g_; }
    uint8_t getB() const { return b_; }
    uint8_t getA() const { return a_; }

    // Additive color mix (clamped at 255 per channel).
    Color operator+(const Color& other) const;

    // Subtractive mix (clamped at 0 per channel).
    Color operator-(const Color& other) const;

    Color& operator+=(const Color& other);

    bool operator==(const Color& other) const;
    bool operator!=(const Color& other) const;

    friend std::ostream& operator<<(std::ostream& os, const Color& c);

    // ANSI escape code to set terminal foreground to this color.
    std::string toAnsi() const;

private:
    uint8_t r_;
    uint8_t g_;
    uint8_t b_;
    uint8_t a_;

    static uint8_t clampAdd(uint8_t a, uint8_t b);
    static uint8_t clampSub(uint8_t a, uint8_t b);
};