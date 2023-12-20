#include "core/input.h"
#include <config.h>
#include <raylib-cpp.hpp>

namespace core {

InputModule::InputModule(flecs::world& ecs) {
    ecs.module<InputModule>();

    ecs.singleton<Input>()
        .set<Input>({ 0 });

    ecs.system<Input>("SetInputState")
        .kind(flecs::PreFrame)
        .each([](flecs::entity e, Input& input) {
            for (int i = 0; i < MAX_KEYBOARD_KEYS; i++) {
                input.keys[i].pressed = IsKeyPressed(i);
                input.keys[i].released = IsKeyReleased(i);
                input.keys[i].current = IsKeyDown(i);
            }

            input.mouse.left.pressed = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
            input.mouse.left.released = IsMouseButtonReleased(MOUSE_LEFT_BUTTON);
            input.mouse.left.current = IsMouseButtonDown(MOUSE_LEFT_BUTTON);

            input.mouse.right.pressed = IsMouseButtonPressed(MOUSE_RIGHT_BUTTON);
            input.mouse.right.released = IsMouseButtonReleased(MOUSE_RIGHT_BUTTON);
            input.mouse.right.current = IsMouseButtonDown(MOUSE_RIGHT_BUTTON);

            input.mouse.middle.pressed = IsMouseButtonPressed(MOUSE_MIDDLE_BUTTON);
            input.mouse.middle.released = IsMouseButtonReleased(MOUSE_MIDDLE_BUTTON);
            input.mouse.middle.current = IsMouseButtonDown(MOUSE_MIDDLE_BUTTON);

            input.mouse.position = GetMousePosition();
            input.mouse.delta = GetMouseDelta();
            input.mouse.wheel = GetMouseWheelMoveV();
        });
}

} // namespace core
