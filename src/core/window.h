#pragma once

#include <memory>
#include <string>
#include <flecs.h>
#include <raylib-cpp.hpp>

namespace core {

struct Window {
    std::unique_ptr<raylib::Window> handle;
};

struct WindowSize {
    raylib::Vector2 dimension = { 0, 0 };
};

struct WindowTitle{
    std::string Title;
};

struct WindowFPS {
    int Target = 60;
};

namespace RenderPhases {
    struct Begin {};
    struct Background {};
    struct Draw {};
    struct UI {};
    struct End {};
    struct Post {};
};

/*
 * WindowModule
 * Handles the application window lifecycle.
*/
struct WindowModule {
    WindowModule(flecs::world& ecs);
};

} // namespace core
