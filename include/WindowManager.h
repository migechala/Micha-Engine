//
// Created by mikhail on 5/20/21.
//

#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H
#include <SDL2/SDL.h>
#include <string>
#include <memory>
#include "Objects.h"
#include "Types.h"
struct SDLDeleter
{
    void operator()(SDL_Window *w) const
    {
        SDL_DestroyWindow(w);
    }
    void operator()(SDL_Renderer *w) const
    {
        SDL_DestroyRenderer(w);
    }
    void operator()(SDL_Texture *w) const
    {
        SDL_DestroyTexture(w);
    }
};

class WindowManager
{
    std::unique_ptr<SDL_Window, SDLDeleter> m_window;
    std::unique_ptr<SDL_Renderer, SDLDeleter> m_renderer;
    SDL_Texture *m_bkg{};

    SDL_Event m_event{};
    type::Vector2i m_size;

    bool hasQuit();

    static type::Vector2i getMonitorSize();

public:
    
    int frameCount;
    
    SDL_Renderer *getRenderer();

    type::Vector2i getSize();

    void setBackground(SDL_Texture *background);

    int draw(SDL_Texture *txt, const SDL_Rect *src, const SDL_Rect *dst);
    int draw(ObjectBase* obj);

    bool update();

    WindowManager(const std::string &windowName, type::Vector2i p_pos, Uint32 p_flag);

    ~WindowManager();
};

#endif //BOXING2D_2_WINDOWMANAGER_H
