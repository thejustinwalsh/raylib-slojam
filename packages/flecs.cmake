# flecs    
find_package(flecs QUIET)
if (NOT flecs_FOUND)
    FetchContent_Declare(
        flecs
        GIT_REPOSITORY  https://github.com/SanderMertens/flecs.git
        GIT_TAG         v3.2.9
    )
    FetchContent_MakeAvailable(flecs)
endif()
