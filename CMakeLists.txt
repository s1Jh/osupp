cmake_minimum_required(VERSION 3.0)

set(PROJECT_NAME notOSUProj)
set(TARGET_NAME notOSU)

project(${PROJECT_NAME})

# Set C++ version to the 2020 revision
set(CMAKE_CXX_STANDARD 20)

set(ROOT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})

set(SYSTEM_DIRECTORY ${ROOT_DIRECTORY}/system)
set(SUI_DIRECTORY ${SYSTEM_DIRECTORY}/SUI)
set(DRAWABLES_DIRECTORY ${SYSTEM_DIRECTORY}/drawables)
set(ANIMATORS_DIRECTORY ${SYSTEM_DIRECTORY}/animators)
set(TYPES_DIRECTORY ${ROOT_DIRECTORY}/types)
set(UTIL_DIRECTORY ${ROOT_DIRECTORY}/util)
set(LOADER_DIRECTORY ${ROOT_DIRECTORY}/loaders)
set(GAMEMODE_DIRECTORY ${ROOT_DIRECTORY}/gamemodes)
set(OBJECT_DIRECTORY ${ROOT_DIRECTORY}/objects)
set(BASES_DIRECTORY ${ROOT_DIRECTORY}/bases)
set(ASSET_DIRECTORY ${ROOT_DIRECTORY}/assets)

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
        ${DRAWABLES_DIRECTORY}
        ${ASSET_DIRECTORY}
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
        ${SYSTEM_DIRECTORY}/Context.cpp
        ${SYSTEM_DIRECTORY}/Locale.cpp
        ${SYSTEM_DIRECTORY}/Main.cpp
        ${SYSTEM_DIRECTORY}/State.cpp
        ${SYSTEM_DIRECTORY}/StateHandler.cpp
        ${SYSTEM_DIRECTORY}/Settings.cpp
        ${SYSTEM_DIRECTORY}/Setting.cpp
        # ${SYSTEM_DIRECTORY}/SUI.cpp


        ${SYSTEM_DIRECTORY}/imgui/imgui.cpp
        ${SYSTEM_DIRECTORY}/imgui/imgui_stdlib.cpp
        ${SYSTEM_DIRECTORY}/imgui/imgui_draw.cpp
        ${SYSTEM_DIRECTORY}/imgui/imgui_demo.cpp
        ${SYSTEM_DIRECTORY}/imgui/imgui_impl_glfw.cpp
        ${SYSTEM_DIRECTORY}/imgui/imgui_impl_opengl3.cpp
        ${SYSTEM_DIRECTORY}/imgui/imgui_tables.cpp
        ${SYSTEM_DIRECTORY}/imgui/imgui_widgets.cpp

        #${SUI_DIRECTORY}/Button.cpp
        #${SUI_DIRECTORY}/Image.cpp

        ${UTIL_DIRECTORY}/Math.cpp
        ${UTIL_DIRECTORY}/Util.cpp
        ${UTIL_DIRECTORY}/Curve.cpp
        ${UTIL_DIRECTORY}/Log.cpp

        ${ANIMATORS_DIRECTORY}/AnimateTexture.cpp
        ${ANIMATORS_DIRECTORY}/DisableAfter.cpp
        ${ANIMATORS_DIRECTORY}/MoveLinear.cpp

        ${DRAWABLES_DIRECTORY}/Drawables.cpp

        ${LOADER_DIRECTORY}/OBJLoader.cpp
        ${LOADER_DIRECTORY}/MAPLoader.cpp
        ${LOADER_DIRECTORY}/OSULoader.cpp

        ${GAMEMODE_DIRECTORY}/Standard.cpp

        ${OBJECT_DIRECTORY}/Note.cpp
        ${OBJECT_DIRECTORY}/Slider.cpp
        ${OBJECT_DIRECTORY}/Spinner.cpp

        ${BASES_DIRECTORY}/BaseHitObject.cpp
        ${BASES_DIRECTORY}/BaseGameMode.cpp
        ${BASES_DIRECTORY}/BaseObjectTemplate.cpp

        ObjectSprite.cpp
        StateInGame.cpp
        StateInit.cpp
        StateMainMenu.cpp
        MapInfo.cpp
        Skin.cpp
        )

set(OpenGL_GL_PREFERENCE GLVND)

find_package(glfw3 3.3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

include_directories(${DIRECTORIES})
include_directories(${GLEW_INCLUDE_DIRS})

set(PLATFORM LINUX)
set(ARCHITECTURE ARCH_AMD64)

message("Platform: ${PLATFORM}, architecture: ${ARCHITECTURE}")
add_compile_definitions(${PLATFORM})
add_compile_definitions(${ARCHITECTURE})

add_executable(${TARGET_NAME} ${SOURCES} ${RES_SOURCES})

# Set the proper warning levels
if (MSVC)
    target_compile_options(${TARGET_NAME} PRIVATE /W4 /WX)
else ()
    target_compile_options(${TARGET_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif ()

add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -P ${ROOT_DIRECTORY}/ConfigureFiles.cmake
        )

add_custom_target(CopyFiles
        COMMAND ${CMAKE_COMMAND} -P ${ROOT_DIRECTORY}/ConfigureFiles.cmake
        )

# Set defines
target_compile_definitions(${TARGET_NAME} PRIVATE
        $<$<CONFIG:Debug>:DEBUG=1>
        $<$<CONFIG:Release>:RELEASE=1>
        $<$<CONFIG:RelWithDebInfo>:RELDEB=1>
        $<$<CONFIG:MinSizeRel>:MINREL=1>
        )

target_link_libraries(${TARGET_NAME} glfw OpenGL::GL ${GLEW_LIBRARIES})

install(TARGETS ${TARGET_NAME} RUNTIME DESTINATION bin)
