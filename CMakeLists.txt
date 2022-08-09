cmake_minimum_required(VERSION 3.0)

set(PROJECT_NAME notOSUProj)
set(TARGET_NAME notOSU)

project(${PROJECT_NAME})

# Set C++ version to the 2020 revision
set(CMAKE_CXX_STANDARD 20)

set(ROOT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
set(SYSTEM_DIRECTORY ${ROOT_DIRECTORY}/system)
set(SUI_DIRECTORY ${SYSTEM_DIRECTORY}/SUI)
set(GFX_DIRECTORY ${SYSTEM_DIRECTORY}/gfx)
set(INPUT_DIRECTORY ${SYSTEM_DIRECTORY}/input)
set(ANIMATORS_DIRECTORY ${SYSTEM_DIRECTORY}/animators)
set(TYPES_DIRECTORY ${SYSTEM_DIRECTORY}/types)
set(UTIL_DIRECTORY ${SYSTEM_DIRECTORY}/util)
set(AUDIO_DIRECTORY ${SYSTEM_DIRECTORY}/audio)
set(CORE_DIRECTORY ${SYSTEM_DIRECTORY}/core)
set(UTIL_DIRECTORY ${SYSTEM_DIRECTORY}/util)
set(IMGUI_DIRECTORY ${ROOT_DIRECTORY}/imgui)
set(LOADER_DIRECTORY ${ROOT_DIRECTORY}/loaders)
set(GAMEMODE_DIRECTORY ${ROOT_DIRECTORY}/gamemodes)
set(OBJECT_DIRECTORY ${ROOT_DIRECTORY}/objects)
set(BASES_DIRECTORY ${ROOT_DIRECTORY}/bases)
set(ASSET_DIRECTORY ${ROOT_DIRECTORY}/assets)
set(STATES_DIRECTORY ${ROOT_DIRECTORY}/states)

set(DIRECTORIES
        ${ROOT_DIRECTORY}
        ${SYSTEM_DIRECTORY}
        ${SUI_DIRECTORY}
        ${GFX_DIRECTORY}
        ${INPUT_DIRECTORY}
        ${ANIMATORS_DIRECTORY}
        ${TYPES_DIRECTORY}
        ${UTIL_DIRECTORY}
        ${AUDIO_DIRECTORY}
        ${CORE_DIRECTORY}
        ${UTIL_DIRECTORY}
        ${IMGUI_DIRECTORY}
        ${LOADER_DIRECTORY}
        ${GAMEMODE_DIRECTORY}
        ${OBJECT_DIRECTORY}
        ${BASES_DIRECTORY}
        ${ASSET_DIRECTORY}
        ${STATES_DIRECTORY}
        )

set(SOURCES
        ${SYSTEM_DIRECTORY}/df2.cpp
        ${SYSTEM_DIRECTORY}/Resources.cpp
        ${SYSTEM_DIRECTORY}/Locale.cpp
        ${SYSTEM_DIRECTORY}/Main.cpp
        ${SYSTEM_DIRECTORY}/Settings.cpp
        ${SYSTEM_DIRECTORY}/Setting.cpp
        # ${SYSTEM_DIRECTORY}/SUI.cpp

        ${CORE_DIRECTORY}/Context.cpp
        ${CORE_DIRECTORY}/Timing.cpp
        ${CORE_DIRECTORY}/State.cpp
        ${CORE_DIRECTORY}/StateHandler.cpp

        ${INPUT_DIRECTORY}/Keyboard.cpp
        ${INPUT_DIRECTORY}/Mouse.cpp

        ${AUDIO_DIRECTORY}/Audio.cpp
        ${AUDIO_DIRECTORY}/AudioDevice.cpp
        ${AUDIO_DIRECTORY}/AudioUtil.cpp
        ${AUDIO_DIRECTORY}/Channel.cpp
        ${AUDIO_DIRECTORY}/Sound.cpp
        ${AUDIO_DIRECTORY}/SoundSample.cpp
        ${AUDIO_DIRECTORY}/SoundStream.cpp

        ${GFX_DIRECTORY}/Renderer.cpp
        ${GFX_DIRECTORY}/Shader.cpp
        ${GFX_DIRECTORY}/Texture.cpp
        ${GFX_DIRECTORY}/Drawables.cpp
        ${GFX_DIRECTORY}/Sprite.cpp
        ${GFX_DIRECTORY}/Image.cpp
        ${GFX_DIRECTORY}/Mesh.cpp
        ${GFX_DIRECTORY}/Camera.cpp
        ${GFX_DIRECTORY}/GraphicsContext.cpp

        ${IMGUI_DIRECTORY}/imgui.cpp
        ${IMGUI_DIRECTORY}/imgui_draw.cpp
        ${IMGUI_DIRECTORY}/imgui_demo.cpp
        ${IMGUI_DIRECTORY}/imgui_impl_glfw.cpp
        ${IMGUI_DIRECTORY}/imgui_impl_opengl3.cpp
        ${IMGUI_DIRECTORY}/imgui_tables.cpp
        ${IMGUI_DIRECTORY}/imgui_widgets.cpp
        ${IMGUI_DIRECTORY}/misc/cpp/imgui_stdlib.cpp
        ${IMGUI_DIRECTORY}/misc/freetype/imgui_freetype.cpp

        #${SUI_DIRECTORY}/Button.cpp
        #${SUI_DIRECTORY}/Image.cpp

        ${UTIL_DIRECTORY}/Math.cpp
        ${UTIL_DIRECTORY}/Util.cpp
        ${UTIL_DIRECTORY}/Curve.cpp
        ${UTIL_DIRECTORY}/Log.cpp

        ${ANIMATORS_DIRECTORY}/AnimateTexture.cpp
        ${ANIMATORS_DIRECTORY}/DisableAfter.cpp
        ${ANIMATORS_DIRECTORY}/MoveLinear.cpp

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

        ${STATES_DIRECTORY}/StateInGame.cpp
        ${STATES_DIRECTORY}/StateInit.cpp
        ${STATES_DIRECTORY}/StateMainMenu.cpp

        ObjectSprite.cpp
        MapInfo.cpp
        Skin.cpp
        system/audio/AudioUtil.hpp system/audio/AudioUtil.cpp)

set(OpenGL_GL_PREFERENCE GLVND)

find_package(glfw3 3.3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(OpenAL REQUIRED)
find_package(GLEW REQUIRED)
find_package(Freetype REQUIRED)
find_package(FFmpeg REQUIRED avcodec avformat avutil)

include_directories(${DIRECTORIES})
include_directories(${GLEW_INCLUDE_DIRS})
include_directories(${FREETYPE_INCLUDE_DIRS})
include_directories(${FFMPEG_INCLUDE_DIRS})

set(LIBS
        glfw OpenGL::GL alut ${OPENAL_LIBRARY}
        ${GLEW_LIBRARIES} ${FREETYPE_LIBRARIES}
        ${FFMPEG_LIBRARIES})

set(PLATFORM LINUX)
set(ARCHITECTURE ARCH_AMD64)

message("Platform: ${PLATFORM}, architecture: ${ARCHITECTURE}")
add_compile_definitions(${PLATFORM})
add_compile_definitions(${ARCHITECTURE})

add_executable(${TARGET_NAME} ${SOURCES})

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

target_link_libraries(${TARGET_NAME} ${LIBS})

install(TARGETS ${TARGET_NAME} RUNTIME DESTINATION bin)
