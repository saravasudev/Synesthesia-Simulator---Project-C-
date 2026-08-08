#pragma once

#include <string>
#include "color.hpp"

// Abstract base for anything drawn to the console in response to a note.
// Each shape owns a horizontal offset (indent_) so a burst of shapes
// spawned from one note can be staggered across the terminal instead of
// stacking in an identical column.
class Shape {
public:
    Shape(const Color& color, int indent);
    virtual ~Shape();

    virtual void update(float dt) = 0;

    // Prints one line: leading indent, then an ASCII/ANSI representation
    // of the shape's current state.
    virtual void draw() const = 0;

    // A shape reports itself as expired once its visual life is over,
    // so the caller can remove it from the active list.
    virtual bool isExpired() const = 0;

protected:
    void drawLine(char glyph, int width, float alpha = 255.0f) const;

    Color color_;
    int indent_;
};

class Circle : public Shape {
public:
    Circle(const Color& color, int indent, float radius);

    void update(float dt) override;
    void draw() const override;
    bool isExpired() const override;

private:
    float radius_;
    float life_;
};

class Square : public Shape {
public:
    Square(const Color& color, int indent, float side);

    void update(float dt) override;
    void draw() const override;
    bool isExpired() const override;

private:
    float side_;
    float life_;
};

// A ring that grows and fades over time, like a ripple in water.
// Ripples are the longest-lived shape and form the "wash" of color
// that lingers in the terminal after a note is played.
class Ripple : public Shape {
public:
    Ripple(const Color& color, int indent, float startRadius);

    void update(float dt) override;
    void draw() const override;
    bool isExpired() const override;

private:
    float radius_;
    float alpha_;
};