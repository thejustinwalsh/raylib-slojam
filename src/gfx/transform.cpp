#include "gfx/transform.h"

namespace gfx {

TransformModule::TransformModule(flecs::world& ecs) {
    ecs.module<TransformModule>();

    // Update position based on direction and speed
    ecs.system<Position, const Direction, const Speed>("Transform")
        .term_at(2).optional()
        .kind(flecs::OnUpdate)
        .each([&](Position& pos, Direction const& dir, Speed const& speed) {
            pos.value.x += dir.value.x * speed.value * ecs.delta_time();
            pos.value.y += dir.value.y * speed.value * ecs.delta_time();
        });

    // Update rotation based on rotator
    ecs.system<Rotation, const Rotator>("Rotate")
        .kind(flecs::OnUpdate)
        .each([&](Rotation& rot, Rotator const& delta) {
            rot.value += delta.value * ecs.delta_time();
        });

    // Update position based on translator
    ecs.system<Position, const Translator>("Translate")
        .kind(flecs::OnUpdate)
        .each([&](Position& pos, Translator const& delta) {
            pos.value.x += delta.value.x * ecs.delta_time();
            pos.value.y += delta.value.y * ecs.delta_time();
        });
}

} // namespace gfx
