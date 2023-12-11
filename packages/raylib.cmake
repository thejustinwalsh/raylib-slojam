#raylib
find_package(raylib QUIET)
if (NOT raylib_FOUND)
    FetchContent_Declare(
        raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG 5.0
    )
    FetchContent_MakeAvailable(raylib)
endif()

# raylib-cpp
find_package(raylib_cpp QUIET)
if (NOT raylib_cpp_FOUND)
    FetchContent_Declare(
        raylib_cpp
        GIT_REPOSITORY https://github.com/RobLoach/raylib-cpp.git
        GIT_TAG v5.0.0
    )
    FetchContent_MakeAvailable(raylib_cpp)
endif()

# raygui
find_package(raygui QUIET)
if (NOT raygui_FOUND)
    FetchContent_Declare(
        raygui
        GIT_REPOSITORY https://github.com/raysan5/raygui.git
        GIT_TAG 4.0
    )
    FetchContent_MakeAvailable(raygui)
endif()

# reasings
find_package(reasings QUIET)
if (NOT reasings_FOUND)
    FetchContent_Declare(
        reasings
        GIT_REPOSITORY https://github.com/raylib-extras/reasings.git
        GIT_TAG 1cc22b1adec3a4146f609e90505f1d4e696b4d7a
    )
    FetchContent_MakeAvailable(reasings)
endif()
