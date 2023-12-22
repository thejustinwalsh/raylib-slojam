#pragma once

#include <flecs.h>

namespace game {

/// Represents the current scene
struct ActiveScene {};

/// Parent for all entities unique to the scene
struct SceneRoot {};

/*
 * SceneModule
 * Handles transitioning the game through scenes
 */
struct SceneModule {
    SceneModule(flecs::world& ecs);
};

}
