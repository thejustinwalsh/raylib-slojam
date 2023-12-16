#pragma once

#include <flecs.h>

namespace game {

 /// Represents the current scene
struct ActiveScene {};

/// Parent for all entities unique to the scene
struct SceneRoot {};

 /// Pipeline for all game scene
using ScenePipeline = flecs::entity;

/// Handles transitioning the game through scene
struct SceneModule {
    SceneModule(flecs::world& ecs);
};

}
