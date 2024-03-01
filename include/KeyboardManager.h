#pragma once
#include <SDL2/SDL.h>

#include <functional>
#include <map>

class KeyboardManager {
  static const Uint8 *state;
  std::multimap<SDL_Scancode, std::pair<std::function<void()>, bool>> listeners;
  static KeyboardManager *instance;
  static bool listen;

 public:
  static KeyboardManager *getInstance();
  const Uint8 *getKeyboardStatus();
  void addListener(SDL_Scancode key, std::function<void()> func);
  void removeListener(SDL_Scancode key);
  void printListner();

  bool isPressed(SDL_Scancode key);
  void update();
  void onListners();
  void offListners();
};