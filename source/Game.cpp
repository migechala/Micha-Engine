#include <SDL.h>
#include <regex>
#include <random>
#include "Game.h"
#include "Objects.h"
#include "Types.h"
#include "WindowManager.h"
#include "KeyboardManager.h"

Game::Game(WindowManager* windowManager): p_windowManager(windowManager), p_place(SDL_GetKeyFromScancode(SDL_SCANCODE_SPACE)), p_size(1000, 100), p_speed(10){
    //020129,197278,edddd4,339BA8,772e25,954155,954155,9AABB5,010847,1065A2
    colors[0] = RGBtoColor("283D3B");
    colors[1] = RGBtoColor("197278");
    colors[2] = RGBtoColor("EDDDD4");
    colors[3] = RGBtoColor("339BA8");
    colors[4] = RGBtoColor("772E25");
    colors[5] = RGBtoColor("954155");
    colors[6] = RGBtoColor("9AABB5");
    colors[7] = RGBtoColor("010847");
    colors[8] = RGBtoColor("1065A2");
    p_tiles.emplace_back(new Tile(type::Vector2i(windowManager->getSize().x / 2 - p_size.x/2, windowManager->getSize().y - p_size.y), p_size, generateRandomColor()));
    KeyboardManager::getInstance()->addListener(SDL_SCANCODE_SPACE);
    setPlaceable();
}

SDL_Color Game::RGBtoColor(std::string rgb){
    std::regex pattern("#?([0-9a-fA-F]{2})([0-9a-fA-F]{2})([0-9a-fA-F]{2})");

    std::smatch match;
    if (std::regex_match(rgb, match, pattern))
    {
        unsigned char r = std::stoul(match[1].str(), nullptr, 16);
        unsigned char g = std::stoul(match[2].str(), nullptr, 16);
        unsigned char b = std::stoul(match[3].str(), nullptr, 16);
        unsigned char a = 255;
        return {r, g, b, a};
    }
    return {0,0,0,0};
}

void Game::setPlaceable(){
    p_placeAbleTile = 
        new Tile(type::Vector2i(p_tiles[p_tiles.size()-1]->dst.x, p_tiles[p_tiles.size()-1]->dst.y),
                            type::Vector2i(p_tiles[p_tiles.size()-1]->dst.w, p_tiles[p_tiles.size()-1]->dst.h),
                            generateRandomColor());

    p_placeAbleTile->dst.y = p_windowManager->getSize().y - p_placeAbleTile->dst.h * (p_tiles.size() + 1);
}

void Game::m_destoryTile(int loc){
    Tile* temp = new Tile(type::Vector2i(p_tiles[p_tiles.size()-1]->dst.x, p_windowManager->getSize().y - p_tiles[p_tiles.size()-1]->dst.h), type::Vector2i(p_tiles[p_tiles.size()-1]->dst.w, p_tiles[p_tiles.size()-1]->dst.h), p_tiles[p_tiles.size()-1]->color);
    p_tiles.clear();
    p_tiles.emplace_back(temp);
    setPlaceable();
}

SDL_Color Game::generateRandomColor(){
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> rand(0, sizeof colors/sizeof colors    [0] - 1);
    static int lastColor[] = {0, 1};
    int colorNum = rand(rng);
    while (colorNum == lastColor[0] || colorNum == lastColor[1]){
        colorNum = rand(rng);
    }
    lastColor[1] = lastColor[0];
    lastColor[0] = colorNum;
    return colors[colorNum];
}

void Game::m_addTile(int offsetX){
    if(offsetX <= 0){
        p_tiles.emplace_back(new Tile(type::Vector2i(p_placeAbleTile->dst.x, p_placeAbleTile->dst.y), 
            type::Vector2i(p_placeAbleTile->dst.w + offsetX, p_placeAbleTile->dst.h),
            p_placeAbleTile->color
            )
        );
    }
    else{
        p_tiles.emplace_back(new Tile(type::Vector2i(p_placeAbleTile->dst.x + offsetX, p_placeAbleTile->dst.y), 
            type::Vector2i(p_placeAbleTile->dst.w - offsetX, p_placeAbleTile->dst.h),
            p_placeAbleTile->color
            )
        );
    }
    setPlaceable();
}

bool Game::checkDone(){
    if(p_placeAbleTile->dst.w <= 0){
        return true;
    }
    return false;
}

int Game::update(){
    if((p_placeAbleTile->dst.x + p_placeAbleTile->dst.w + p_speed >= p_windowManager->getSize().x) || (p_placeAbleTile->dst.x + p_speed < 0)){
        p_speed *= -1;
    }
    p_placeAbleTile->dst.x += p_speed;
    for(auto i : p_tiles){
        if(p_windowManager->draw(i) != 0){
            return -1;
        }
    }
    bool isPressed = KeyboardManager::getInstance()->isPressed().find(SDL_SCANCODE_SPACE)->second;

    static bool pressed = false;
    if(!pressed && isPressed){
        m_addTile(p_tiles[p_tiles.size()-1]->dst.x - p_placeAbleTile->dst.x);
    }
    if(isPressed){
        pressed = true;
    }
    else{
        pressed = false;
    }
    if(p_tiles.size() == 9){ 
        m_destoryTile(0);
    }
    p_windowManager->draw(p_placeAbleTile);
    return checkDone();
}