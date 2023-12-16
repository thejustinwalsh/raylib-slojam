#include "game/scene.h"

namespace game {

SceneModule::SceneModule(flecs::world& ecs) {
    ecs.module<SceneModule>();

    ecs.component<ActiveScene>()
        .add(flecs::Exclusive);
}

} // namespace game
