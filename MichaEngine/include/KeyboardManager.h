#pragma once
#include <SDL2/SDL.h>

#include <functional>
#include <map>

class KeyboardManager {
  static const Uint8 *state;
  std::multimap<SDL_Scancode, std::tuple<std::function<void()>, bool, bool>>
      listeners;
  static KeyboardManager *instance;
  static bool listen;

 public:
  static KeyboardManager *getInstance();
  const Uint8 *getKeyboardStatus();
  void addListener(SDL_Scancode key, std::function<void()> func,
                   bool repeatable = false);
  void removeListener(SDL_Scancode key);
  void printListner();
  template <class... Args>
  bool isNotPressed(Args... keys);
  bool isPressed(SDL_Scancode key);
  void update();
  void onListners();
  void offListners();
  ~KeyboardManager();
};

template <class... Args>
inline bool KeyboardManager::isNotPressed(Args... keys) {
  return ((... && !isPressed(keys)));
}
