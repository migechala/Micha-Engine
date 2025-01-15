// Created by Mikhail Chalakov on 2/18/24.

#pragma once

#include <SDL2/SDL.h>

#include <memory>
#include <vector>

namespace eng {

// Struct to represent 2D vectors
struct Vector2i {
  int x, y;

  // Constructors
  Vector2i();
  Vector2i(int x, int y);

  // Operator overloads for vector arithmetic
  Vector2i operator+(const Vector2i& change) const;
  Vector2i operator-(const Vector2i& change) const;
  void operator+=(const Vector2i& change);
  Vector2i operator/(const Vector2i& change) const;
  Vector2i operator/(const int& change) const;
  Vector2i operator*(const Vector2i& change) const;
};

// ObjectOptions class for configuring objects
class ObjectOptions {
 private:
  eng::Vector2i p_position;
  eng::Vector2i p_size;
  eng::Vector2i p_velocity;
  eng::Vector2i p_acceleration;
  SDL_RendererFlip p_flip;
  SDL_Color p_color;
  bool p_gravity;
  int id;

 public:
  // Constructors
  ObjectOptions();

  // Builder pattern for setting options
  ObjectOptions& setPosition(eng::Vector2i position);
  ObjectOptions& setSize(eng::Vector2i size);
  ObjectOptions& setVelocity(eng::Vector2i velocity);
  ObjectOptions& setAcceleration(eng::Vector2i acceleration);
  ObjectOptions& setFlip(SDL_RendererFlip flip);
  ObjectOptions& setColor(SDL_Color color);
  ObjectOptions& enableGravity();
  void setId(int id);

  // Getters
  Vector2i getPosition() const;
  Vector2i getSize() const;
  Vector2i getVelocity() const;
  Vector2i getAcceleration() const;
  SDL_RendererFlip getFlip() const;
  SDL_Color getColor() const;
  bool isGravityEnabled() const;
  int getId();
};

// SpriteOptions class inheriting from ObjectOptions
class SpriteOptions : public ObjectOptions {
 private:
  std::vector<std::shared_ptr<SDL_Texture>> p_textures;
  std::vector<int> p_numSpritesPerSheet;
  eng::Vector2i p_spriteSize;
  int p_fptu;  // Frames per texture update

 public:
  // Constructors
  SpriteOptions();

  // Setters for sprite-specific options
  SpriteOptions& setTextures(
      std::vector<std::shared_ptr<SDL_Texture>> textures);
  SpriteOptions& setNumberOfSpritesPerSheet(std::vector<int> num);
  SpriteOptions& setRealSpriteSize(eng::Vector2i spriteSize);
  SpriteOptions& setFramesPerTextureUpdate(int numFrames);

  // Getters
  int getFramesPerTextureUpdate();
  std::vector<int> getNumberOfSpritesPerSheet();
  std::vector<std::shared_ptr<SDL_Texture>> getTextures();
  eng::Vector2i getRealSpriteSize();
};

// Object class to represent an object in the game world
class Object {
 protected:
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
  // Constructor accepting ObjectOptions
  Object(ObjectOptions& options);
  virtual ~Object() = default;

  // Check if the object is a sprite
  virtual bool isSprite();

  // Setters
  void setPosition(eng::Vector2i newPosition);
  void setVelocity(eng::Vector2i newVelocity);
  void setAcceleration(eng::Vector2i newAcceleration);

  // Getters
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

  // State setters
  void setRising(bool arg);
  void setGrounded(bool arg);
};

// Sprite class inheriting from Object, representing an animated object
class Sprite : public Object {
 private:
  std::vector<std::shared_ptr<SDL_Texture>> spritesheets;
  std::vector<std::vector<eng::Vector2i>> cutOuts;
  std::vector<int> numSpritesPerSheet;
  int spritesheetIndex;
  int currentCutIndex;
  int framesPerUpdate;
  SDL_Rect p_src;
  bool p_update;

  // Set the current sprite cut-out
  void setCutOut(eng::Vector2i pos);

 public:
  // Getter
  std::shared_ptr<SDL_Texture> getTexture();
  std::vector<Vector2i> getCutOuts();

  // Update and change sprite-related properties
  void updateTexture(int frame);
  void changeSpritesheet(int index);
  bool isSprite() override;
  void toggleUpdate();
  void setFramesPerUpdate(int fpu);
  SDL_Rect& getSrc();

  // Constructor accepting SpriteOptions
  Sprite(SpriteOptions& options);
};

}  // namespace eng
