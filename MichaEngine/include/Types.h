// Created by Mikhail Chalakov on 2/18/24.

#pragma once

#include <SDL2/SDL.h>
#include <memory>
#include <vector>

namespace eng {

template <typename T>
concept Numeric = std::is_arithmetic_v<T>;

template <Numeric T> struct Vector2 {
  T x, y;

  // Constructors
  Vector2() = default;
  Vector2(T x, T y) : x(x), y(y) {}

  // Arithmetic operators

  template <Numeric U> Vector2<T> operator+(const Vector2<U> &other) const { return Vector2(x + other.x, y + other.y); }
  template <Numeric U> Vector2<T> operator-(const Vector2<U> &other) const { return Vector2(x - other.x, y - other.y); }
  template <Numeric U> Vector2<T> operator/(U scalar) const {
    return {x / static_cast<T>(scalar), y / static_cast<T>(scalar)};
  }
  template <Numeric U> Vector2<T> operator*(const Vector2<U> &other) const { return Vector2(x * other.x, y * other.y); }

  void operator+=(const Vector2 &other) {
    x += other.x;
    y += other.y;
  }
  void operator-=(const Vector2 &other) {
    x -= other.x;
    y -= other.y;
  }

  /**
   * Calculate the dot product with another vector.
   * \param other The other vector.
   * \return The dot product as a scalar value.
   */
  template <Numeric U> T dot(const Vector2<U> &other) const { return x * other.x + y * other.y; }
  /**
   * Calculate the length (magnitude) of the vector.
   * \return The length as a scalar value.
   */
  T length() const { return std::sqrt(x * x + y * y); }
};

// Class for configuring objects
class SpriteOptions {
private:
  eng::Vector2<int> p_position, p_size, p_hitbox, p_hitboxOffset;
  eng::Vector2<float> p_velocity, p_acceleration;
  SDL_RendererFlip p_flip;
  SDL_Color p_color;
  bool p_gravity = false;
  int id = -1;

  std::vector<std::shared_ptr<SDL_Texture>> p_textures;
  std::vector<int> p_numSpritesPerSheet;
  eng::Vector2<int> p_realSpriteSize;
  int p_fptu = 0; // Frames per texture update

public:
  SpriteOptions() = default;
  virtual ~SpriteOptions() = default;

  // Builder pattern for setting options
  /**
   * Set the position of the sprite.
   * \param newPos The new position as a Vector2<int>.
   * \return Reference to this SpriteOptions instance for chaining.
   */
  SpriteOptions &setPosition(eng::Vector2<int> newPos);
  /**
   * Set the size of the sprite.
   * \param newSize The new size as a Vector2<int>.
   * \return Reference to this SpriteOptions instance for chaining.
   */
  SpriteOptions &setSize(eng::Vector2<int> newSize);
  /**
   * Set the size of the hitbox.
   * \param newSize The new hitbox size as a Vector2<int>.
   * \return Reference to this SpriteOptions instance for chaining.
   */
  SpriteOptions &setHitbox(eng::Vector2<int> size);
  /**
   * Set the offset of the hitbox.
   * \param offset The offset of the hitbox as a Vector2<int>.
   * \return Reference to this SpriteOptions instance for chaining.
   */
  SpriteOptions &setHitboxOffset(eng::Vector2<int> offset);
  /**
   * Set the velocity of the sprite.
   * \param velocity The new velocity as a Vector2<float>.
   * \return Reference to this SpriteOptions instance for chaining.
   */
  SpriteOptions &setVelocity(eng::Vector2<float> velocity);
  /**
   * Set the acceleration of the sprite.
   * \param acceleration The new acceleration as a Vector2<float>.
   * \return Reference to this SpriteOptions instance for chaining.
   */
  SpriteOptions &setAcceleration(eng::Vector2<float> acceleration);
  /**
   * Set the flip mode of the sprite.
   * \param flip The SDL_RendererFlip value to set.
   * \return Reference to this SpriteOptions instance for chaining.
   */
  SpriteOptions &setFlip(SDL_RendererFlip flip);
  /**
   * Set the color of the sprite.
   * \param color The SDL_Color to set.
   * \return Reference to this SpriteOptions instance for chaining.
   */
  SpriteOptions &setColor(SDL_Color color);
  /**
   * Enable or disable gravity for the sprite.
   * \return Reference to this SpriteOptions instance for chaining.
   */
  SpriteOptions &enableGravity();
  /**
   * Set the textures of the sprite.
   * \param textures The new textures as a vector of shared pointers to SDL_Texture.
   * \return Reference to this SpriteOptions instance for chaining.
   */
  SpriteOptions &setTextures(std::vector<std::shared_ptr<SDL_Texture>> textures, bool surpress = true);
  /**
   * Set the number of sprites per sheet.
   * \param num The new number of sprites per sheet as a vector of integers.
   * \return Reference to this SpriteOptions instance for chaining.
   */
  SpriteOptions &setNumberOfSpritesPerSheet(std::vector<int> num);
  /**
   * Set the real sprite size.
   * \param spriteSize The new real sprite size as a Vector2<int>.
   * \return Reference to this SpriteOptions instance for chaining.
   */
  SpriteOptions &setRealSpriteSize(eng::Vector2<int> spriteSize);
  /**
   * Set the frames per texture update.
   * \param numFrames The number of frames per texture update.
   * \return Reference to this SpriteOptions instance for chaining.
   */
  SpriteOptions &setFramesPerTextureUpdate(int numFrames);

  // Getters
  inline int getFramesPerTextureUpdate() const { return p_fptu; }
  inline std::vector<int> getNumberOfSpritesPerSheet() const { return p_numSpritesPerSheet; }
  inline std::vector<std::shared_ptr<SDL_Texture>> getTextures() const { return p_textures; }
  inline eng::Vector2<int> getRealSpriteSize() const { return p_realSpriteSize; }

  inline void setId(int id) { this->id = id; }
  inline int getId() const { return id; }

  inline Vector2<int> getPosition() const { return p_position; }
  inline Vector2<int> getSize() const { return p_size; }
  inline Vector2<int> getHitbox() const { return p_hitbox; }
  inline Vector2<int> getHitboxOffset() const { return p_hitboxOffset; }
  inline Vector2<float> getVelocity() const { return p_velocity; }
  inline Vector2<float> getAcceleration() const { return p_acceleration; }
  inline SDL_RendererFlip getFlip() const { return p_flip; }
  inline SDL_Color getColor() const { return p_color; }
  inline bool isGravityEnabled() const { return p_gravity; }
};

class Sprite {
private:
  std::vector<std::vector<eng::Vector2<int>>> cutOuts;
  std::vector<int> numSpritesPerSheet;
  int spritesheetIndex = 0;
  int currentCutIndex = 0;
  int framesPerUpdate = 0;
  SDL_Rect p_src{};
  bool p_update = false;

  inline void setCutOut(eng::Vector2<int> pos) {
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

  /**
   * Get the current texture of the sprite.
   * \return Shared pointer to the current SDL_Texture.
   */
  inline std::shared_ptr<SDL_Texture> getTexture() { return getOptions().getTextures()[spritesheetIndex]; }
  /**
   * Get the cutouts for the current spritesheet.
   * \return Vector of Vector2<int> representing the cutouts.
   */
  inline std::vector<Vector2<int>> getCutOuts() { return cutOuts[spritesheetIndex]; }

  /**
   * Update the texture based on the current frame.
   * \param frame The current frame number.
   */
  void updateTexture(int frame);
  /**
   * Change the current spritesheet.
   * \param index The index of the new spritesheet.
   */
  void changeSpritesheet(int index);
  /**
   * Toggle the update state of the sprite.
   */
  inline void toggleUpdate() { p_update = !p_update; }
  /**
   * Set the number of frames per update.
   * \param fpu The number of frames per update.
   */
  inline void setFramesPerUpdate(int fpu) { framesPerUpdate = fpu; }
  /**
   * Get the source rectangle of the sprite.
   * \return Reference to the SDL_Rect representing the source rectangle.
   */
  inline SDL_Rect &getSrc() { return p_src; }

  // Setters
  /**
   * Set the position of the sprite.
   * \param newPosition The new position as a Vector2<int>.
   */
  inline void setPosition(eng::Vector2<int> newPosition) { p_options.setPosition(newPosition); }
  /**
   * Set the angle of the sprite.
   * \param angle The new angle in degrees.
   */
  inline void setVelocity(eng::Vector2<float> newVelocity) { p_options.setVelocity(newVelocity); }
  /**
   *  Set the acceleration of the sprite.
   * \param newAcceleration The new acceleration as a Vector2<float>.
   */
  inline void setAcceleration(eng::Vector2<float> newAcceleration) { p_options.setAcceleration(newAcceleration); }
  /**
   * Set the rising state of the sprite.
   * \param arg The new rising state.
   */
  inline void setRising(bool arg) { p_rising = arg; }
  /**
   * Set the grounded state of the sprite.
   * \param arg The new grounded state.
   */
  inline void setGrounded(bool arg) { p_grounded = arg; }

  // Getters
  /**
   * Get the ID of the sprite.
   * \return The ID of the sprite.
   */
  inline int getId() { return p_options.getId(); }
  /**
   * Get the options of the sprite.
   * \return The options of the sprite.
   */
  inline SpriteOptions getOptions() { return p_options; }
  /**
   * Get the angle of the sprite.
   * \return The angle of the sprite in degrees.
   */
  inline float getAngle() { return p_angle; }
  /**
   * Check if the sprite is rising.
   * \return True if the sprite is rising, false otherwise.
   */
  inline bool isRising() { return p_rising; }
  /**
   * Check if the sprite is grounded.
   * \return True if the sprite is grounded, false otherwise.
   */
  inline bool isGrounded() { return p_grounded; }
};

} // namespace eng
