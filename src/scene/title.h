#pragma once

#include <flecs.h>
#include "game/scene.h"

namespace scene {

struct TitleSceneModule {
    TitleSceneModule(flecs::world& ecs);
};

} // namespace scene
