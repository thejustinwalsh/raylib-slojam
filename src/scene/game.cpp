#include "scene/game.h"
#include "scene/types.h"
#include "gfx/sprite.h"
#include "gfx/transform.h"
#include "core/resource.h"
#include "core/input.h"

#include <raylib-cpp.hpp>

namespace scene {

GameSceneModule::GameSceneModule(flecs::world& ecs) {
    ecs.module<GameSceneModule>();
    ecs.import<game::SceneModule>();

    struct Player {};
    struct Bullets {};

    ecs.observer<game::ActiveScene>("ActivateGameScene")
        .event(flecs::OnAdd)
        .second<GameScene>()
        .each([&](game::ActiveScene) {
            flecs::entity scene = ecs.entity<game::SceneRoot>();

            ecs.defer_begin();
            ecs.delete_with(flecs::ChildOf, scene);
            ecs.defer_end();

            ecs.entity<GameScene>()
                .add(flecs::Phase)
                .depends_on(flecs::OnUpdate);

            ecs.entity("Player")
                .is_a(gfx::Sprite)
                .add<Player>()
                .set<core::ResourceResolver>({"square.png"})
                .set<gfx::Position>({{(float)GetScreenWidth()/2, (float)GetScreenHeight()/2}})
                .set<gfx::Tint>({{raylib::Color::Red()}})
                .child_of(scene);
        });
    
    ecs.system<Player, gfx::Position>("MovePlayer")
        .kind<GameScene>()
        .each([&](Player, gfx::Position& pos) {
            auto input = ecs.get<core::Input>();

            auto dir = raylib::Vector2::Zero();
            if (input->keys[KEY_RIGHT].current) dir.x += 1;
            if (input->keys[KEY_LEFT].current) dir.x -= 1;
            if (input->keys[KEY_UP].current) dir.y -= 1;
            if (input->keys[KEY_DOWN].current) dir.y += 1;

            pos.value += dir.Normalize() * ecs.delta_time() * 200;

            if(input->keys[KEY_SPACE].pressed) {
                ecs.entity()
                    .is_a(gfx::Sprite)
                    .add<Bullets>()
                    .set<core::ResourceResolver>({"square.png"})
                    .set<gfx::Position>({pos.value})
                    .set<gfx::Scale>({{0.5f, 0.5f}})
                    .set<gfx::Tint>({{raylib::Color::Black()}})
                    .child_of(ecs.entity<game::SceneRoot>());
            }
        });
}

} // namespace scene
