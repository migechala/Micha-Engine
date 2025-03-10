#pragma once
#include <string>
#include <vector>

#include "KeyboardManager.h"
#include "Logger.h"
#include "MichaApp.h"
#include "ObjectManager.h"
#include "ResourceLoader.h"
#include "Types.h"
#include "WindowManager.h"
void start(MichaApp *execute);

//
int create_sprite(eng::SpriteOptions &options);
