//
// Created by Mikhail Chalakov on 2/18/24.
//

#pragma once
#include <SDL2/SDL.h>

#include <vector>

namespace type {
struct Vector2i {
  int x, y;

  Vector2i(int x, int y);

  Vector2i operator+(const Vector2i &change) const;
  Vector2i operator-(const Vector2i &change) const;

  void operator+=(const Vector2i &change);

  Vector2i operator/(const Vector2i &change) const;
  Vector2i operator/(const int &change) const;
  Vector2i operator*(const Vector2i &change) const;
};

class Object {
  int p_id;
  type::Vector2i p_position;
  type::Vector2i p_velocity;
  type::Vector2i p_acceleration;
  type::Vector2i p_size;
  SDL_Color p_color;
  bool p_gravity;
  bool p_rising;
  bool p_grounded;
  bool p_animated;
  double p_angle;
  SDL_RendererFlip p_flip;

 public:
  // Object(type::Vector2i position, type::Vector2i size, SDL_Color color);
  Object();
  virtual ~Object() = default;
  virtual bool isSprite();

  // Builder
  Object *gravityOn();
  Object *setVelocity(type::Vector2i velocity);
  Object *setAcceleration(type::Vector2i acceleration);
  Object *setPosition(Vector2i newPos);
  Object *setSize(Vector2i newSize);
  Object *setColor(SDL_Color color);
  Object *setId(int id);
  Object *setFlip(SDL_RendererFlip flip);

  // Getter
  int getId();
  type::Vector2i getPosition();
  type::Vector2i getSize();
  type::Vector2i getVelocity();
  type::Vector2i getAcceleration();

  float getAngle();

  SDL_RendererFlip getFlip();

  SDL_Color getColor();

  bool isRising();
  bool isGrounded();
  bool hasGravity();

  void setRising(bool arg);
  void setGrounded(bool arg);
};
//
class Sprite : public Object {
  std::vector<std::shared_ptr<SDL_Texture>> spritesheets;
  std::vector<type::Vector2i> cutOuts;
  std::vector<int> numSpritesPerSheet;
  int spritesheetIndex;
  int currentCutIndex;
  SDL_Rect p_src;
  void setCutOut(type::Vector2i pos);

 public:
  // Getter
  std::shared_ptr<SDL_Texture> getTexture();
  std::vector<Vector2i> *getCutOuts();
  // Functions
  void updateTexture();
  void changeSpritesheet(int index);

  bool isSprite() override;

  SDL_Rect &getSrc();

  // Constructor
  Sprite(std::vector<std::shared_ptr<SDL_Texture>> textures,
         std::vector<int> numSpritesPerSheet, type::Vector2i spriteSize);
};
}  // namespace type
