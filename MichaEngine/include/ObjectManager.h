#pragma once
#include <vector>

#include "Types.h"
#include "WindowManager.h"
class ObjectManager {
  std::vector<std::shared_ptr<type::Object>> objects;
  static ObjectManager* instance;

 public:
  ObjectManager();
  static ObjectManager* getInstance();
  std::shared_ptr<type::Object> addObject(type::Object* newObject);
  void removeObject(int objId);
  int updateObject(int objId, std::shared_ptr<WindowManager> window);
  int updateAllObjects(std::shared_ptr<WindowManager> window);
  std::shared_ptr<type::Object> getObject(int objId);
  std::shared_ptr<type::Sprite> getSprite(int objId);
  int getNumObjects();

  ~ObjectManager();
};