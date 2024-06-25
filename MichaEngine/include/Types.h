//
// Created by Mikhail Chalakov on 2/18/24.
//

#pragma once
#include <SDL2/SDL.h>

#include <vector>

namespace type {
struct Vector2i {
  int x, y;

  Vector2i(int p_x, int p_y);

  Vector2i operator+(const Vector2i &p_change) const;
  Vector2i operator-(const Vector2i &p_change) const;

  void operator+=(const Vector2i &p_change);

  Vector2i operator/(const Vector2i &p_change) const;
  Vector2i operator/(const int &p_change) const;
  Vector2i operator*(const Vector2i &p_change) const;
};

struct Vector2d {
  double x, y;

  Vector2d(double p_x, double p_y);

  Vector2d operator+(const Vector2d &p_change) const;

  void operator+=(const Vector2d &p_change);

  Vector2d operator/(const Vector2d &p_change) const;
  Vector2d operator*(const Vector2d &p_change) const;
};
struct Object {
  Object(type::Vector2i position, type::Vector2i size, SDL_Color color);
  Object(type::Vector2i position, type::Vector2i size, type::Vector2i velocity,
         SDL_Color color);

  type::Vector2i position;
  type::Vector2i velocity;
  type::Vector2i size;
  SDL_Rect dst;
  SDL_Rect src;
  SDL_Color color;
  bool rising;
  bool animated;
};
class Sprite : public Object {
  int m_counter{};
  std::vector<SDL_Texture *> textures;

 public:
  // Setter
  void setPosition(Vector2i p_newPos);
  void setSize(Vector2i p_newSize);

  // Changer
  void changePosition(Vector2i p_change);

  // Getter
  type::Vector2i getPosition() const;
  SDL_Texture *getTexture();

  // Functions
  int updateTexture();

  // Constructor
  Sprite(type::Vector2i p_position, type::Vector2i p_size);
  Sprite(type::Vector2i p_position, type::Vector2i p_size,
         type::Vector2i p_velocity);
  Sprite(type::Vector2i p_position, type::Vector2i p_size,
         std::vector<SDL_Texture *> p_textures);
  Sprite(type::Vector2i p_position, type::Vector2i p_size,
         type::Vector2i p_velocity, std::vector<SDL_Texture *> p_textures);
};
}  // namespace type
