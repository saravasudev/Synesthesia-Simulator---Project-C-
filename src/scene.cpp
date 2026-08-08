#include "scene.hpp"
#include <algorithm>

void Scene::spawn(std::unique_ptr<Shape> shape) {
    shapes_.push_back(std::move(shape));
}

void Scene::update(float dt) {
    std::for_each(shapes_.begin(), shapes_.end(),
                  [dt](const std::unique_ptr<Shape>& s) { s->update(dt); });

    shapes_.erase(
        std::remove_if(shapes_.begin(), shapes_.end(),
                        [](const std::unique_ptr<Shape>& s) { return s->isExpired(); }),
        shapes_.end());
}

void Scene::draw() const {
    for (const auto& shape : shapes_) {
        shape->draw();
    }
}