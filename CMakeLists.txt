cmake_minimum_required(VERSION 3.0)

set(PROJECT_NAME notOSUProj)
set(TARGET_NAME notOSU)

project(${PROJECT_NAME})

# Set C++ version to the 2020 revision
set(CMAKE_CXX_STANDARD 20)

set(ROOT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})

set(SYSTEM_DIRECTORY ${ROOT_DIRECTORY}/system)
set(SUI_DIRECTORY ${SYSTEM_DIRECTORY}/SUI)
set(TYPES_DIRECTORY ${ROOT_DIRECTORY}/types)
set(UTIL_DIRECTORY ${ROOT_DIRECTORY}/util)
set(LOADER_DIRECTORY ${ROOT_DIRECTORY}/loaders)
set(GAMEMODE_DIRECTORY ${ROOT_DIRECTORY}/gamemodes)
set(OBJECT_DIRECTORY ${ROOT_DIRECTORY}/objects)
set(BASES_DIRECTORY ${ROOT_DIRECTORY}/bases)
set(ANIMATORS_DIRECTORY ${SYSTEM_DIRECTORY}/animators)

set(DIRECTORIES
        ${ROOT_DIRECTORY}
        ${SYSTEM_DIRECTORY}
        ${TYPES_DIRECTORY}
        ${UTIL_DIRECTORY}
        ${LOADER_DIRECTORY}
        ${GAMEMODE_DIRECTORY}
        ${OBJECT_DIRECTORY}
        ${BASES_DIRECTORY}
        ${SUI_DIRECTORY}
        ${ANIMATORS_DIRECTORY}
        )

set(SOURCES
        ${SYSTEM_DIRECTORY}/df2.cpp
        ${SYSTEM_DIRECTORY}/Keyboard.cpp
        ${SYSTEM_DIRECTORY}/Mouse.cpp
        ${SYSTEM_DIRECTORY}/Renderer.cpp
        ${SYSTEM_DIRECTORY}/Shader.cpp
        ${SYSTEM_DIRECTORY}/Texture.cpp
        ${SYSTEM_DIRECTORY}/Sprite.cpp
        ${SYSTEM_DIRECTORY}/Image.cpp
        ${SYSTEM_DIRECTORY}/Timing.cpp
        ${SYSTEM_DIRECTORY}/Resources.cpp
        ${SYSTEM_DIRECTORY}/Mesh.cpp
        ${SYSTEM_DIRECTORY}/Camera.cpp
        #${SYSTEM_DIRECTORY}/SUI.cpp

        #${SUI_DIRECTORY}/Button.cpp
        #${SUI_DIRECTORY}/Image.cpp

        ${UTIL_DIRECTORY}/Math.cpp
        ${UTIL_DIRECTORY}/Util.cpp
        ${UTIL_DIRECTORY}/Curve.cpp

        ${LOADER_DIRECTORY}/OBJLoader.cpp
        ${LOADER_DIRECTORY}/MAPLoader.cpp

        ${GAMEMODE_DIRECTORY}/Standard.cpp

        ${OBJECT_DIRECTORY}/Note.cpp
        ${OBJECT_DIRECTORY}/Slider.cpp
        ${OBJECT_DIRECTORY}/Spinner.cpp

        ${BASES_DIRECTORY}/BaseHitObject.cpp
        ${BASES_DIRECTORY}/BaseGameMode.cpp
        ${BASES_DIRECTORY}/BaseObjectTemplate.cpp

        ${ANIMATORS_DIRECTORY}/AnimateTexture.cpp
        ${ANIMATORS_DIRECTORY}/DisableAfter.cpp
        ${ANIMATORS_DIRECTORY}/MoveLinear.cpp

        NotOSUObjectSprite.cpp
        Game.cpp
        MapInfo.cpp
        Skin.cpp
        )

set(OpenGL_GL_PREFERENCE GLVND)

find_package(glfw3 3.3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

include_directories(${DIRECTORIES})
include_directories(${GLEW_INCLUDE_DIRS})

add_compile_definitions(LINUX)
add_compile_definitions(ARCH_AMD64)
add_compile_definitions(DEBUG)

add_executable(${TARGET_NAME} ${SOURCES})

# Set the proper warning levels
if (MSVC)
    target_compile_options(${TARGET_NAME} PRIVATE /W4 /WX)
else ()
    target_compile_options(${TARGET_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif ()

target_link_libraries(${TARGET_NAME} glfw OpenGL::GL ${GLEW_LIBRARIES})

install(TARGETS ${TARGET_NAME} RUNTIME DESTINATION bin)
