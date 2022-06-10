cmake_minimum_required(VERSION 3.0)

project(notOSU)

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
        )

set(SOURCES
        ${SYSTEM_DIRECTORY}/df2.cpp
        ${SYSTEM_DIRECTORY}/Keyboard.cpp
        ${SYSTEM_DIRECTORY}/Mouse.cpp
        ${SYSTEM_DIRECTORY}/Renderer.cpp
        ${SYSTEM_DIRECTORY}/Shader.cpp
        ${SYSTEM_DIRECTORY}/Texture.cpp
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

        ${GAMEMODE_DIRECTORY}/CommonResourceLoaders.cpp
        ${GAMEMODE_DIRECTORY}/Standard.cpp

        ${OBJECT_DIRECTORY}/Note.cpp
        ${OBJECT_DIRECTORY}/Slider.cpp
        #${OBJECT_DIRECTORY}/SliderUtils.cpp
        ${OBJECT_DIRECTORY}/Spinner.cpp

        ${BASES_DIRECTORY}/BaseHitObject.cpp
        ${BASES_DIRECTORY}/BaseGameMode.cpp
        ${BASES_DIRECTORY}/BaseObjectTemplate.cpp

        Game.cpp
        MapInfo.cpp
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

add_executable(notOSU ${SOURCES})

target_link_libraries(notOSU glfw OpenGL::GL ${GLEW_LIBRARIES})

install(TARGETS notOSU RUNTIME DESTINATION bin)
