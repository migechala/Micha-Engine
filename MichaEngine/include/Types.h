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

struct Object {
  type::Vector2i position;
  type::Vector2i velocity;
  type::Vector2i acceleration;
  type::Vector2i size;
  SDL_Rect dst;
  SDL_Rect src;
  SDL_Color color;
  bool rising;
  bool grounded;
  bool animated;

  Object(type::Vector2i position, type::Vector2i size, SDL_Color color);
  Object(type::Vector2i position, type::Vector2i size, type::Vector2i velocity,
         type::Vector2i acceleration, SDL_Color color);
  virtual ~Object() = default;
  virtual bool isSprite();
};
class Sprite : public Object {
  std::vector<SDL_Texture *> spritesheets;
  int spritesheetIndex;
  type::Vector2i spriteSize;

 public:
  // Setter
  void setPosition(Vector2i newPos);
  void setSize(Vector2i newSize);

  // Changer
  void changePosition(Vector2i change);

  // Getter
  SDL_Texture *getTexture();
  // Functions
  void updateTexture();
  void changeSpritesheet(int index);

  bool isSprite() override;

  // Constructor
  Sprite(type::Vector2i position, type::Vector2i size,
         std::vector<SDL_Texture *> textures, type::Vector2i spriteSize);
  Sprite(type::Vector2i position, type::Vector2i size, type::Vector2i velocity,
         type::Vector2i acceleration, std::vector<SDL_Texture *> textures,
         type::Vector2i spriteSize);
};
}  // namespace type
