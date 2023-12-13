cmake_minimum_required(VERSION 3.14)

project(raylib_gamejam_template)
include(FetchContent)

# Resoruce copy macro
macro(configure_files srcDir destDir)
    message(STATUS "Configuring directory ${destDir}")
    make_directory(${destDir})

    file(GLOB templateFiles RELATIVE ${srcDir} "${srcDir}/*")
    foreach(templateFile ${templateFiles})
        set(srcTemplatePath ${srcDir}/${templateFile})
        if(NOT IS_DIRECTORY ${srcTemplatePath})
            message(STATUS "Configuring file ${templateFile}")
            configure_file(
                    ${srcTemplatePath}
                    ${destDir}/${templateFile}
                    COPYONLY)
        endif(NOT IS_DIRECTORY ${srcTemplatePath})
    endforeach(templateFile)
endmacro(configure_files)

# Include raylib, flecs, and additional modules and packages for each
include(${CMAKE_CURRENT_LIST_DIR}/packages/flecs.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/packages/raylib.cmake)

# Gather all src files and dependencies
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp src/*.h src/*.hpp)
file(GLOB_RECURSE RESOURCES CONFIGURE_DEPENDS resources/*)

# Setup the executable target
add_executable(${PROJECT_NAME} ${SOURCES} ${raygui_SOURCE_DIR/})
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)
target_compile_definitions(${PROJECT_NAME} PUBLIC -DRAYGUI_IMPLEMENTATION -DFLECS_NO_APP -DFLECS_NO_PLECS)

# Include directories
target_include_directories(${PROJECT_NAME} PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${raygui_SOURCE_DIR}/src 
    ${reasings_SOURCE_DIR}/src
    ${flecs_SOURCE_DIR}/include
)

# Link libraries
target_link_libraries(${PROJECT_NAME} PUBLIC 
    raylib 
    raylib_cpp 
    flecs::flecs_static
)

# Copy resources to build directory
configure_files(${CMAKE_CURRENT_SOURCE_DIR}/resources ${CMAKE_CURRENT_BINARY_DIR}/resources)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/etc/template.html ${CMAKE_CURRENT_BINARY_DIR}/etc/template.html)

# Web Configuration
if (${PLATFORM} STREQUAL "Web")
    # Tell Emscripten to build an example.html file.
    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".html")
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "--preload-file ./resources --shell-file ${CMAKE_CURRENT_SOURCE_DIR}/etc/template.html")
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/resources;${CMAKE_CURRENT_BINARY_DIR}/etc/template.html")

    # Enable debug symbols in non release builds
    if (NOT CMAKE_BUILD_TYPE STREQUAL "Release")
        target_compile_options(${PROJECT_NAME} PRIVATE -g)
        target_link_options(${PROJECT_NAME} PRIVATE -g -gsource-map --emit-symbol-map --emrun)
    endif()

    # Required linker flags for using Raylib with Emscripten
    target_link_options(${PROJECT_NAME} PRIVATE 
        -sEXPORTED_FUNCTIONS=['_main','_malloc'] 
        -sEXPORTED_RUNTIME_METHODS=['ccall','cwrap']
        -sUSE_GLFW=3 
        -sALLOW_MEMORY_GROWTH=1
        -sSTACK_SIZE=1mb
    )
endif()

# That's it! You should have an example executable that you can run. Have fun!
