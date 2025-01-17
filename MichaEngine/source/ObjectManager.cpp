#include "ObjectManager.h"

#include <iostream>

#include "Logger.h"
ObjectManager* ObjectManager::instance = nullptr;

int ObjectManager::top(int id) {
  if (!objects[id]) {
    LOG_ERR("id does not exist");
  };

  return objects[id]->getPosition().y + objects[id]->getHitbox().y +
         objects[id]->getHitboxOffset().y;
}

int ObjectManager::bottom(int id) {
  if (!objects[id]) {
    LOG_ERR("id does not exist");
  };

  return objects[id]->getPosition().y + objects[id]->getHitboxOffset().y;
}

int ObjectManager::left(int id) {
  if (!objects[id]) {
    LOG_ERR("id does not exist");
  };
  return objects[id]->getPosition().x - objects[id]->getHitbox().x / 2 +
         objects[id]->getHitboxOffset().x;
}

int ObjectManager::right(int id) {
  if (!objects[id]) {
    LOG_ERR("id does not exist");
  };
  return objects[id]->getPosition().x + objects[id]->getHitbox().x / 2 +
         objects[id]->getHitboxOffset().x;
}

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

bool ObjectManager::doesExist(int objId) { return objects[objId] != nullptr; }

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
  int leftA, leftB;
  int rightA, rightB;
  int topA, topB;
  int bottomA, bottomB;

  // Calculate A
  leftA = left(idA);
  rightA = right(idA);
  topA = top(idA);
  bottomA = bottom(idA);

  // Calculate B
  leftB = left(idB);
  rightB = right(idB);
  topB = top(idB);
  bottomB = bottom(idB);

  if (bottomA >= topB) return false;
  if (topA <= bottomB) return false;
  if (rightA <= leftB) return false;
  if (leftA >= rightB) return false;
  return true;
}

int ObjectManager::getNumObjects() { return objects.size(); }

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
