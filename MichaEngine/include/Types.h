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
  Vector2i() = default;
  Vector2i(int x, int y);

  // Operator overloads for vector arithmetic
  Vector2i operator+(const Vector2i &change) const;
  Vector2i operator-(const Vector2i &change) const;
  Vector2i operator/(const Vector2i &change) const;
  Vector2i operator/(int change) const;
  Vector2i operator*(const Vector2i &change) const;
  void operator+=(const Vector2i &change);
  void operator=(const Vector2i &newVector);
  void operator-=(const Vector2i &change);
};

// Class for configuring objects
class SpriteOptions {
private:
  eng::Vector2i p_position, p_size, p_velocity, p_acceleration, p_hitbox, p_hitboxOffset;
  SDL_RendererFlip p_flip;
  SDL_Color p_color;
  bool p_gravity = false;
  int id = -1;

  std::vector<std::shared_ptr<SDL_Texture>> p_textures;
  std::vector<int> p_numSpritesPerSheet;
  eng::Vector2i p_realSpriteSize;
  int p_fptu = 0; // Frames per texture update

public:
  SpriteOptions() = default;
  virtual ~SpriteOptions() = default;

  // Builder pattern for setting options
  SpriteOptions &setPosition(eng::Vector2i newPos);
  SpriteOptions &setSize(eng::Vector2i newSize);
  SpriteOptions &setHitbox(eng::Vector2i size);
  SpriteOptions &setHitboxOffset(eng::Vector2i offset);
  SpriteOptions &setVelocity(eng::Vector2i velocity);
  SpriteOptions &setAcceleration(eng::Vector2i acceleration);
  SpriteOptions &setFlip(SDL_RendererFlip flip);
  SpriteOptions &setColor(SDL_Color color);
  SpriteOptions &enableGravity();
  SpriteOptions &setTextures(std::vector<std::shared_ptr<SDL_Texture>> textures);
  SpriteOptions &setNumberOfSpritesPerSheet(std::vector<int> num);
  SpriteOptions &setRealSpriteSize(eng::Vector2i spriteSize);
  SpriteOptions &setFramesPerTextureUpdate(int numFrames);

  // Getters
  inline int getFramesPerTextureUpdate() const { return p_fptu; }
  inline std::vector<int> getNumberOfSpritesPerSheet() const { return p_numSpritesPerSheet; }
  inline std::vector<std::shared_ptr<SDL_Texture>> getTextures() const { return p_textures; }
  inline eng::Vector2i getRealSpriteSize() const { return p_realSpriteSize; }

  inline void setId(int id) { this->id = id; }
  inline int getId() const { return id; }

  inline Vector2i getPosition() const { return p_position; }
  inline Vector2i getSize() const { return p_size; }
  inline Vector2i getHitbox() const { return p_hitbox; }
  inline Vector2i getHitboxOffset() const { return p_hitboxOffset; }
  inline Vector2i getVelocity() const { return p_velocity; }
  inline Vector2i getAcceleration() const { return p_acceleration; }
  inline SDL_RendererFlip getFlip() const { return p_flip; }
  inline SDL_Color getColor() const { return p_color; }
  inline bool isGravityEnabled() const { return p_gravity; }
};

/*


  eng::Vector2i p_position, p_size, p_velocity, p_acceleration, p_hitbox, p_hitboxOffset;
  SDL_RendererFlip p_flip;
  SDL_Color p_color;
  bool p_gravity = false;
  int id = -1;

  std::vector<std::shared_ptr<SDL_Texture>> p_textures;
  std::vector<int> p_numSpritesPerSheet;
  eng::Vector2i p_realSpriteSize;

*/

// Sprite class representing an animated object
class Sprite {
private:
  std::vector<std::vector<eng::Vector2i>> cutOuts;
  std::vector<int> numSpritesPerSheet;
  int spritesheetIndex = 0;
  int currentCutIndex = 0;
  int framesPerUpdate = 0;
  SDL_Rect p_src{};
  bool p_update = false;

  inline void setCutOut(eng::Vector2i pos) {
    p_src.x = pos.x;
    p_src.y = pos.y;
  }

  bool p_rising = false;
  bool p_grounded = false;
  bool p_animated = false;
  double p_angle = 0.0;
  SpriteOptions p_options;

public:
  explicit Sprite(SpriteOptions &options);
  ~Sprite() = default;

  inline std::shared_ptr<SDL_Texture> getTexture() { return getOptions().getTextures()[spritesheetIndex]; }
  inline std::vector<Vector2i> getCutOuts() { return cutOuts[spritesheetIndex]; }

  void updateTexture(int frame);
  void changeSpritesheet(int index);
  inline void toggleUpdate() { p_update = !p_update; }
  inline void setFramesPerUpdate(int fpu) { framesPerUpdate = fpu; }
  inline SDL_Rect &getSrc() { return p_src; }

  // Setters
  inline void setPosition(eng::Vector2i newPosition) { p_options.setPosition(newPosition); }
  inline void setVelocity(eng::Vector2i newVelocity) { p_options.setVelocity(newVelocity); }
  inline void setAcceleration(eng::Vector2i newAcceleration) { p_options.setAcceleration(newAcceleration); }
  inline void setRising(bool arg) { p_rising = arg; }
  inline void setGrounded(bool arg) { p_grounded = arg; }

  // Getters
  inline int getId() { return p_options.getId(); }
  inline SpriteOptions getOptions() { return p_options; }
  inline float getAngle() { return p_angle; }
  inline bool isRising() { return p_rising; }
  inline bool isGrounded() { return p_grounded; }
};

} // namespace eng
