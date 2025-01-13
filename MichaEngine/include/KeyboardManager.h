#pragma once

#include <SDL2/SDL.h>

#include <functional>
#include <map>

class KeyboardManager {
 private:
  static const Uint8* state;  // SDL keyboard state
  std::multimap<SDL_Scancode, std::tuple<std::function<void()>, bool, bool>>
      listeners;                     // Registered listeners
  static KeyboardManager* instance;  // Singleton instance
  static bool listen;                // Flag to control listener status

 public:
  // Singleton pattern to get the instance of KeyboardManager
  static KeyboardManager* getInstance();

  // Retrieves the current state of the keyboard
  const Uint8* getKeyboardStatus();

  // Adds a listener for a key press event with an optional repeatable flag
  void addListener(SDL_Scancode key, std::function<void()> func,
                   bool repeatable = false);

  // Removes the listener for a specific key
  void removeListener(SDL_Scancode key);

  // Prints out the list of listeners (for debugging)
  void printListener();

  // Template function to check if multiple keys are not pressed
  template <class... Args>
  bool isNotPressed(Args... keys);

  // Checks if a specific key is pressed
  bool isPressed(SDL_Scancode key);

  // Updates the state of the listeners (should be called regularly)
  void update();

  // Turns on the listeners
  void onListeners();

  // Turns off the listeners
  void offListeners();

  // Destructor
  ~KeyboardManager();
};

// Template function to check if none of the specified keys are pressed
template <class... Args>
inline bool KeyboardManager::isNotPressed(Args... keys) {
  return ((... && !isPressed(keys)));  // Fold expression to check all keys
}
