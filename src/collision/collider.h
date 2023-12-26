#pragma once

#include <flecs.h>
#include <raylib-cpp.hpp>

namespace collision {

struct CollidesWith {};

struct SphereCollider {
    float radius;
};

struct BoxCollider {
    raylib::Vector2 extents;
};

struct ColliderModule {
    ColliderModule(flecs::world& ecs);
};

} // namespace collision
