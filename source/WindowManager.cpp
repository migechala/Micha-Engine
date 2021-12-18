//
// Created by mikhail on 5/20/21.
//

#include "WindowManager.h"
#include "Logger.h"
#include "Types.h"
#include <SDL2/SDL.h>
#include <iostream>

WindowManager::WindowManager(const std::string &windowName, type::Vector2i p_pos, Uint32 p_flag) : m_size(
        getMonitorSize()), frameCount(0)
{
    SDL_Log("Created Window");
    m_window.reset(SDL_CreateWindow(windowName.c_str(), p_pos.x, p_pos.y, m_size.x, m_size.y, p_flag));
    m_renderer.reset(SDL_CreateRenderer(m_window.get(), -1, SDL_RENDERER_ACCELERATED));

    if (m_window == nullptr)
    {
        SDL_Log("Error! Window is null");
    }
}

type::Vector2i WindowManager::getSize()
{
    return m_size;
}

bool WindowManager::hasQuit()
{
    while (SDL_PollEvent(&m_event))
    {
        if (m_event.type == SDL_QUIT)
        {

            return true;
        }
        if (m_event.type == SDL_KEYDOWN)
        {
            if (m_event.key.keysym.sym == SDLK_ESCAPE)
            {
                return true;
            }
        }
    }
    return false;
}

int WindowManager::draw(SDL_Texture *txt, const SDL_Rect *src, const SDL_Rect *dst)
{
    return SDL_RenderCopy(getRenderer(), txt, src, dst);
}
int WindowManager::draw(ObjectBase* obj){
    SDL_Color oldColor; 
    SDL_GetRenderDrawColor(getRenderer(), &oldColor.r, &oldColor.g, &oldColor.b, &oldColor.a);
    SDL_SetRenderDrawColor(getRenderer(), obj->color.r, obj->color.g, obj->color.b, obj->color.a);
    int ret = SDL_RenderFillRect(getRenderer(), &obj->dst);
    SDL_SetRenderDrawColor(getRenderer(), oldColor.r, oldColor.g, oldColor.b, oldColor.a);
    return ret;
}

bool WindowManager::update()
{
    SDL_RenderPresent(getRenderer());
    if (draw(m_bkg, nullptr, nullptr) != 0)
    {
        LOG_ERR(SDL_GetError());
    }
    ++frameCount;
    return hasQuit();
}

type::Vector2i WindowManager::getMonitorSize()
{
    SDL_DisplayMode t_dm;
    SDL_GetCurrentDisplayMode(0, &t_dm);
    return {t_dm.w, t_dm.h};
}

void WindowManager::setBackground(SDL_Texture *background)
{
    m_bkg = background;
}

SDL_Renderer *WindowManager::getRenderer()
{
    return m_renderer.get();
}
WindowManager::~WindowManager()
{
    SDL_DestroyTexture(m_bkg);
}

