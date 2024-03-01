//
// Created by Mikhail Chalakov on 2/18/24.
//
#include "WindowManager.h"
#include <string>

bool WindowManager::hasQuit() {
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

type::Vector2i WindowManager::getMonitorSize() {
    SDL_DisplayMode t_dm;
    SDL_GetCurrentDisplayMode(0, &t_dm);
    int *ret;
    return {t_dm.w, t_dm.h};
}

int WindowManager::draw(SDL_Texture *txt, const SDL_Rect *src, const SDL_Rect *dst) {
    return SDL_RenderCopy(getRenderer(), txt, src, dst);
}

type::Vector2i WindowManager::getAbsolutePosition(type::Vector2i position)
{
    int *h;
    SDL_GetWindowSize(m_window.get(), h, nullptr);
    return {position.x,*h-position.y};
}
int WindowManager::draw(Object *object)
{
    SDL_Color oldColor;
    type::Vector2i pos = getAbsolutePosition(object->position);
    object->dst.x = pos.x;
    object->dst.y = pos.y;
    int ret = 0;
    if (object->texture == nullptr)
    {
        SDL_GetRenderDrawColor(getRenderer(), &oldColor.r, &oldColor.g, &oldColor.b, &oldColor.a);
        SDL_SetRenderDrawColor(getRenderer(), object->color.r, object->color.g, object->color.b, object->color.a);
        ret = SDL_RenderFillRect(getRenderer(), &object->dst);
        SDL_SetRenderDrawColor(getRenderer(), oldColor.r, oldColor.g, oldColor.b, oldColor.a);
    }
    else
    {
        SDL_RenderCopy(getRenderer(), object->texture, &object->src, &object->dst);
    }
    return ret;
}

bool WindowManager::update()
{
    SDL_RenderPresent(getRenderer());
    if (draw(background, nullptr, nullptr) != 0)
    {
        SDL_Log("%s", SDL_GetError());
    }
    ++frameCount;
    return hasQuit();
}
void WindowManager::setBackground(SDL_Texture *bkg)
{
    background = bkg;
}

SDL_Renderer *WindowManager::getRenderer() {
    return m_renderer.get();
}

WindowManager::WindowManager(const std::string &windowName, type::Vector2i p_pos, Uint32 p_flag): frameCount(0)
{
    SDL_Log("Created Window");
    m_window.reset(SDL_CreateWindow(windowName.c_str(), p_pos.x, p_pos.y, getMonitorSize().x, getMonitorSize().y, p_flag));
    m_renderer.reset(SDL_CreateRenderer(m_window.get(), -1, SDL_RENDERER_ACCELERATED));

    if (m_window == nullptr)
    {
        SDL_Log("Error! Window is null");
    }
}

WindowManager::~WindowManager() {
    SDL_DestroyTexture(background);
}
