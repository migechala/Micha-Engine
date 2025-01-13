#pragma once

#include <memory>
#include <vector>

#include "Types.h"
#include "WindowManager.h"

class ObjectManager {
 private:
  std::vector<std::shared_ptr<eng::Object>> objects;
  static ObjectManager* instance;
  static bool drawDebug;

 public:
  // Constructor
  ObjectManager();

  // Singleton instance retrieval
  static ObjectManager* getInstance();

  // Object management methods
  int addObject(std::shared_ptr<eng::Object> newObject);
  void removeObject(int objId);
  int updateObject(int objId, int frame, std::shared_ptr<WindowManager> window);
  int updateAllObjects(std::shared_ptr<WindowManager> window, int frame);

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
