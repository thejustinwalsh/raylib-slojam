#pragma once

#include <string>
#include <flecs.h>

namespace core {

struct ResourceResolver {
    std::string value;
};

struct Resource {
    std::string value;
};

/*
 * ResourceModule
 * Handles resolving resource paths
*/
struct ResourceModule {
    ResourceModule(flecs::world& ecs);
};

}
