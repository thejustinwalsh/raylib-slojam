#include "collision/collider.h"

#include <flecs.h>

namespace collision {

ColliderModule::ColliderModule(flecs::world& ecs) {
    ecs.module<ColliderModule>();
    ecs.component<CollidesWith>().add(flecs::Tag);
}

} // namespace collision
