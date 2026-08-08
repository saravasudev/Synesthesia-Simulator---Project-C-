#pragma once

#include "color.hpp"

// Abstract base for anything drawn on screen in response to a note/frequency.
class Shape {
public:
    Shape(const Color& color);
    virtual ~Shape();

    virtual void update(float dt) = 0;

    // Prints an ASCII/ANSI representation of the shape's current state.
    virtual void draw() const = 0;

    // A shape reports itself as expired once its visual life is over,
    // so the caller can remove it from the active list.
    virtual bool isExpired() const = 0;

protected:
    Color color_;
};

class Circle : public Shape {
public:
    Circle(const Color& color, float radius);

    void update(float dt) override;
    void draw() const override;
    bool isExpired() const override;

private:
    float radius_;
    float life_;
};

class Square : public Shape {
public:
    Square(const Color& color, float side);

    void update(float dt) override;
    void draw() const override;
    bool isExpired() const override;

private:
    float side_;
    float life_;
};

// A ring that grows and fades over time, like a ripple in water.
class Ripple : public Shape {
public:
    Ripple(const Color& color, float startRadius);

    void update(float dt) override;
    void draw() const override;
    bool isExpired() const override;

private:
    float radius_;
    float alpha_;
};