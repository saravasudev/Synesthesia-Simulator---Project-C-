#include "shape.hpp"
#include <algorithm>
#include <iostream>
#include <string>

namespace {
    const char* RESET = "\033[0m";
}

Shape::Shape(const Color& color, int indent) : color_(color), indent_(indent) {}

Shape::~Shape() {}

void Shape::drawLine(char glyph, int width, float alpha) const {
    if (width < 0) {
        width = 0;
    }
    (void)alpha; // reserved for future true-alpha terminals
    std::cout << std::string(indent_, ' ') << color_.toAnsi() << std::string(width, glyph)
               << RESET << "\n";
}

// ---------------- Circle ----------------

Circle::Circle(const Color& color, int indent, float radius)
    : Shape(color, indent), radius_(radius), life_(1.0f) {}

void Circle::update(float dt) {
    life_ -= dt;
}

void Circle::draw() const {
    int width = static_cast<int>(radius_) * 2;
    drawLine('o', width);
}

bool Circle::isExpired() const {
    return life_ <= 0.0f;
}

// ---------------- Square ----------------

Square::Square(const Color& color, int indent, float side)
    : Shape(color, indent), side_(side), life_(1.0f) {}

void Square::update(float dt) {
    life_ -= dt;
}

void Square::draw() const {
    int width = static_cast<int>(side_);
    drawLine('#', width);
}

bool Square::isExpired() const {
    return life_ <= 0.0f;
}

// ---------------- Ripple ----------------

Ripple::Ripple(const Color& color, int indent, float startRadius)
    : Shape(color, indent), radius_(startRadius), alpha_(255.0f) {}

void Ripple::update(float dt) {
    // Ripples grow and fade slowly, so they linger and form a wash of
    // color across the terminal rather than disappearing instantly.
    radius_ += 8.0f * dt;
    alpha_ -= 35.0f * dt;
    alpha_ = std::max(alpha_, 0.0f);
}

void Ripple::draw() const {
    int width = static_cast<int>(radius_);
    drawLine('.', width, alpha_);
}

bool Ripple::isExpired() const {
    return alpha_ <= 0.0f;
}