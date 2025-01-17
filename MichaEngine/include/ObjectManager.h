#pragma once

#include <memory>
#include <vector>

#include "Types.h"

class ObjectManager {
 private:
  std::vector<std::shared_ptr<eng::Object>> objects;
  std::vector<int> freeObjectLoc;
  static ObjectManager* instance;
  int top(int id), bottom(int id), left(int id), right(int id);

 public:
  // Frame size used for position calculations
  eng::Vector2i frameSize = {0, 0};
  // Constructor
  ObjectManager();

  // Singleton instance retrieval
  static ObjectManager* getInstance();

  // Object management methods
  int addObject(std::shared_ptr<eng::Object> newObject);
  void removeObject(int objId);
  int updateObject(int objId, int frame);
  int updateAllObjects(int frame);

  bool doesExist(int objId);

  // Accessor methods
  std::shared_ptr<eng::Object> getObject(int objId);
  std::shared_ptr<eng::Sprite> getSprite(int objId);

  // Collision detection
  bool collide(int idA, int idB);

  // Debugging and utility methods
  int getNumObjects();

  // Update Frame Size
  void updateFrameSize(eng::Vector2i newSize);

  // Destructor
  ~ObjectManager();
};
