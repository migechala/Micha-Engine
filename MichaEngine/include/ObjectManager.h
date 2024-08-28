#pragma once
#include <vector>

#include "Types.h"
#include "WindowManager.h"
class ObjectManager {
  std::vector<std::shared_ptr<eng::Object>> objects;
  static ObjectManager* instance;

 public:
  ObjectManager();
  static ObjectManager* getInstance();
  int addObject(std::shared_ptr<eng::Object> newObject);
  void removeObject(int objId);
  int updateObject(int objId, int frame, std::shared_ptr<WindowManager> window);
  int updateAllObjects(std::shared_ptr<WindowManager> window, int frame);
  std::shared_ptr<eng::Object> getObject(int objId);
  std::shared_ptr<eng::Sprite> getSprite(int objId);
  bool collide(int idA, int idB);
  int getNumObjects();

  ~ObjectManager();
};