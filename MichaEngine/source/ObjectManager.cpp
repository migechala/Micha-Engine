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

std::shared_ptr<type::Object> ObjectManager::addObject(
    type::Object* newObject) {
  int newId = objects.size();

  LOG_INFO("Adding new object with id: " + std::to_string(newId),
           LOG_LEVEL::PRIORITY);
  std::shared_ptr<type::Object> obj(newObject);
  obj->setId(newId);
  objects.emplace_back(obj);
  return obj;
}

void ObjectManager::removeObject(int objId) { objects[objId] = nullptr; }

int ObjectManager::updateObject(int objId,
                                std::shared_ptr<WindowManager> window) {
  std::shared_ptr<type::Object> obj = objects[objId];
  if (!obj) return objId;
  if (obj->isRising() &&
      obj->getPosition().y >= (window->getSize().y - obj->getSize().y -
                               obj->getVelocity().y)) {  // on upperbound
    obj->setPosition(
        {obj->getPosition().x, window->getSize().y - obj->getSize().y});
    obj->setVelocity({obj->getVelocity().x, 0});
  } else if (obj->getPosition().y <=
             0 - obj->getVelocity().y) {  // on the ground
    obj->setGrounded(true);
    obj->setPosition({obj->getPosition().x, 0});
    if (obj->getAcceleration().y > 0) {
      obj->setVelocity({obj->getVelocity().x, 0});
      obj->setPosition({obj->getPosition().x, 1});
      obj->setGrounded(false);
    }
  } else {
    obj->setPosition(obj->getVelocity() + obj->getPosition());
    obj->setVelocity(obj->getAcceleration() + obj->getVelocity());
    if (obj->hasGravity()) {
      obj->setAcceleration({obj->getAcceleration().x, -1});
    }
  }
  if (obj->isRising() && obj->getVelocity().y <= 0) {
    obj->setRising(false);
    return objId;
  } else if (!obj->isRising() && obj->getVelocity().y > 0) {
    obj->setRising(true);
  }
  // if (obj->position.x - obj->size.x / 2 <= 0) {  // left bound
  //   obj->position.x = obj->size.x / 2;
  // } else if (obj->position.x + obj->size.x / 2 >=
  //            window->getSize().x) {  // right bound
  //   obj->position.x = window->getSize().x - obj->size.x / 2;
  // }
  if (obj->isSprite()) {
    std::reinterpret_pointer_cast<type::Sprite>(obj)->updateTexture();
  }
  return objId;
}

int ObjectManager::updateAllObjects(std::shared_ptr<WindowManager> window) {
  for (int i = 0; i < objects.size(); ++i) {
    updateObject(i, window);
  }
  return objects.size();
}

std::shared_ptr<type::Object> ObjectManager::getObject(int objId) {
  return objects[objId];
}

std::shared_ptr<type::Sprite> ObjectManager::getSprite(int objId) {
  auto obj = objects[objId];
  if (obj && obj->isSprite()) {
    return std::reinterpret_pointer_cast<type::Sprite>(obj);
  }
  LOG_ERR("ATTEMPTED TO CALL getSprite WITH NON-SPRITE OBJECT")
  return nullptr;
}

int ObjectManager::getNumObjects() { return objects.size(); }

ObjectManager::~ObjectManager() { delete instance; }
