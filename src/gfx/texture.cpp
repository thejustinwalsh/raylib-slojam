#include "gfx/texture.h"

#include <filesystem>
#include "core/resource.h"

namespace gfx {

using namespace core;

TextureModule::TextureModule(flecs::world& ecs) {
    ecs.import<core::ResourceModule>();

    ecs.observer<Texture, ResourcePath>("AddTexture")
        .event(flecs::OnSet)
        .each([](Texture& texture, const ResourcePath& path) {
            if (!path.value.empty() && IsWindowReady()) {
                texture.handle = raylib::Texture2D(path.value);
            }
        });
    
    ecs.observer<Texture>("RemoveTexture")
        .event(flecs::OnRemove)
        .each([](Texture& texture) {
            texture.handle.Unload();
        });
}

} // namespace gfx
