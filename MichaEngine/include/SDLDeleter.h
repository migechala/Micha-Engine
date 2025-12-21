#pragma once
#include <SDL2/SDL_system.h>
#include <SDL2/SDL_ttf.h>
struct SDLDeleter {
  void operator()(SDL_Window *w) const { SDL_DestroyWindow(w); }
  void operator()(SDL_Renderer *w) const { SDL_DestroyRenderer(w); }
  void operator()(SDL_Texture *w) const { SDL_DestroyTexture(w); }
  void operator()(TTF_Font *w) const { TTF_CloseFont(w); }
};