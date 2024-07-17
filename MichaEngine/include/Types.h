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

struct Vector2d {
  double x, y;

  Vector2d(double x, double y);

  Vector2d operator+(const Vector2d &change) const;

  void operator+=(const Vector2d &change);

  Vector2d operator/(const Vector2d &change) const;
  Vector2d operator*(const Vector2d &change) const;
};
struct Object {
  Object(type::Vector2i position, type::Vector2i size, SDL_Color color);
  Object(type::Vector2i position, type::Vector2i size, type::Vector2i velocity,
         type::Vector2i acceleration, SDL_Color color);

  type::Vector2i position;
  type::Vector2i velocity;
  type::Vector2i acceleration;
  type::Vector2i size;
  SDL_Rect dst;
  SDL_Rect src;
  SDL_Color color;
  bool rising;
  bool animated;
};
class Sprite : public Object {
  int counter{};
  std::vector<SDL_Texture *> textures;

 public:
  // Setter
  void setPosition(Vector2i newPos);
  void setSize(Vector2i newSize);

  // Changer
  void changePosition(Vector2i change);

  // Getter
  type::Vector2i getPosition() const;
  SDL_Texture *getTexture();

  // Functions
  int updateTexture();

  // Constructor
  Sprite(type::Vector2i position, type::Vector2i size);
  Sprite(type::Vector2i position, type::Vector2i size, type::Vector2i velocity);
  Sprite(type::Vector2i position, type::Vector2i size, type::Vector2i velocity,
         type::Vector2i acceleration);
  Sprite(type::Vector2i position, type::Vector2i size,
         std::vector<SDL_Texture *> textures);
  Sprite(type::Vector2i position, type::Vector2i size, type::Vector2i velocity,
         type::Vector2i acceleration, std::vector<SDL_Texture *> textures);
};
}  // namespace type
