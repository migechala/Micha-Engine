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
  type::Object* obj = objects[objId];
  if (obj->rising && obj->position.y >= (window->getSize().y - obj->size.y -
                                         obj->velocity.y)) {  // on upperbound
    obj->position.y = window->getSize().y - obj->size.y;
    obj->velocity.y = 0;
  } else if (obj->position.y <= 0 - obj->velocity.y) {  // on the ground
    obj->grounded = true;
    obj->position.y = 0;
    if (obj->acceleration.y > 0) {
      obj->velocity.y = 0;
      obj->position.y = 1;
      obj->grounded = false;
    }
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
  if (obj->isSprite()) {
    reinterpret_cast<type::Sprite*>(obj)->updateTexture();
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

type::Sprite* ObjectManager::getSprite(int objId) {
  auto obj = objects[objId];
  if (obj->isSprite()) {
    return reinterpret_cast<type::Sprite*>(obj);
  }
  LOG_ERR("ATTEMPTED TO CALL getSprite WITH NON-SPRITE OBJECT")
  return nullptr;
}

int ObjectManager::getNumObjects() { return objects.size(); }

ObjectManager::~ObjectManager() { delete instance; }
