#pragma once

#include <raylib-cpp.hpp>

namespace gfx {

    struct Position {
        raylib::Vector2 value = {0, 0};;
    };

    struct Rotation {
        float value = 0;
    };

    struct Scale {
        raylib::Vector2 value = {1, 1};
    };

    struct Origin {
        raylib::Vector2 value = {0, 0};
    };

    namespace Origins {
        static raylib::Vector2 TopLeft = {0, 0};
        static raylib::Vector2 TopCenter = {0.5, 0};
        static raylib::Vector2 TopRight = {1, 0};
        static raylib::Vector2 CenterLeft = {0, 0.5};
        static raylib::Vector2 Center = {0.5, 0.5};
        static raylib::Vector2 CenterRight = {1, 0.5};
        static raylib::Vector2 BottomLeft = {0, 1};
        static raylib::Vector2 BottomCenter = {0.5, 1};
        static raylib::Vector2 BottomRight = {1, 1};
    }

} // namespace gfx
