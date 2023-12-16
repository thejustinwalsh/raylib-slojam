#pragma once

#include <flecs.h>
#include "game/scene.h"

namespace scene {

struct GameSceneModule {
    GameSceneModule(flecs::world& ecs);
};

} // namespace scene
