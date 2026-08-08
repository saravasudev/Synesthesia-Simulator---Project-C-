#pragma once

#include <deque>
#include <memory>
#include "shape.hpp"

// Owns and drives every active Shape spawned in response to a note.
class Scene {
public:
    void spawn(std::unique_ptr<Shape> shape);

    // Advances every shape and removes ones that have expired.
    void update(float dt);

    void draw() const;

    std::size_t activeCount() const { return shapes_.size(); }

private:
    std::deque<std::unique_ptr<Shape>> shapes_;
};