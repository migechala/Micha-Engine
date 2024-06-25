#include "ObjectManager.h"

#include "Logger.h"
ObjectManager* ObjectManager::instance = nullptr;

ObjectManager::ObjectManager() {}

ObjectManager* ObjectManager::getInstance() {
  if (instance == nullptr) {
    instance = new ObjectManager();
  }
  return instance;
}

int ObjectManager::addObject(type::Object* newObject) {
  int newId = objects.size();
  LOG_INFO("Adding new object with id: " + std::to_string(newId),
           LOG_LEVEL::MEDIUM);
  objects.emplace_back(newObject);
  return newId;
}

void ObjectManager::removeObject(type::Object* rmObject) {}
void ObjectManager::removeObject(int objId) { objects[objId] = nullptr; }

int ObjectManager::updateObject(int objId) {
  objects[objId]->position.y += objects[objId]->velocity.y;
  objects[objId]->position.x += objects[objId]->velocity.x;
  if (objects[objId]->rising && objects[objId]->velocity.y <= 0) {
    objects[objId]->rising = false;
  } else if (!objects[objId]->rising && objects[objId]->velocity.y > 0) {
    objects[objId]->rising = true;
  }
  return objId;
}

int ObjectManager::updateAllObjects() {
  for (int i = 0; i < objects.size(); ++i) {
    updateObject(i);
  }
  return objects.size();
}

type::Object* ObjectManager::getObject(int objId) { return objects[objId]; }

int ObjectManager::getNumObjects() { return objects.size(); }

ObjectManager::~ObjectManager() { delete instance; }
