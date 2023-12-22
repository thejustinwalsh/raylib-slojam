#pragma once

#include <string>
#include <flecs.h>

namespace core {

// TODO: Resource Resolver that is aware of resource type and applies the correct component
/// Resolves a relative resource path and applies a Resource component to the entity
struct ResourceResolver {
    std::string value;
};

/// Full path to the resource
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
