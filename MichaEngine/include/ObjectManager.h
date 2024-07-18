#pragma once
#include <vector>

#include "Types.h"
#include "WindowManager.h"
class ObjectManager {
  std::vector<type::Object*> objects;
  static ObjectManager* instance;

 public:
  ObjectManager();
  static ObjectManager* getInstance();
  int addObject(type::Object* newObject);
  void removeObject(type::Object* rmObject);
  void removeObject(int objId);
  int updateObject(int objId, WindowManager* window);
  int updateAllObjects(WindowManager* window);
  type::Object* getObject(int objId);
  type::Sprite* getSprite(int objId);
  int getNumObjects();

  ~ObjectManager();
};