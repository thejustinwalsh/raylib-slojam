#include "gfx/texture.h"

#include <filesystem>
#include <memory>
#include <unordered_map>
#include "core/resource.h"

namespace gfx {

using namespace core;

std::unordered_map<std::string, std::weak_ptr<raylib::Texture>> textureCache;

TextureModule::TextureModule(flecs::world& ecs) {
    ecs.import<core::ResourceModule>();

    // Load the texture if it has not been loaded yet.
    // Hand out the shared_ptr to the existing texture if it has been loaded.
    ecs.observer<Texture, const Resource>("AddTexture")
        .event(flecs::OnSet)
        .each([](Texture& texture, Resource const& res) {
            if (!res.value.empty() && IsWindowReady()) {
                if (textureCache.find(res.value) != textureCache.end() && !textureCache[res.value].expired()) {
                    texture.handle = textureCache[res.value].lock();
                    return;
                }

                texture.handle = std::make_shared<raylib::Texture>(res.value);
                textureCache.insert({res.value, texture.handle});
            }
        });
    
    // Release the shared_ptr for this handle, and allow RAII to clean up the texture.
    // We are not removing the key from the cache, it may be used again through the lifetime of the application.
    ecs.observer<Texture>("RemoveTexture")
        .event(flecs::OnRemove)
        .each([](Texture& texture) {
            texture.handle.reset();
        });
}

} // namespace gfx
