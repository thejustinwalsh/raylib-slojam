#include <memory>
#include <iostream>
#include <flecs.h>
#include "core/resource.h"
#include "core/window.h"
#include "gfx/sprite.h"
#include "gfx/transform.h"

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

constexpr int SCREEN_WIDTH = 960;
constexpr int SCREEN_HEIGHT = 450;

//----------------------------------------------------------------------------------
// Module Functions Definition
//----------------------------------------------------------------------------------
void UpdateDrawFrame(void)
{
    // Update
    //----------------------------------------------------------------------------------
    // TODO: Update your variables here
    //----------------------------------------------------------------------------------

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();

        ClearBackground(RAYWHITE);

        //GuiLabel({ 0, 0, 100, 20 }, "Hello World!");

        //DrawTexture(logo, screenWidth / 2 - logo.width / 2, screenHeight / 2 - logo.height / 2, WHITE);

    EndDrawing();
    //----------------------------------------------------------------------------------
}

int main()
{
    flecs::world ecs;

    ecs_os_init();

#if !defined(PLATFORM_WEB) && !defined(NDEBUG)
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
    ecs.import<gfx::SpriteModule>();

    struct MainWindow{};
    ecs.singleton<MainWindow>()
        .set<core::WindowSize>({{SCREEN_WIDTH, SCREEN_HEIGHT}})
        .set<core::WindowTitle>({"Raylib Slo-Jam 2023"})
        .set<core::WindowFPS>({60})
        .add<core::Window>();


    // TODO: Add the game/title, game/loop, and game/over modules and enable/disable them as needed.

    struct Logo {};
    ecs.entity<Logo>()
        .is_a(gfx::Sprite)
        .set<core::ResourcePath>({"raylib_logo.png"})
        .set<gfx::Position>({{SCREEN_WIDTH/2, SCREEN_HEIGHT/2}});
    
    #if defined(PLATFORM_WEB)
        emscripten_set_main_loop_arg([](void* ctx) { ecs_progress((ecs_world_t*)ctx, 0); }, ecs, 0, true);
    #endif

    while (ecs.progress()) { }

    ecs_os_fini();

    return 0;
}
