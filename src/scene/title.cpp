#include "scene/title.h"

#include "core/input.h"
#include "core/resource.h"
#include "core/window.h"
#include "gfx/sprite.h"
#include "gfx/transform.h"
#include "scene/types.h"

// TODO: Get the screen size from the main window component?
constexpr int SCREEN_WIDTH = 960;
constexpr int SCREEN_HEIGHT = 450;

namespace scene {

TitleSceneModule::TitleSceneModule(flecs::world& ecs) {
    ecs.module<TitleSceneModule>();
    ecs.import<game::SceneModule>();

    struct Continue {};

    ecs.observer<game::ActiveScene>("ActivateTitleScene")
        .event(flecs::OnAdd)
        .second<TitleScene>()
        .each([&](game::ActiveScene) {
            flecs::entity scene = ecs.entity<game::SceneRoot>();

            ecs.defer_begin();
            ecs.delete_with(flecs::ChildOf, scene);
            ecs.defer_end();

            ecs.entity<TitleScene>()
                .add(flecs::Phase)
                .depends_on(flecs::OnUpdate);

            ecs.entity("Logo")
                .is_a(gfx::Sprite)
                .set<core::ResourceResolver>({"raylib_logo.png"})
                .set<gfx::Position>({{SCREEN_WIDTH/2, SCREEN_HEIGHT/2}})
                .child_of(scene);

            ecs.entity("ContinueButton")
                .add<Continue>()
                .add<core::Input>()
                .child_of(scene);
        });

    ecs.system<Continue>("TitleSceneContinue")
        .kind<TitleScene>()
        .each([&](Continue) {
            auto input = ecs.get<core::Input>();
            if (input->mouse.left.pressed) {
                ecs.defer_begin();
                ecs.add<game::ActiveScene, scene::GameScene>();
                ecs.defer_end();
            }
        });
}
    
} // namespace scene
