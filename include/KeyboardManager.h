#ifndef KEYBOARD_MANAGER_H
#define KEYBOARD_MANAGER_H
#include <SDL2/SDL.h>
#include <map>
class KeyboardManager{
    static const Uint8 *state;
    std::vector<SDL_Scancode> listeners;
    static KeyboardManager* instance;
public:
    static KeyboardManager* getInstance();
    const Uint8 *getKeyboardStatus();
    void addListener(SDL_Scancode key);
    std::map<SDL_Scancode, bool> isPressed();
};

#endif