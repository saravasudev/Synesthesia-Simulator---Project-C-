#include "color.hpp"

Color::Color() : r_(0), g_(0), b_(0), a_(255) {}

Color::Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
    : r_(r), g_(g), b_(b), a_(a) {}

Color::~Color() {}

uint8_t Color::clampAdd(uint8_t a, uint8_t b) {
    int sum = static_cast<int>(a) + static_cast<int>(b);
    return sum > 255 ? 255 : static_cast<uint8_t>(sum);
}

uint8_t Color::clampSub(uint8_t a, uint8_t b) {
    int diff = static_cast<int>(a) - static_cast<int>(b);
    return diff < 0 ? 0 : static_cast<uint8_t>(diff);
}

Color Color::operator+(const Color& other) const {
    return Color(clampAdd(r_, other.r_), clampAdd(g_, other.g_),
                 clampAdd(b_, other.b_), clampAdd(a_, other.a_));
}

Color Color::operator-(const Color& other) const {
    return Color(clampSub(r_, other.r_), clampSub(g_, other.g_),
                 clampSub(b_, other.b_), clampSub(a_, other.a_));
}

Color& Color::operator+=(const Color& other) {
    r_ = clampAdd(r_, other.r_);
    g_ = clampAdd(g_, other.g_);
    b_ = clampAdd(b_, other.b_);
    a_ = clampAdd(a_, other.a_);
    return *this;
}

bool Color::operator==(const Color& other) const {
    return r_ == other.r_ && g_ == other.g_ && b_ == other.b_ && a_ == other.a_;
}

bool Color::operator!=(const Color& other) const {
    return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, const Color& c) {
    os << "Color(r=" << static_cast<int>(c.r_) << ", g=" << static_cast<int>(c.g_)
       << ", b=" << static_cast<int>(c.b_) << ", a=" << static_cast<int>(c.a_) << ")";
    return os;
}

std::string Color::toAnsi() const {
    int r6 = (r_ * 5) / 255;
    int g6 = (g_ * 5) / 255;
    int b6 = (b_ * 5) / 255;
    int index = 16 + (36 * r6) + (6 * g6) + b6;
    return "\033[38;5;" + std::to_string(index) + "m";
}