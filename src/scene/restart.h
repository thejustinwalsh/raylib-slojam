#pragma once

#include <flecs.h>
#include "game/scene.h"

namespace scene {

struct RestartSceneModule {
    RestartSceneModule(flecs::world& ecs);
};

} // namespace scene
