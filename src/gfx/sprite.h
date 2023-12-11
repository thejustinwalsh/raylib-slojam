#pragma once

#include <string>
#include <flecs.h>
#include <raylib-cpp.hpp>

namespace gfx {

extern flecs::entity Sprite;

struct Tint {
    raylib::Color value = {0, 0, 0, 255};
};

struct SpriteRendering {};

/*
 * SpriteModule
 * Handles rendering simple sprites
*/
struct SpriteModule {
    SpriteModule(flecs::world& ecs);
};

} // namespace gfx
