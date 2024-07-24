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

std::shared_ptr<type::Object> create_object();
//
std::shared_ptr<type::Object> create_object(
    std::vector<std::shared_ptr<SDL_Texture>> textures,
    std::vector<int> numberOfSpritesPerTexture, type::Vector2i spriteSize);