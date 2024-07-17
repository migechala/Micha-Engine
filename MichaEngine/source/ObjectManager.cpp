#include "ObjectManager.h"

#include "Logger.h"
#include "WindowManager.h"
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

int ObjectManager::updateObject(int objId, WindowManager* window) {
  auto obj = objects[objId];
  if (obj->rising && obj->position.y >= window->getSize().y) {
    obj->position.y = window->getSize().y;
    obj->velocity.y = 0;
    LOG_INFO(std::to_string(window->getSize().y), LOG_LEVEL::PRIORITY);
  } else if (obj->position.y <= 0) {
    obj->position.y = 0;
  } else {
    obj->position += obj->velocity;
    obj->velocity += obj->acceleration;
    obj->acceleration.y = -1;
  }
  if (obj->rising && obj->velocity.y <= 0) {
    obj->rising = false;
    return objId;
  } else if (!obj->rising && obj->velocity.y > 0) {
    obj->rising = true;
  }

  return objId;
}

int ObjectManager::updateAllObjects(WindowManager* window) {
  for (int i = 0; i < objects.size(); ++i) {
    updateObject(i, window);
  }
  return objects.size();
}

type::Object* ObjectManager::getObject(int objId) { return objects[objId]; }

int ObjectManager::getNumObjects() { return objects.size(); }

ObjectManager::~ObjectManager() { delete instance; }
