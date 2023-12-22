#include "scene/game.h"
#include "scene/types.h"
#include "gfx/sprite.h"
#include "gfx/transform.h"
#include "core/resource.h"
#include "core/input.h"

#include <raylib-cpp.hpp>

namespace scene {

flecs::timer spawner;

GameSceneModule::GameSceneModule(flecs::world& ecs) {
    ecs.module<GameSceneModule>();
    ecs.import<game::SceneModule>();

    struct OnGameSceneUpdate {};
    struct OnGameSceneValidate {};
    
    struct Player {};
    struct Bullet {};
    struct Enemy {};

    struct FireDelay {
        float value = 0;
        float max = 0.1f;
    };

    spawner = ecs.timer()
        .interval(1)
        .rate(60);

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
                .set<FireDelay>({0.05, 0.05})
                .child_of(scene);
        });
    
    // Move the player and spawn bullets
    ecs.system<Player, FireDelay, gfx::Direction, gfx::Speed, const gfx::Position>("UpdatePlayer")
        .kind<OnGameSceneUpdate>()
        .each([&](Player, FireDelay& delay, gfx::Direction& dir, gfx::Speed& speed, gfx::Position const& pos) {
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
                delay.value += ecs.delta_time();
                if (delay.value >= delay.max) {
                    delay.value = 0;
                    ecs.entity()
                        .is_a(gfx::Sprite)
                        .add<Bullet>()
                        .set<core::ResourceResolver>({"square.png"})
                        .set<gfx::Position>({pos.value})
                        .set<gfx::Direction>({dir.value})
                        .set<gfx::Speed>({500})
                        .set<gfx::Scale>({{0.5f, 0.5f}})
                        .set<gfx::Tint>({{raylib::Color::Black()}})
                        .child_of(ecs.entity<game::SceneRoot>());
                }
            }

            if (input->keys[KEY_SPACE].released) {
                delay.value = delay.max;
            }
        });

    ecs.system("SpawnEnemy")
        .kind<OnGameSceneUpdate>()
        .tick_source(spawner)
        .iter([&](flecs::iter& it) {
            const raylib::Vector2 pos = {
                (float)GetRandomValue(0, GetScreenWidth()),
                (float)GetRandomValue(0, GetScreenHeight())
            };

            const float angle = GetRandomValue(0, 360);
            raylib::Vector2 dir = {
                cos(angle * DEG2RAD),
                sin(angle * DEG2RAD)
            };

            const float speed = GetRandomValue(50, 200);

            const float size = GetRandomValue(2, 6);

            ecs.entity()
                .is_a(gfx::Sprite)
                .add<Enemy>()
                .set<core::ResourceResolver>({"square.png"})
                .set<gfx::Position>({pos})
                .set<gfx::Direction>({dir})
                .set<gfx::Rotation>({0})
                .set<gfx::Rotator>({(float)GetRandomValue(-90, 90)})
                .set<gfx::Speed>({speed})
                .set<gfx::Scale>({{size, size}})
                .set<gfx::Tint>({{raylib::Color::Black()}})
                .child_of(ecs.entity<game::SceneRoot>());

            spawner.rate(GetRandomValue(60, 240));
        });

    // Destroy bullets and enemies when they leave the screen
    auto destroyOffScreen = [&](flecs::iter& it) {
        auto positions = it.field<const gfx::Position>(2);
        for (auto i : it) {
            flecs::entity e = it.entity(i);
            const gfx::Position& pos = positions[i];
            
            if (pos.value.x < 0 || pos.value.x > GetScreenWidth() ||
                pos.value.y < 0 || pos.value.y > GetScreenHeight()) {
                it.world().defer_begin();
                ecs_delete(it.world(), e);
                it.world().defer_end();
            }
        }
    };

    auto screenWrap = [&](flecs::iter& it) {
        auto positions = it.field<gfx::Position>(2);
        for (auto i : it) {
            flecs::entity e = it.entity(i);
            gfx::Position& pos = positions[i];
            
            if (pos.value.x < 0) pos.value.x += GetScreenWidth();
            if (pos.value.y < 0) pos.value.y += GetScreenHeight();
            pos.value.x = std::fmod(pos.value.x, GetScreenWidth());
            pos.value.y = std::fmod(pos.value.y, GetScreenHeight());
        }
    };

    ecs.system<Bullet, gfx::Position>("ScreenWrapBullets")
        .kind<OnGameSceneValidate>()
        .iter(screenWrap);

    ecs.system<Enemy, gfx::Position>("ScreenWrapEnemies")
        .kind<OnGameSceneValidate>()
        .iter(screenWrap);

    ecs.system<Player, gfx::Position>("ScreenWrapPlayer")
        .kind<OnGameSceneValidate>()
        .iter(screenWrap);

}

} // namespace scene
