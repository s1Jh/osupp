cmake_minimum_required(VERSION 3.0)

set(PROJECT_NAME notOSUProj)
set(TARGET_NAME notOSU)

project(${PROJECT_NAME})

# Set C++ version to the 2020 revision
set(CMAKE_CXX_STANDARD 20)

set(ROOT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
set(SYSTEM_DIRECTORY ${ROOT_DIRECTORY}/system)
set(GAME_DIRECTORY ${ROOT_DIRECTORY}/game)
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
set(OBJECT_DIRECTORY ${GAME_DIRECTORY}/objects)
set(OSU_OBJECT_DIRECTORY ${OBJECT_DIRECTORY}/osu)
set(ASSET_DIRECTORY ${ROOT_DIRECTORY}/assets)
set(STATES_DIRECTORY ${GAME_DIRECTORY}/states)
set(MAPPERS_DIRECTORY ${GAME_DIRECTORY}/mappers)

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
        ${OSU_OBJECT_DIRECTORY}
        ${GAME_DIRECTORY}
        ${ASSET_DIRECTORY}
        ${STATES_DIRECTORY}
        ${MAPPERS_DIRECTORY}
        )

set(SOURCES
        ${SYSTEM_DIRECTORY}/df2.cpp
        ${SYSTEM_DIRECTORY}/Resources.cpp
        ${SYSTEM_DIRECTORY}/Locale.cpp
        ${SYSTEM_DIRECTORY}/Main.cpp
        ${SYSTEM_DIRECTORY}/Settings.cpp
        ${SYSTEM_DIRECTORY}/Setting.cpp
        ${SYSTEM_DIRECTORY}/Timer.cpp
        # ${SYSTEM_DIRECTORY}/SUI.cpp

        ${CORE_DIRECTORY}/Context.cpp
        ${CORE_DIRECTORY}/Timing.cpp
        ${CORE_DIRECTORY}/State.cpp
        ${CORE_DIRECTORY}/StateHandler.cpp

        ${INPUT_DIRECTORY}/Keyboard.cpp
        ${INPUT_DIRECTORY}/Mouse.cpp

        ${MAPPERS_DIRECTORY}/HumanInput.cpp
        ${MAPPERS_DIRECTORY}/AutoPilot.cpp

        ${AUDIO_DIRECTORY}/Audio.cpp
        ${AUDIO_DIRECTORY}/AudioDevice.cpp
        ${AUDIO_DIRECTORY}/AudioUtil.cpp
        ${AUDIO_DIRECTORY}/Channel.cpp
        ${AUDIO_DIRECTORY}/Sound.cpp
        ${AUDIO_DIRECTORY}/SoundSample.cpp
        ${AUDIO_DIRECTORY}/SoundStream.cpp
        ${AUDIO_DIRECTORY}/AudioUtil.cpp

        ${GFX_DIRECTORY}/Renderer.cpp
        ${GFX_DIRECTORY}/Shader.cpp
        ${GFX_DIRECTORY}/Texture.cpp
        ${GFX_DIRECTORY}/Drawables.cpp
        ${GFX_DIRECTORY}/Sprite.cpp
        ${GFX_DIRECTORY}/Image.cpp
        ${GFX_DIRECTORY}/Mesh.cpp
        ${GFX_DIRECTORY}/Camera.cpp
        ${GFX_DIRECTORY}/GraphicsContext.cpp
        ${GFX_DIRECTORY}/Generics.cpp

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

        # ${GAMEMODE_DIRECTORY}/Standard.cpp

        ${OSU_OBJECT_DIRECTORY}/Note.cpp
        ${OSU_OBJECT_DIRECTORY}/Slider.cpp
        ${OSU_OBJECT_DIRECTORY}/Spinner.cpp

        ${OBJECT_DIRECTORY}/BaseHitObject.cpp
        ${OBJECT_DIRECTORY}/BaseObjectTemplate.cpp

        ${STATES_DIRECTORY}/StateInGame.cpp
        ${STATES_DIRECTORY}/StateInit.cpp
        ${STATES_DIRECTORY}/StateMainMenu.cpp

        ${GAME_DIRECTORY}/GameManager.cpp
        ${GAME_DIRECTORY}/ObjectSprite.cpp
        ${GAME_DIRECTORY}/MapInfo.cpp
        ${GAME_DIRECTORY}/Skin.cpp
)

set(OpenGL_GL_PREFERENCE GLVND)

if (MSVC)

else()
	find_package(glfw3 3.3 REQUIRED)
	find_package(OpenGL REQUIRED)
	find_package(OpenAL REQUIRED)
	find_package(GLEW REQUIRED)
	find_package(Freetype REQUIRED)
	find_package(FFmpeg REQUIRED avcodec avformat avutil)

	set(GLFW_HEADERS "")
	set(GLFW_LIBS glfw)

	if (${OPENAL_FOUND})
		set(OAL_HEADERS "${OPENAL_INCLUDE_DIR}")
		set(OAL_LIBS "${OPENAL_LIBRARY}")
	else()
		message(FATAL_ERROR "OpenAL could not be located")
	endif()

	if (${OPENGL_FOUND})
		set(OGL_HEADERS "${OPENGL_INCLUDE_DIR}")
		set(OGL_LIBS OpenGL::GL)
	else()
		message(FATAL_ERROR "OpenGL could not be located")
	endif()

	if (${FFMPEG_FOUND})
		set(FFMPEG_HEADERS "${FFMPEG_INCLUDE_DIR}")
		set(FFMPEG_LIBS "${FFMPEG_LIBRARIES}")
	else()
		message(FATAL_ERROR "FFMPEG could not be located")
	endif()

	if (${FREETYPE_FOUND})
		set(FT2_HEADERS "${FREETYPE_INCLUDE_DIRS}")
		set(FT2_LIBS "${FREETYPE_LIBRARIES}")
	else()
		message(FATAL_ERROR "Freetype 2 could not be located")
	endif()
endif()

include_directories(${DIRECTORIES})

set(LIBS)

function(ConfigureLibrary LIBRARY_NAME)
	
	message("Configuring ${LIBRARY_NAME}")
	
	set(HEADERS ${LIBRARY_NAME}_HEADERS)
	set(LIBRARIES ${LIBRARY_NAME}_LIBS)
	
	if (DEFINED ${HEADERS})
		include_directories(${${HEADERS}})
	else()
		message(FATAL_ERROR "${LIBRARY_NAME} headers not defined")
	endif()
	
	if (DEFINED ${LIBRARIES})
		set(LIBS "${LIBS};${${LIBRARIES}}" PARENT_SCOPE)
	else()
		message(FATAL_ERROR "${LIBRARY_NAME} libraries not defined")
	endif()

endfunction()

ConfigureLibrary(GLFW)
ConfigureLibrary(OAL)
ConfigureLibrary(OGL)
ConfigureLibrary(FFMPEG)
ConfigureLibrary(FT2)

string(STRIP "${LIBS}" LIBS)

if (NOT DEFINED PLATFORM)
	message(FATAL_ERROR "No platform defined")
endif()
if (NOT DEFINED ARCHITECTURE)
	message(FATAL_ERROR "No architecture defined")
endif()

message("Platform: ${PLATFORM}, architecture: ${ARCHITECTURE}")
message("Linking against: ${LIBS}")
add_compile_definitions(${PLATFORM})
add_compile_definitions(${ARCHITECTURE})

add_executable(${TARGET_NAME} ${SOURCES})

# Set the proper warning levels
if (MSVC)
    target_compile_options(${TARGET_NAME} PRIVATE /W4 /Zc:preprocessor)
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
