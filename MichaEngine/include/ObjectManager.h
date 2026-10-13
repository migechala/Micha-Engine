#pragma once

#include <memory>
#include <vector>

#include "Types.h"

class ObjectManager {
private:
  std::vector<std::shared_ptr<eng::Sprite>> sprites;
  std::vector<int> freeObjectLoc;
  bool isValidId(int objId) const;
  eng::Vector2<int> frameSize = {0, 0};
  int top(int id), bottom(int id), left(int id), right(int id);
  bool hasSolidGroundBelow(int objId);
  void resolveSolidCollisions(int objId, eng::Vector2<float> attemptedVelocity);
  float gravityStrength;

public:
  // Constructor
  ObjectManager();

  // Singleton instance retrieval
  static ObjectManager *getInstance();

  // Object management methods
  int addSprite(std::shared_ptr<eng::Sprite> newSprite);
  void removeSprite(int objId);
  int updateSprite(int objId, int frame);
  int updateAllSprites(int frame);

  bool doesExist(int objId);

  // Accessor methods
  std::shared_ptr<eng::Sprite> getSprite(int objId);

  // Collision detection
  bool collide(int idA, int idB);

  // Debugging and utility methods
  int getNumObjects();

  // Update Frame Size
  void updateFrameSize(eng::Vector2<int> newSize);
  eng::Vector2<int> getFrameSize() const { return frameSize; }

  /**
   * Set the strength of gravity
   * \param gravity the new gravity strength
   */
  void setGravity(float gravity) { gravityStrength = gravity; }

  // Destructor
  ~ObjectManager() = default;
};
