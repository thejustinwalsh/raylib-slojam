#pragma once

#include <string>
#include <memory>
#include <flecs.h>
#include <raylib-cpp.hpp>

namespace gfx {

struct Texture {
    std::shared_ptr<raylib::Texture2D> handle;
};

/*
 * TextureModule
 * Handles loading and unloading textures
*/
struct TextureModule {
    TextureModule(flecs::world& ecs);
};

} // namespace gfx
