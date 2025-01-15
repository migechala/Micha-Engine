#include "ObjectManager.h"

#include "Logger.h"
ObjectManager* ObjectManager::instance = nullptr;
bool ObjectManager::drawDebug = false;

ObjectManager::ObjectManager() {}

ObjectManager* ObjectManager::getInstance() {
  if (instance == nullptr) {
    instance = new ObjectManager();
  }
  return instance;
}

int ObjectManager::addObject(std::shared_ptr<eng::Object> newObject) {
  if (frameSize.x == 0 || frameSize.y == 0) {
    LOG_ERR("Frame size not set")
    return -1;
  }
  int newId = 0;
  if (freeObjectLoc.empty()) {
    newId = objects.size();
  } else {
    newId = freeObjectLoc.back();
    freeObjectLoc.pop_back();
  }
  LOG_INFO("Adding new object with id: " + std::to_string(newId),
           LOG_LEVEL::MEDIUM);
  objects.emplace_back(newObject);
  return newId;
}

void ObjectManager::removeObject(int objId) {
  objects[objId] = nullptr;
  freeObjectLoc.emplace_back(objId);
  LOG_INFO(std::to_string(freeObjectLoc.size()), LOG_LEVEL::PRIORITY);
}

int ObjectManager::updateObject(int objId, int frame) {
  std::shared_ptr<eng::Object> obj = objects[objId];
  if (!obj) {
    return objId;
  }
  if (obj->isRising() &&
      obj->getPosition().y >= (frameSize.y - obj->getSize().y -
                               obj->getVelocity().y)) {  // on upperbound
    obj->setPosition({obj->getPosition().x, frameSize.y - obj->getSize().y});
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
  if (obj->isSprite()) {
    std::reinterpret_pointer_cast<eng::Sprite>(obj)->updateTexture(frame);
  }
  return objId;
}

int ObjectManager::updateAllObjects(int frame) {
  for (int i = 0; i < objects.size(); ++i) {
    updateObject(i, frame);
  }
  return objects.size();
}

std::shared_ptr<eng::Object> ObjectManager::getObject(int objId) {
  return objects[objId];
}

std::shared_ptr<eng::Sprite> ObjectManager::getSprite(int objId) {
  auto obj = objects[objId];
  if (obj && obj->isSprite()) {
    return std::reinterpret_pointer_cast<eng::Sprite>(obj);
  }
  LOG_ERR("ATTEMPTED TO CALL getSprite WITH NON-SPRITE OBJECT")
  return nullptr;
}

bool ObjectManager::collide(int idA, int idB) {
  if (!objects[idB]) return false;
  auto objectA = getObject(idA);
  auto objectB = getObject(idB);
  int leftA, leftB;
  int rightA, rightB;
  int topA, topB;
  int bottomA, bottomB;

  // Calculate A
  leftA = objectA->getPosition().x - objectA->getSize().x / 2;
  rightA = leftA + objectA->getSize().x / 2;
  topA = objectA->getPosition().y + objectA->getSize().y / 2;
  bottomA = objectA->getPosition().y - objectA->getSize().y / 2;

  // Calculate B
  leftB = objectB->getPosition().x - objectB->getSize().x / 2;
  rightB = leftB + objectB->getSize().x / 2;
  topB = objectB->getPosition().y + objectB->getSize().y / 2;
  bottomB = objectB->getPosition().y - objectB->getSize().y / 2;

  if (bottomA >= topB) return false;
  if (topA <= bottomB) return false;
  if (rightA <= leftB) return false;
  if (leftA >= rightB) return false;
  return true;
}

int ObjectManager::getNumObjects() { return objects.size(); }

void ObjectManager::setDebug(bool dbg) { drawDebug = dbg; }

void ObjectManager::updateFrameSize(eng::Vector2i newSize) {
  frameSize = newSize;
  for (int i = 0; i < objects.size(); ++i) {
    auto obj = objects[i];
    if (obj->getPosition().y >= frameSize.y) {
      obj->setPosition({obj->getPosition().x, frameSize.y - obj->getSize().y});
    }
  }
}

ObjectManager::~ObjectManager() { delete instance; }
