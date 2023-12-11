#include "gfx/sprite.h"

#include <raylib-cpp.hpp>
#include "core/resource.h"
#include "core/window.h"
#include "gfx/texture.h"
#include "gfx/transform.h"

namespace gfx {

using namespace core;

flecs::entity Sprite;

SpriteModule::SpriteModule(flecs::world& ecs) {
    ecs.import<core::ResourceModule>();
    ecs.import<core::WindowModule>();
    ecs.import<gfx::TextureModule>();

    Sprite = ecs.prefab("Sprite")
        .add<SpriteRendering>()
        .add<ResourcePath>()
        .add<Texture>()
        .set<Position>({{0, 0}})
        .set<Rotation>({0})
        .set<Scale>({{1, 1}})
        .set<Origin>({Origins::Center})
        .set<Tint>({{raylib::Color::White()}});

    ecs.system<SpriteRendering, Texture, Position, Rotation, Scale, Origin, Tint>("DrawSprite")
        .kind<core::RenderPhases::Draw>()
        .each([](SpriteRendering, 
            const Texture& texture, 
            const Position& pos, 
            const Rotation& rot, 
            const Scale& scale, 
            const Origin& origin,
            const Tint& tint) {
                DrawTexturePro(
                    texture.handle,
                    raylib::Rectangle(0, 0, texture.handle.width, texture.handle.height),
                    raylib::Rectangle(pos.value.x, pos.value.y, texture.handle.width * scale.value.x, texture.handle.height * scale.value.y),
                    {origin.value.x * texture.handle.width, origin.value.y * texture.handle.height},
                    rot.value,
                    tint.value
                );
            });
}

} // namespace gfx
