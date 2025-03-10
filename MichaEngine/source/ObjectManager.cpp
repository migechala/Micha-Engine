#include "ObjectManager.h"

#include <iostream>

#include "Logger.h"
ObjectManager *ObjectManager::instance = nullptr;

int ObjectManager::top(int id) {
  if (!sprites[id]) {
    LOG_ERR("id does not exist");
  };

  return sprites[id]->getOptions().getPosition().y + sprites[id]->getOptions().getHitbox().y +
         sprites[id]->getOptions().getHitboxOffset().y;
}

int ObjectManager::bottom(int id) {
  if (!sprites[id]) {
    LOG_ERR("id does not exist");
  };

  return sprites[id]->getOptions().getPosition().y + sprites[id]->getOptions().getHitboxOffset().y;
}

int ObjectManager::left(int id) {
  if (!sprites[id]) {
    LOG_ERR("id does not exist");
  };
  return sprites[id]->getOptions().getPosition().x - sprites[id]->getOptions().getHitbox().x / 2 +
         sprites[id]->getOptions().getHitboxOffset().x;
}

int ObjectManager::right(int id) {
  if (!sprites[id]) {
    LOG_ERR("id does not exist");
  };
  return sprites[id]->getOptions().getPosition().x + sprites[id]->getOptions().getHitbox().x / 2 +
         sprites[id]->getOptions().getHitboxOffset().x;
}

ObjectManager::ObjectManager() {}

ObjectManager *ObjectManager::getInstance() {
  if (instance == nullptr) {
    instance = new ObjectManager();
  }
  return instance;
}

int ObjectManager::addSprite(std::shared_ptr<eng::Sprite> newSprite) {
  if (frameSize.x == 0 || frameSize.y == 0) {
    LOG_ERR("Frame size not set")
    return -1;
  }
  int newId = 0;
  if (freeObjectLoc.empty()) {
    newId = sprites.size();
  } else {
    newId = freeObjectLoc.back();
    freeObjectLoc.pop_back();
  }
  LOG_INFO("Adding new object with id: " + std::to_string(newId), LOG_LEVEL::MEDIUM);
  sprites.emplace_back(newSprite);
  return newId;
}

void ObjectManager::removeSprite(int objId) {
  sprites[objId] = nullptr;
  freeObjectLoc.emplace_back(objId);
}

int ObjectManager::updateSprite(int objId, int frame) {
  std::shared_ptr<eng::Sprite> sprite = sprites[objId];
  if (!sprite) {
    return objId;
  }
  if (sprite->isRising() &&
      sprite->getOptions().getPosition().y >=
          (frameSize.y - sprite->getOptions().getSize().y - sprite->getOptions().getVelocity().y)) { // on upperbound
    sprite->setPosition({sprite->getOptions().getPosition().x, frameSize.y - sprite->getOptions().getSize().y});
    sprite->setVelocity({sprite->getOptions().getVelocity().x, 0});
  } else if (sprite->getOptions().getPosition().y <= 0 - sprite->getOptions().getVelocity().y) { // on the ground
    sprite->setGrounded(true);
    sprite->setPosition({sprite->getOptions().getPosition().x, 0});
    if (sprite->getOptions().getAcceleration().y > 0) {
      sprite->setVelocity({sprite->getOptions().getVelocity().x, 0});
      sprite->setPosition({sprite->getOptions().getPosition().x, 1});
      sprite->setGrounded(false);
    }
  } else {
    sprite->setPosition(sprite->getOptions().getVelocity() + sprite->getOptions().getPosition());
    sprite->setVelocity(sprite->getOptions().getAcceleration() + sprite->getOptions().getVelocity());
    if (sprite->getOptions().isGravityEnabled()) {
      sprite->setAcceleration({sprite->getOptions().getAcceleration().x, -1});
    }
  }
  if (sprite->isRising() && sprite->getOptions().getVelocity().y <= 0) {
    sprite->setRising(false);
    return objId;
  } else if (!sprite->isRising() && sprite->getOptions().getVelocity().y > 0) {
    sprite->setRising(true);
  }
  sprite->updateTexture(frame);
  return objId;
}

int ObjectManager::updateAllSprites(int frame) {
  for (int i = 0; i < sprites.size(); ++i) {
    updateSprite(i, frame);
  }
  return sprites.size();
}

bool ObjectManager::doesExist(int objId) { return sprites[objId] != nullptr; }

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

  if (bottomA >= topB)
    return false;
  if (topA <= bottomB)
    return false;
  if (rightA <= leftB)
    return false;
  if (leftA >= rightB)
    return false;
  return true;
}

int ObjectManager::getNumObjects() { return sprites.size(); }

void ObjectManager::updateFrameSize(eng::Vector2i newSize) {
  frameSize = newSize;
  for (int i = 0; i < sprites.size(); ++i) {
    auto sprite = sprites[i];
    if (sprite->getOptions().getPosition().y >= frameSize.y) {
      sprite->setPosition({sprite->getOptions().getPosition().x, frameSize.y - sprite->getOptions().getSize().y});
    }
  }
}

ObjectManager::~ObjectManager() { delete instance; }
