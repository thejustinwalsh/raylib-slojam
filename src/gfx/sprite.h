#pragma once

#include <string>
#include <flecs.h>
#include <raylib-cpp.hpp>

namespace gfx {

/// Sprite Prefab
extern flecs::entity Sprite;

/// Tint component to multiply the color of a sprite
struct Tint {
    raylib::Color value = {0, 0, 0, 255};
};

/// Tag component for sprites that should be rendered using the SpriteModule
struct SpriteRendering {};

/*
 * SpriteModule
 * Handles rendering simple sprites
 */
struct SpriteModule {
    SpriteModule(flecs::world& ecs);
};

} // namespace gfx
