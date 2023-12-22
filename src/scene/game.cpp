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

    struct OnGameSceneUpdate {};
    struct OnGameSceneValidate {};
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

            ecs.entity<OnGameSceneUpdate>()
                .add(flecs::Phase)
                .depends_on(flecs::PreUpdate);

            ecs.entity<OnGameSceneValidate>()
                .add(flecs::Phase)
                .depends_on(flecs::OnValidate);
                            
            ecs.entity("Player")
                .is_a(gfx::Sprite)
                .add<Player>()
                .set<core::ResourceResolver>({"square.png"})
                .set<gfx::Position>({{(float)GetScreenWidth()/2, (float)GetScreenHeight()/2}})
                .set<gfx::Direction>({{0, 0}})
                .set<gfx::Speed>({200})
                .set<gfx::Tint>({{raylib::Color::Red()}})
                .child_of(scene);
        });
    
    // Move the player and spawn bullets
    ecs.system<Player, gfx::Direction, gfx::Speed, const gfx::Position>("UpdatePlayer")
        .kind<OnGameSceneUpdate>()
        .each([&](Player, gfx::Direction& dir, gfx::Speed& speed, gfx::Position const& pos) {
            auto input = ecs.get<core::Input>();

            // Update direction
            dir.value = raylib::Vector2::Zero();
            if (input->keys[KEY_RIGHT].current) dir.value.x += 1;
            else if (input->keys[KEY_LEFT].current) dir.value.x -= 1;
            if (input->keys[KEY_UP].current) dir.value.y -= 1;
            else if (input->keys[KEY_DOWN].current) dir.value.y += 1;
            dir.value = dir.value.Normalize();
            
            // TODO: Speed change via analog input, accumulate speed with keyboard input
            speed.value = 200;

            // Fire a bullet
            if (input->keys[KEY_SPACE].current && dir.value.Length() > 0) {
                ecs.entity()
                    .is_a(gfx::Sprite)
                    .add<Bullets>()
                    .set<core::ResourceResolver>({"square.png"})
                    .set<gfx::Position>({pos.value})
                    .set<gfx::Direction>({dir.value})
                    .set<gfx::Speed>({500})
                    .set<gfx::Scale>({{0.5f, 0.5f}})
                    .set<gfx::Tint>({{raylib::Color::Black()}})
                    .child_of(ecs.entity<game::SceneRoot>());
            }
        });

    // Destroy bullets when they leave the screen
    ecs.system<Bullets, const gfx::Position>("DestroyOffScreenBullets")
        .kind<OnGameSceneValidate>()
        .each([&](flecs::entity e, Bullets, gfx::Position const& pos) {
            if (pos.value.x < 0 || pos.value.x > GetScreenWidth() ||
                pos.value.y < 0 || pos.value.y > GetScreenHeight()) {
                ecs.defer_begin();
                ecs.delete_with(e);
                ecs.defer_end();
            }
        });

}

} // namespace scene
