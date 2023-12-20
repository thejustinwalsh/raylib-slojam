#include <memory>
#include <iostream>
#include <flecs.h>

#include "core/input.h"
#include "core/resource.h"
#include "core/window.h"
#include "gfx/sprite.h"
#include "gfx/transform.h"
#include "game/scene.h"
#include "scene/types.h"
#include "scene/title.h"
#include "scene/game.h"
#include "scene/restart.h"

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

constexpr int SCREEN_WIDTH = 960;
constexpr int SCREEN_HEIGHT = 450;

int main()
{
    flecs::world ecs;

    ecs_os_init();

#if !defined(PLATFORM_WEB) && defined(DEBUG)
    ecs.import<flecs::monitor>();
    ecs.set<flecs::Rest>({});
#endif

    SetTraceLogCallback([](int logLevel, const char *text, va_list args) {
        static char r[24] = "#[green]raylib#[reset] ";
        static char log_msg[1024] = {0};
        strncpy(log_msg, r, 24);
        vsnprintf(log_msg + 23, 1024 - 23, text, args);

        switch (logLevel) {
            case LOG_INFO:
            default:
                flecs::log::trace(log_msg);
                break;
            case LOG_DEBUG:
                flecs::log::dbg(log_msg);
                break;
            case LOG_WARNING:
                flecs::log::warn(log_msg);
                break;
           case LOG_ERROR:
                flecs::log::err(log_msg);
                break;
        }
    });

    flecs::log::set_level(1);

    char *no_color = getenv("NO_COLOR");
    flecs::log::enable_colors(!(no_color != NULL && no_color[0] != '\0'));

    ecs.import<core::ResourceModule>();
    ecs.import<core::WindowModule>();
    ecs.import<core::InputModule>();
    ecs.import<gfx::SpriteModule>();
    ecs.import<game::SceneModule>();
    ecs.import<scene::TitleSceneModule>();
    ecs.import<scene::GameSceneModule>();
    ecs.import<scene::RestartSceneModule>();

    struct MainWindow{};
    ecs.singleton<MainWindow>()
        .set<core::WindowSize>({{SCREEN_WIDTH, SCREEN_HEIGHT}})
        .set<core::WindowTitle>({"Raylib Slo-Jam 2023"})
        .set<core::WindowFPS>({144})
        .add<core::Window>();

    // Init game title scene
    ecs.add<game::ActiveScene, scene::TitleScene>();
    
    #if defined(PLATFORM_WEB)
        emscripten_set_main_loop_arg([](void* ctx) { ecs_progress((ecs_world_t*)ctx, 0); }, ecs, 0, true);
    #else
        ecs.set_target_fps(144);
    #endif

    while (ecs.progress()) { }

    ecs_os_fini();

    return 0;
}
