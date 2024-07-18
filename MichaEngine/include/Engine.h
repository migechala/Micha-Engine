#pragma once
#include <string>
#include <vector>

#include "Executable.h"
#include "KeyboardManager.h"
#include "Logger.h"
#include "ObjectManager.h"
#include "ResourceLoader.h"
#include "Types.h"
#include "WindowManager.h"
void start(ExecutableClass* execute);
void start(std::unique_ptr<ExecutableClass> execute);

int create_sprite(type::Vector2i position, type::Vector2i size,
                  type::Vector2i velocity = {0, 0},
                  type::Vector2i acceleration = {0, 0},
                  SDL_Color color = {0, 0, 0, 0});
int create_sprite(std::vector<SDL_Texture*> textures, type::Vector2i position,
                  type::Vector2i spriteSize, type::Vector2i size,
                  type::Vector2i velocity = {0, 0},
                  type::Vector2i acceleration = {0, 0});