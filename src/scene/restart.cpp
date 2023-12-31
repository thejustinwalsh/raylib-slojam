#include "scene/restart.h"
#include "scene/types.h"

namespace scene {

RestartSceneModule::RestartSceneModule(flecs::world& ecs) {
    ecs.module<RestartSceneModule>();
    ecs.import<game::SceneModule>();

    ecs.observer<game::ActiveScene>("ActivateRestartScene")
        .event(flecs::OnAdd)
        .second<RestartScene>()
        .each([&](game::ActiveScene) {
            flecs::entity scene = ecs.component<game::SceneRoot>();

            ecs.defer_begin();
            ecs.delete_with(flecs::ChildOf, scene);
            ecs.defer_end();

            // TODO: Create all entities for the scene as children of the scene root
        });
}

} // namespace scene
