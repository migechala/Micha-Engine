#include <vector>
#include <iostream>
#include "KeyboardManager.h"
#include "Logger.h"

KeyboardManager *KeyboardManager::instance = nullptr;
bool KeyboardManager::listen = true;

const Uint8 *KeyboardManager::state = SDL_GetKeyboardState(NULL);
KeyboardManager *KeyboardManager::getInstance()
{

    if (instance == nullptr)
    {
        instance = new KeyboardManager();
    }
    return instance;
}
void KeyboardManager::addListener(SDL_Scancode key, std::function<void()> func)
{
    listeners.insert({key, std::make_pair(func, false)});
}

bool KeyboardManager::isPressed(SDL_Scancode key)
{
    return state[key];
}

void KeyboardManager::onListners()
{
    listen = true;
}

void KeyboardManager::printListner()
{
    LOG_INFO("____________________________");
    for (auto &i : listeners)
    {
        LOG_INFO("| " + std::string(SDL_GetKeyName(SDL_GetKeyFromScancode(i.first))) + " | func | " + std::to_string(i.second.second) + " |");
    }
    LOG_INFO("----------------------------");
}

void KeyboardManager::offListners()
{
    listen = false;
}

const Uint8 *KeyboardManager::getKeyboardStatus()
{
    return state;
}

void KeyboardManager::removeListener(SDL_Scancode key)
{
    listeners.erase(key);
}

void KeyboardManager::update()
{
    if (!listen)
    {
        return;
    }
    for (auto &i : listeners)
    {
        /*
         * i->first is the scancode key
         * i->second will return the std::pair of the function that will be ran and the lock state
         * i->second->first is the function that will be ran
         * i->second->second is the lock state of the key
         */
        if (state[i.first] && !i.second.second)
        {
            i.second.first();
            i.second.second = true; // lock
        }
        else if (!state[i.first] && i.second.second)
        {
            i.second.second = false; // unlock
        }
    }
}
