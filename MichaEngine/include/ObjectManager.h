#pragma once
#include <vector>

#include "Types.h"
class ObjectManager {
  std::vector<type::Object*> objects;
  static ObjectManager* instance;

 public:
  ObjectManager();
  static ObjectManager* getInstance();
  int addObject(type::Object* newObject);
  void removeObject(type::Object* rmObject);
  void removeObject(int objId);
  int updateObject(int objId);
  int updateAllObjects();
  type::Object* getObject(int objId);
  int getNumObjects();

  ~ObjectManager();
};