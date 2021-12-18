#include <SDL2/SDL.h>
#include "Logger.h"
#include "ResourceLoader.h"
#include "WindowManager.h"
#include "KeyboardManager.h"
#include "Game.h"
#include "Objects.h"
#define CHECK(func, tar, var){ \
    if(func != tar){ \
        var = !var; \
        LOG_ERR(SDL_GetError()) \
    } \
}
int main(){
    SDL_Init(SDL_INIT_EVERYTHING);
    Logger::resetLogger();
    std::string logTest = std::to_string(100);
    if(logTest != "100"){
        LOG_ERR("To string returns wrong value, should be 100, returns " + logTest);
    }
    WindowManager* windowManager = new WindowManager("Jumper", type::Vector2i(SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED), SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN);
    windowManager->setBackground(ResourceLoader::getInstance()->loadTexture("../assets/background/bkg_a.jpg", windowManager->getRenderer()));
   
    Game* game = new Game(windowManager); 

    bool done = false;
    while(!done){
        Uint32 start = SDL_GetPerformanceCounter();        
        //Do loops
        CHECK(windowManager->update(), false, done);

        if(game->update()){
            done = true;
            // Game Over
        }
        //Limit fps
        Uint32 end = SDL_GetPerformanceCounter();
        float elapsedMS = (end - start) / (float)SDL_GetPerformanceFrequency() * 1000.0f;
        SDL_Delay(floor(16.666f - elapsedMS)); // 16 2/3 miliseconds in between frame AKA, cap at 60 fps
    }
    return 0;
}