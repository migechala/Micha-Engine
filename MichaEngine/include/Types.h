// Created by Mikhail Chalakov on 2/18/24.

#pragma once

#include <SDL2/SDL.h>

#include <memory>
#include <vector>

namespace eng {
class Visitor;

// Struct to represent 2D vectors
struct Vector2i {
  int x, y;

  // Constructors
  Vector2i();
  Vector2i(int x, int y);

  // Operator overloads for vector arithmetic
  Vector2i operator+(const Vector2i& change) const;
  Vector2i operator-(const Vector2i& change) const;
  Vector2i operator/(const Vector2i& change) const;
  Vector2i operator/(const int& change) const;
  Vector2i operator*(const Vector2i& change) const;
  void operator+=(const Vector2i& change);
  void operator-=(const Vector2i& change);
};

// ObjectOptions class for configuring objects
class ObjectOptions {
 private:
  eng::Vector2i p_position;
  eng::Vector2i p_size;
  eng::Vector2i p_velocity;
  eng::Vector2i p_acceleration;
  eng::Vector2i p_hitbox;
  eng::Vector2i p_hitboxOffset;
  SDL_RendererFlip p_flip;
  SDL_Color p_color;
  bool p_gravity;
  int id;

 public:
  // Constructors
  ObjectOptions() = default;

  // Destructor
  virtual ~ObjectOptions() = default;

  // Builder pattern for setting options
  ObjectOptions& setPosition(eng::Vector2i newPos) {
    p_position = newPos;
    return (*this);
  }

  ObjectOptions& setSize(eng::Vector2i newSize) {
    p_size = newSize;
    return (*this);
  }

  ObjectOptions& setHitbox(eng::Vector2i size) {
    p_hitbox = size;
    return (*this);
  }

  ObjectOptions& setHitboxOffset(eng::Vector2i offset) {
    p_hitboxOffset = offset;
    return (*this);
  }

  ObjectOptions& setVelocity(eng::Vector2i velocity) {
    p_velocity = velocity;
    return (*this);
  }

  ObjectOptions& setAcceleration(eng::Vector2i acceleration) {
    p_acceleration = acceleration;
    return (*this);
  }

  ObjectOptions& setFlip(SDL_RendererFlip flip) {
    p_flip = flip;
    return (*this);
  }

  ObjectOptions& setColor(SDL_Color color) {
    p_color = color;
    return (*this);
  }

  ObjectOptions& enableGravity() {
    p_gravity = true;
    return (*this);
  }

  inline void setId(int id) { id = id; }

  // Getters
  Vector2i getPosition() const { return p_position; }
  Vector2i getSize() const { return p_size; }
  Vector2i getHitbox() const { return p_hitbox; }
  Vector2i getHitboxOffset() const { return p_hitboxOffset; }
  Vector2i getVelocity() const { return p_velocity; }
  Vector2i getAcceleration() const { return p_acceleration; }
  SDL_RendererFlip getFlip() const { return p_flip; }
  SDL_Color getColor() const { return p_color; }
  bool isGravityEnabled() const { return p_gravity; }
  int getId() const { return id; };
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
  SpriteOptions() = default;

  // Desturctor
  virtual ~SpriteOptions() = default;

  // Setters for sprite-specific options
  SpriteOptions& setTextures(
      std::vector<std::shared_ptr<SDL_Texture>> textures) {
    p_textures = std::move(textures);
    return (*this);
  }

  SpriteOptions& setNumberOfSpritesPerSheet(std::vector<int> num) {
    p_numSpritesPerSheet = std::move(num);
    return (*this);
  }

  SpriteOptions& setRealSpriteSize(eng::Vector2i spriteSize) {
    p_spriteSize = spriteSize;
    return (*this);
  }

  SpriteOptions& setFramesPerTextureUpdate(int numFrames) {
    p_fptu = numFrames;
    return (*this);
  }

  // Getters
  int getFramesPerTextureUpdate() const { return p_fptu; }
  std::vector<int> getNumberOfSpritesPerSheet() const {
    return p_numSpritesPerSheet;
  }
  std::vector<std::shared_ptr<SDL_Texture>> getTextures() const {
    return p_textures;
  }
  eng::Vector2i getRealSpriteSize() const { return p_spriteSize; }
};

// Object class to represent an object in the game world
class Object {
 protected:
  bool p_rising;
  bool p_grounded;
  bool p_animated;
  double p_angle;
  SDL_RendererFlip p_flip;
  ObjectOptions p_options;

 public:
  // Constructor accepting ObjectOptions
  Object(ObjectOptions& options);
  virtual ~Object() = default;
  virtual void accept(Visitor& visitor);

  // Setters
  void setPosition(eng::Vector2i newPosition);
  void setVelocity(eng::Vector2i newVelocity);
  void setAcceleration(eng::Vector2i newAcceleration);

  // Getters
  int getId();
  ObjectOptions getOptions();
  float getAngle();
  SDL_RendererFlip getFlip();
  bool isRising();
  bool isGrounded();

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
  // Constructor accepting SpriteOptions
  Sprite(SpriteOptions& options);
  virtual ~Sprite() = default;
  // Getter
  std::shared_ptr<SDL_Texture> getTexture();
  std::vector<Vector2i> getCutOuts();

  // Update and change sprite-related properties
  void updateTexture(int frame);
  void changeSpritesheet(int index);
  void toggleUpdate();
  void setFramesPerUpdate(int fpu);
  SDL_Rect& getSrc();

  virtual void accept(Visitor& visitor) override;
};

class Visitor {
 public:
  virtual void visit(Object& obj) = 0;
  virtual void visit(Sprite& sprite) = 0;
  virtual ~Visitor() = default;
};

class UpdateTextureVisitor : public Visitor {
  int frame;

 public:
  UpdateTextureVisitor(int frame);
  void visit(Object& obj) override;

  void visit(Sprite& sprite) override;
};

}  // namespace eng
