#pragma once

#include <memory>
#include <vector>

#include "Types.h"
#include "WindowManager.h"

class ObjectManager {
 private:
  std::vector<std::shared_ptr<eng::Object>> objects;
  std::vector<int> freeObjectLoc;
  static ObjectManager* instance;
  static bool drawDebug;

 public:
  // Frame size used for position calculations
  const eng::Vector2i frameSize = {1920, 1080};
  // Constructor
  ObjectManager();

  // Singleton instance retrieval
  static ObjectManager* getInstance();

  // Object management methods
  int addObject(std::shared_ptr<eng::Object> newObject);
  void removeObject(int objId);
  int updateObject(int objId, int frame);
  int updateAllObjects(int frame);

  // Accessor methods
  std::shared_ptr<eng::Object> getObject(int objId);
  std::shared_ptr<eng::Sprite> getSprite(int objId);

  // Collision detection
  bool collide(int idA, int idB);

  // Debugging and utility methods
  int getNumObjects();
  void setDebug(bool dbg);

  // Destructor
  ~ObjectManager();
};
