#include "core/resource.h"

#include <filesystem>
#include <raylib-cpp.hpp>

namespace core {

ResourceModule::ResourceModule(flecs::world& ecs) {
    const std::filesystem::path appDir = GetApplicationDirectory();

    ecs.observer<ResourcePath>("ResolveResourcePath")
        .event(flecs::OnSet)
        .each([appDir](ResourcePath& path) {
            if (!path.value.empty()) {
                path.value = (appDir / std::string("resources") / path.value);
            }
        });

    ecs.observer<ResourcePath>("ClearResourcePath")
        .event(flecs::OnRemove)
        .each([](ResourcePath& path) {
            path.value = "";
        });


    ecs.module<core::ResourceModule>();
}

} // namespace core
