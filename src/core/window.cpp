#include "core/window.h"

#include <vector>
#include <ranges>
#include <algorithm>

namespace core {

WindowModule::WindowModule(flecs::world& ecs) {
    std::vector Phases = {
        ecs.entity<RenderPhases::Begin>(),
        ecs.entity<RenderPhases::Background>(),
        ecs.entity<RenderPhases::Draw>(),
        ecs.entity<RenderPhases::UI>(),
        ecs.entity<RenderPhases::End>()
    };

    flecs::entity_t PriorPhase = flecs::OnStore;
    for (auto& Phase : Phases) {
        Phase.add(flecs::Phase).depends_on(PriorPhase);
        PriorPhase = Phase;
    }

    ecs.observer<Window, WindowSize>("AddWindow")
        .event(flecs::OnAdd)
        .each([](Window& window, const WindowSize& size) {
            window.handle = std::make_unique<raylib::Window>(size.dimension.x, size.dimension.y);
        });

    ecs.observer<Window>("RemoveWindow")
        .event(flecs::OnRemove)
        .each([](Window& window) {
            window.handle.reset();
        });

    ecs.observer<Window, const WindowTitle>("UpdateWindowTitle")
        .event(flecs::OnSet)
        .each([](Window& window, const WindowTitle& title) {
            window.handle->SetTitle(title.Title.c_str());
        });

    ecs.observer<Window, WindowSize>("UpdateWindowSize")
        .event(flecs::OnSet)
        .each([](Window& window, const WindowSize& size) {
            window.handle->SetSize(size.dimension.x, size.dimension.y);
        });

    ecs.observer<Window, WindowFPS>("UpdateWindowTargetFPS")
        .event(flecs::OnSet)
        .each([](Window& window, const WindowFPS& fps) {
#ifndef PLATFORM_WEB
            window.handle->SetTargetFPS(fps.Target);
#endif
        });
    
    auto begin = ecs.system<Window>("BeginDrawing")
        .kind<RenderPhases::Begin>()
        .each([](Window& window) {
            window.handle->BeginDrawing();
            window.handle->ClearBackground(raylib::Color::White());
        });

    auto end = ecs.system<Window>("EndDrawing")
        .kind<RenderPhases::End>()
        .each([&](Window& window) {
            window.handle->EndDrawing();
#ifndef PLATFORM_WEB
            if (window.handle->ShouldClose()) {
                window.handle->Close();
                ecs.quit();
            }
#endif
        });
}

} // namespace core
