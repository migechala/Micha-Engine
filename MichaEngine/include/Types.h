//
// Created by Mikhail Chalakov on 2/18/24.
//

#pragma once
#include <SDL2/SDL.h>

#include <vector>

namespace eng {
struct Vector2i {
  int x, y;

  Vector2i();
  Vector2i(int x, int y);

  Vector2i operator+(const Vector2i &change) const;
  Vector2i operator-(const Vector2i &change) const;

  void operator+=(const Vector2i &change);

  Vector2i operator/(const Vector2i &change) const;
  Vector2i operator/(const int &change) const;
  Vector2i operator*(const Vector2i &change) const;
};
class ObjectOptions {
  eng::Vector2i p_position;
  eng::Vector2i p_size;
  eng::Vector2i p_velocity;
  eng::Vector2i p_acceleration;
  SDL_RendererFlip p_flip;
  SDL_Color p_color;
  bool p_gravity;

 public:
  ObjectOptions();
  // Builder
  ObjectOptions &setPosition(eng::Vector2i position);
  ObjectOptions &setSize(eng::Vector2i size);
  ObjectOptions &setVelocity(eng::Vector2i velocity);
  ObjectOptions &setAcceleration(eng::Vector2i acceleration);
  ObjectOptions &setFlip(SDL_RendererFlip flip);
  ObjectOptions &setColor(SDL_Color color);
  ObjectOptions &enableGravity();

  // Getters
  Vector2i getPosition() const;
  Vector2i getSize() const;
  Vector2i getVelocity() const;
  Vector2i getAcceleration() const;
  SDL_RendererFlip getFlip() const;
  SDL_Color getColor() const;
  bool isGravityEnabled() const;
};
class SpriteOptions : public ObjectOptions {
  std::vector<std::shared_ptr<SDL_Texture>> p_textures;
  std::vector<int> p_numSpritesPerSheet;
  eng::Vector2i p_spriteSize;
  int p_fptu;

 public:
  SpriteOptions();
  // Setters
  SpriteOptions &setTextures(
      std::vector<std::shared_ptr<SDL_Texture>> textures);
  SpriteOptions &setNumberOfSpritesPerSheet(std::vector<int> num);
  SpriteOptions &setRealSpriteSize(eng::Vector2i spriteSize);
  SpriteOptions &setFramesPerTextureUpdate(int numFrames);
  // Getters

  int getFramesPerTextureUpdate();

  std::vector<int> getNumberOfSpritesPerSheet();
  std::vector<std::shared_ptr<SDL_Texture>> getTextures();
  eng::Vector2i getRealSpriteSize();
};

class Object {
  int p_id;
  eng::Vector2i p_position;
  eng::Vector2i p_velocity;
  eng::Vector2i p_acceleration;
  eng::Vector2i p_size;
  SDL_Color p_color;
  bool p_gravity;
  bool p_rising;
  bool p_grounded;
  bool p_animated;
  double p_angle;
  SDL_RendererFlip p_flip;

 public:
  Object(ObjectOptions &options);
  virtual ~Object() = default;
  virtual bool isSprite();

  // Setters
  void setPosition(eng::Vector2i newPosition);
  void setVelocity(eng::Vector2i newVelocity);
  void setAcceleration(eng::Vector2i newAcceleration);
  // Getter
  int getId();
  eng::Vector2i getPosition();
  eng::Vector2i getSize();
  eng::Vector2i getVelocity();
  eng::Vector2i getAcceleration();

  float getAngle();

  SDL_RendererFlip getFlip();

  SDL_Color getColor();

  bool isRising();
  bool isGrounded();
  bool hasGravity();

  void setRising(bool arg);
  void setGrounded(bool arg);
};
class Sprite : public Object {
  std::vector<std::shared_ptr<SDL_Texture>> spritesheets;
  std::vector<std::vector<eng::Vector2i>> cutOuts;
  std::vector<int> numSpritesPerSheet;
  int spritesheetIndex;
  int currentCutIndex;
  int framesPerUpdate;
  SDL_Rect p_src;
  bool p_update;
  void setCutOut(eng::Vector2i pos);

 public:
  // Getter
  std::shared_ptr<SDL_Texture> getTexture();
  std::vector<Vector2i> getCutOuts();
  // Functions
  void updateTexture(int frame);
  void changeSpritesheet(int index);
  bool isSprite() override;
  void toggleUpdate();
  void setFramesPerUpdate(int fpu);

  SDL_Rect &getSrc();

  // Constructor
  Sprite(SpriteOptions &options);
};

}  // namespace eng
