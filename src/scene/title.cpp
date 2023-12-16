#include "scene/title.h"
#include "scene/types.h"
#include "core/resource.h"
#include "gfx/sprite.h"
#include "gfx/transform.h"


// TODO: Get the screen size from the main window component?
constexpr int SCREEN_WIDTH = 960;
constexpr int SCREEN_HEIGHT = 450;

namespace scene {

TitleSceneModule::TitleSceneModule(flecs::world& ecs) {
    ecs.module<TitleSceneModule>();
    ecs.import<game::SceneModule>();

    flecs::entity title = ecs.pipeline()
        .with(flecs::System)
        .without<GameScene>()
        .without<RestartScene>()
        .build();

    ecs.set<TitleScene>({ title });

    ecs.observer<game::ActiveScene>("ActivateTitleScene")
        .event(flecs::OnAdd)
        .second<TitleScene>()
        .each([&](game::ActiveScene) {
            flecs::entity scene = ecs.component<game::SceneRoot>();

            ecs.defer_begin();
            ecs.delete_with(flecs::ChildOf, scene);
            ecs.defer_end();

            struct Logo {};
            ecs.entity<Logo>()
                .is_a(gfx::Sprite)
                .set<core::ResourcePath>({"raylib_logo.png"})
                .set<gfx::Position>({{SCREEN_WIDTH/2, SCREEN_HEIGHT/2}})
                .child_of(scene);

            // TODO: Create all entities for the scene as children of the scene root

            ecs.set_pipeline(ecs.get<TitleScene>()->pipeline);
        });
}
    
} // namespace scene
