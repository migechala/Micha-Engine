#pragma once

#include "Types.h"

namespace eng {

class Camera {
private:
  Vector2<float> p_position = {0.0f, 0.0f};
  Vector2<int> p_viewport = {0, 0};

public:
  Camera() = default;
  explicit Camera(Vector2<int> viewport) : p_viewport(viewport) {}

  void setViewport(Vector2<int> viewport) { p_viewport = viewport; }
  Vector2<int> getViewport() const { return p_viewport; }

  void setPosition(Vector2<float> position) { p_position = position; }
  void setPosition(Vector2<int> position) {
    p_position = {static_cast<float>(position.x), static_cast<float>(position.y)};
  }
  void move(Vector2<float> delta) { p_position += delta; }
  Vector2<float> getPosition() const { return p_position; }

  void centerOn(Vector2<int> worldPosition) {
    p_position = {static_cast<float>(worldPosition.x - p_viewport.x / 2),
                  static_cast<float>(worldPosition.y - p_viewport.y / 2)};
  }

  Vector2<int> worldToScreen(Vector2<int> worldPosition) const {
    return {static_cast<int>(worldPosition.x - p_position.x),
            static_cast<int>(p_viewport.y - (worldPosition.y - p_position.y))};
  }
};

} // namespace eng
