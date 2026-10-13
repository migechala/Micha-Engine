#include "ObjectManager.h"

#include <cstdlib>
#include <iostream>

#include "Logger.h"

bool ObjectManager::isValidId(int objId) const {
  return objId >= 0 && objId < static_cast<int>(sprites.size());
}

int ObjectManager::top(int id) {
  if (!isValidId(id) || !sprites[id]) {
    LOG_ERR("id does not exist");
    return 0;
  };

  return sprites[id]->getOptions().getPosition().y + sprites[id]->getOptions().getHitbox().y +
         sprites[id]->getOptions().getHitboxOffset().y;
}

int ObjectManager::bottom(int id) {
  if (!isValidId(id) || !sprites[id]) {
    LOG_ERR("id does not exist");
    return 0;
  };

  return sprites[id]->getOptions().getPosition().y + sprites[id]->getOptions().getHitboxOffset().y;
}

int ObjectManager::left(int id) {
  if (!isValidId(id) || !sprites[id]) {
    LOG_ERR("id does not exist");
    return 0;
  };
  return sprites[id]->getOptions().getPosition().x - sprites[id]->getOptions().getHitbox().x / 2 +
         sprites[id]->getOptions().getHitboxOffset().x;
}

int ObjectManager::right(int id) {
  if (!isValidId(id) || !sprites[id]) {
    LOG_ERR("id does not exist");
    return 0;
  };
  return sprites[id]->getOptions().getPosition().x + sprites[id]->getOptions().getHitbox().x / 2 +
         sprites[id]->getOptions().getHitboxOffset().x;
}

bool ObjectManager::hasSolidGroundBelow(int objId) {
  if (!doesExist(objId)) {
    return false;
  }

  for (int otherId = 0; otherId < static_cast<int>(sprites.size()); ++otherId) {
    if (otherId == objId || !doesExist(otherId) || !sprites[otherId]->getOptions().isSolid()) {
      continue;
    }

    const bool horizontallyOverlaps = right(objId) > left(otherId) && left(objId) < right(otherId);
    const bool standingOnTop = std::abs(bottom(objId) - top(otherId)) <= 1;
    if (horizontallyOverlaps && standingOnTop) {
      return true;
    }
  }
  return false;
}

void ObjectManager::resolveSolidCollisions(int objId, eng::Vector2<float> attemptedVelocity) {
  auto sprite = getSprite(objId);
  if (!sprite) {
    return;
  }

  for (int otherId = 0; otherId < static_cast<int>(sprites.size()); ++otherId) {
    if (otherId == objId || !doesExist(otherId)) {
      continue;
    }

    auto other = sprites[otherId];
    if (!other->getOptions().isSolid() || !collide(objId, otherId)) {
      continue;
    }

    auto position = sprite->getOptions().getPosition();
    auto velocity = sprite->getOptions().getVelocity();

    if (attemptedVelocity.x > 0.0f) {
      position.x = left(otherId) - sprite->getOptions().getHitbox().x / 2 - sprite->getOptions().getHitboxOffset().x;
      velocity.x = 0.0f;
    } else if (attemptedVelocity.x < 0.0f) {
      position.x = right(otherId) + sprite->getOptions().getHitbox().x / 2 - sprite->getOptions().getHitboxOffset().x;
      velocity.x = 0.0f;
    }

    if (attemptedVelocity.y > 0.0f) {
      position.y = bottom(otherId) - sprite->getOptions().getHitbox().y - sprite->getOptions().getHitboxOffset().y;
      velocity.y = 0.0f;
    } else if (attemptedVelocity.y < 0.0f) {
      position.y = top(otherId) - sprite->getOptions().getHitboxOffset().y;
      velocity.y = 0.0f;
      sprite->setGrounded(true);
    }

    sprite->setPosition(position);
    sprite->setVelocity(velocity);
  }
}

ObjectManager::ObjectManager() : gravityStrength(0.7f) {}

ObjectManager *ObjectManager::getInstance() {
  static ObjectManager instance;
  return &instance;
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
  newSprite->setId(newId);
  if (newId == static_cast<int>(sprites.size())) {
    sprites.emplace_back(newSprite);
  } else {
    sprites[newId] = newSprite;
  }
  return newId;
}

void ObjectManager::removeSprite(int objId) {
  if (!isValidId(objId) || !sprites[objId]) {
    return;
  }
  sprites[objId] = nullptr;
  freeObjectLoc.emplace_back(objId);
}

int ObjectManager::updateSprite(int objId, int frame) {
  if (!isValidId(objId)) {
    return objId;
  }
  std::shared_ptr<eng::Sprite> sprite = sprites[objId];
  if (!sprite) {
    return objId;
  }

  if (sprite->getOptions().isStatic()) {
    sprite->updateTexture(frame);
    return objId;
  }

  auto velocity = sprite->getOptions().getVelocity();
  bool wasGrounded = sprite->isGrounded();
  sprite->setGrounded(false);

  sprite->setPosition({sprite->getOptions().getPosition().x + static_cast<int>(velocity.x),
                       sprite->getOptions().getPosition().y});
  resolveSolidCollisions(objId, {velocity.x, 0.0f});

  velocity = sprite->getOptions().getVelocity();
  sprite->setPosition({sprite->getOptions().getPosition().x,
                       sprite->getOptions().getPosition().y + static_cast<int>(velocity.y)});
  resolveSolidCollisions(objId, {0.0f, velocity.y});

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
  }
  if (!sprite->isGrounded() && wasGrounded && sprite->getOptions().getVelocity().y <= 0.0f && hasSolidGroundBelow(objId)) {
    sprite->setGrounded(true);
  }

  if (sprite->getOptions().isGravityEnabled()) {
    if (sprite->isGrounded()) {
      sprite->setAcceleration({sprite->getOptions().getAcceleration().x, 0.0f});
      sprite->setVelocity({sprite->getOptions().getVelocity().x, 0.0f});
    } else {
      sprite->setAcceleration({sprite->getOptions().getAcceleration().x, -gravityStrength});
    }
  }

  sprite->setVelocity(sprite->getOptions().getAcceleration() + sprite->getOptions().getVelocity());
  if (sprite->isGrounded()) {
    sprite->setVelocity({sprite->getOptions().getVelocity().x, 0.0f});
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

bool ObjectManager::doesExist(int objId) { return isValidId(objId) && sprites[objId] != nullptr; }

std::shared_ptr<eng::Sprite> ObjectManager::getSprite(int objId) {
  if (!doesExist(objId)) {
    return nullptr;
  }
  return sprites[objId];
}

bool ObjectManager::collide(int idA, int idB) {
  if (!doesExist(idA) || !doesExist(idB)) {
    return false;
  }
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

void ObjectManager::updateFrameSize(eng::Vector2<int> newSize) {
  frameSize = newSize;
  for (int i = 0; i < sprites.size(); ++i) {
    auto sprite = sprites[i];
    if (!sprite) {
      continue;
    }
    if (sprite->getOptions().getPosition().y >= frameSize.y) {
      sprite->setPosition({sprite->getOptions().getPosition().x, frameSize.y - sprite->getOptions().getSize().y});
    }
  }
}
