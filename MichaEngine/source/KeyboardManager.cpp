#include "KeyboardManager.h"

#include <iostream>
#include <vector>

#include "Logger.h"

KeyboardManager *KeyboardManager::instance = nullptr;
bool KeyboardManager::listen = true;

const Uint8 *KeyboardManager::state = SDL_GetKeyboardState(NULL);

KeyboardManager *KeyboardManager::getInstance() {
  if (instance == nullptr) {
    instance = new KeyboardManager();
  }
  return instance;
}

void KeyboardManager::addListener(std::vector<SDL_Scancode> keys, std::function<void()> func, bool repeatable) {
  for (auto key : keys) {
    listeners.insert({key, std::make_tuple(func, false, repeatable)});
    LOG_INFO(std::string("Adding listener for key: ").append(SDL_GetScancodeName(key)), LOG_LEVEL::MEDIUM);
    if (repeatable) {
      LOG_INFO(std::string("Successfully added listener for key: ").append(SDL_GetScancodeName(key)),
               LOG_LEVEL::MEDIUM);
      continue;
    }
    LOG_INFO(std::string("Successfully added listener for key: ").append(SDL_GetScancodeName(key)), LOG_LEVEL::MEDIUM);
  }
}

bool KeyboardManager::isPressed(SDL_Scancode key) { return state[key]; }

void KeyboardManager::onListeners() { listen = true; }

void KeyboardManager::printListener() {
  LOG_INFO("____________________________", LOG_LEVEL::LOW);
  for (auto &i : listeners) {
    LOG_INFO("| " + std::string(SDL_GetKeyName(SDL_GetKeyFromScancode(i.first))) + " | func | " +
                 std::to_string(std::get<1>(i.second)) + " |",
             LOG_LEVEL::LOW);
  }
  LOG_INFO("----------------------------", LOG_LEVEL::LOW);
}

void KeyboardManager::offListeners() { listen = false; }

const Uint8 *KeyboardManager::getKeyboardStatus() { return state; }

void KeyboardManager::removeListener(SDL_Scancode key) { listeners.erase(key); }

void KeyboardManager::update() {
  if (!listen) {
    return;
  }
  for (auto &i : listeners) {
    /*
     * i->first is the scancode key
     * i->second will return the std::pair of the function that will be ran and
     * the lock state i->second->first is the function that will be ran
     * i->second->second is the lock state of the key
     */
    if (state[i.first] && (!std::get<1>(i.second) || std::get<2>(i.second))) {
      std::get<0>(i.second)();
      std::get<1>(i.second) = true; // lock
    } else if (!state[i.first] && std::get<1>(i.second)) {
      std::get<1>(i.second) = false; // unlock
    }
  }
}
KeyboardManager::~KeyboardManager() {
  delete state;
  delete instance;
}
