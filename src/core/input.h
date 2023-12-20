#pragma once

#include <flecs.h>
#include <raylib-cpp.hpp>

namespace core {

struct KeyState {
    bool pressed = false;
    bool released = false;
    bool current = false;
};

struct MouseState {
    raylib::Vector2 position = {0, 0};
    raylib::Vector2 delta = {0, 0};
    raylib::Vector2 wheel = {0, 0};
    KeyState left;
    KeyState right;
    KeyState middle;
};

struct Input {
    KeyState keys[512];
    MouseState mouse;
};

struct InputModule {
    InputModule(flecs::world& ecs);
};

} // namespace core
