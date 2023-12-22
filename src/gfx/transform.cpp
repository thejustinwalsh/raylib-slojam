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
}

} // namespace gfx
