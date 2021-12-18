#include <vector>
#include "KeyboardManager.h"
#include "Logger.h"

KeyboardManager* KeyboardManager::instance = nullptr;
const Uint8* KeyboardManager::state = SDL_GetKeyboardState(NULL);
KeyboardManager* KeyboardManager::getInstance(){
    
    if (instance == nullptr){
        instance = new KeyboardManager();
    } 
    return instance;
}

void KeyboardManager::addListener(SDL_Scancode key){
    listeners.emplace_back(key);
}

std::map<SDL_Scancode, bool> KeyboardManager::isPressed(){
    std::map<SDL_Scancode, bool> pressed;
    for(auto i : listeners){
        pressed.insert(std::pair<SDL_Scancode, bool>(i, state[i]));
    }
    return pressed;
}

const Uint8 *KeyboardManager::getKeyboardStatus(){
    return state;
}
