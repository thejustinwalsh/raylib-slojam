#include "scene/game.h"
#include "scene/types.h"

namespace scene {

GameSceneModule::GameSceneModule(flecs::world& ecs) {
    ecs.module<GameSceneModule>();
    ecs.import<game::SceneModule>();

    flecs::entity game = ecs.pipeline()
        .with(flecs::System)
        .without<TitleScene>()
        .without<RestartScene>()
        .build();

    ecs.set<GameScene>({ game });

    ecs.observer<game::ActiveScene>("ActivateGameScene")
        .event(flecs::OnAdd)
        .second<GameScene>()
        .each([&](game::ActiveScene) {
            flecs::entity scene = ecs.component<game::SceneRoot>();

            ecs.defer_begin();
            ecs.delete_with(flecs::ChildOf, scene);
            ecs.defer_end();

             // TODO: Create all entities for the scene as children of the scene root

            ecs.set_pipeline(ecs.get<TitleScene>()->pipeline);
        });
}

} // namespace scene
