#pragma once
#include <vector>
#include <SDL.h>
#include "Logger.h"
#include "Objects.h"
#include "Types.h"
#include "WindowManager.h"

class Game{
    WindowManager* p_windowManager;
    
    SDL_Color colors[9];

    std::vector<Tile*> p_tiles;
    type::Vector2i p_size;
    Tile* p_placeAbleTile;

    int p_speed;

    SDL_Keycode p_place;

    void setPlaceable();

    void m_destoryTile(int loc);
    void m_addTile(int offsetX);

    SDL_Color RGBtoColor(std::string rgb);
    SDL_Color generateRandomColor();

    bool checkDone();
public:
    Game(WindowManager* windowManager);
    int update();
};