#include "shape.hpp"
#include <algorithm>
#include <iostream>
#include <string>

namespace {
    const char* RESET = "\033[0m";
}

Shape::Shape(const Color& color) : color_(color) {}

Shape::~Shape() {}

// ---------------- Circle ----------------

Circle::Circle(const Color& color, float radius)
    : Shape(color), radius_(radius), life_(1.0f) {}

void Circle::update(float dt) {
    life_ -= dt;
}

void Circle::draw() const {
    int width = static_cast<int>(radius_) * 2;
    std::cout << color_.toAnsi() << std::string(width, 'o') << RESET << "\n";
}

bool Circle::isExpired() const {
    return life_ <= 0.0f;
}

// ---------------- Square ----------------

Square::Square(const Color& color, float side)
    : Shape(color), side_(side), life_(1.0f) {}

void Square::update(float dt) {
    life_ -= dt;
}

void Square::draw() const {
    int width = static_cast<int>(side_);
    std::cout << color_.toAnsi() << std::string(width, '#') << RESET << "\n";
}

bool Square::isExpired() const {
    return life_ <= 0.0f;
}

// ---------------- Ripple ----------------

Ripple::Ripple(const Color& color, float startRadius)
    : Shape(color), radius_(startRadius), alpha_(255.0f) {}

void Ripple::update(float dt) {
    radius_ += 20.0f * dt;
    alpha_ -= 150.0f * dt;
    alpha_ = std::max(alpha_, 0.0f);
}

void Ripple::draw() const {
    int width = static_cast<int>(radius_);
    std::cout << color_.toAnsi() << std::string(width, '.') << RESET << "\n";
}

bool Ripple::isExpired() const {
    return alpha_ <= 0.0f;
}