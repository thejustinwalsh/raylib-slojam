#include "core/resource.h"

#include <filesystem>
#include <raylib-cpp.hpp>

namespace core {

ResourceModule::ResourceModule(flecs::world& ecs) {
    ecs.module<core::ResourceModule>();
    
    const std::filesystem::path appDir = GetApplicationDirectory();

    ecs.observer<ResourceResolver>("ResolveResource")
        .event(flecs::OnSet)
        .each([appDir](flecs::entity e, ResourceResolver const& path) {
            if (!path.value.empty()) {
                std::string resource = (appDir / std::string("resources") / path.value);
                if (std::filesystem::exists(resource)) {
                    e.set<Resource>({ resource });
                }
            }
        });

    ecs.observer<ResourceResolver>("ClearResourcePath")
        .event(flecs::OnRemove)
        .each([appDir](flecs::entity e, ResourceResolver const& path) {
            std::string resource = (appDir / std::string("resources") / path.value);
            e.remove<Resource>();
        });
}

} // namespace core
