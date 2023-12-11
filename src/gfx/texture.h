#pragma once

#include <string>
#include <flecs.h>
#include <raylib-cpp.hpp>

namespace gfx {

struct Texture {
    raylib::Texture2D handle;
};

/*
 * TextureModule
 * Handles loading and unloading textures
*/
struct TextureModule {
    TextureModule(flecs::world& ecs);
};

} // namespace gfx
