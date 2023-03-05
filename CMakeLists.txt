cmake_minimum_required(VERSION 3.0)

set(PROJECT_NAME osuppproject)
set(TARGET_NAME osupp)

project(${PROJECT_NAME})

# Set C++ version to the 2020 revision
set(CMAKE_CXX_STANDARD 20)

set(ROOT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
set(SYSTEM_DIRECTORY ${ROOT_DIRECTORY}/system)
set(	VIDEO_DIRECTORY ${SYSTEM_DIRECTORY}/video)
set(	INPUT_DIRECTORY ${SYSTEM_DIRECTORY}/input)
set(	ANIMATORS_DIRECTORY ${VIDEO_DIRECTORY}/animators)
set(	TYPES_DIRECTORY ${SYSTEM_DIRECTORY}/types)
set(	UTIL_DIRECTORY ${SYSTEM_DIRECTORY}/util)
set(	AUDIO_DIRECTORY ${SYSTEM_DIRECTORY}/audio)
set(	CORE_DIRECTORY ${SYSTEM_DIRECTORY}/core)
set(	MATH_DIRECTORY ${SYSTEM_DIRECTORY}/math)
set(	TASKS_DIRECTORY ${SYSTEM_DIRECTORY}/tasks)
set(	GUI_DIRECTORY ${SYSTEM_DIRECTORY}/gui)
set(		GUI_OBJECTS_DIRECTORY ${GUI_DIRECTORY}/objects)
set(	STATE_DIRECTORY ${SYSTEM_DIRECTORY}/state)
set(GAME_DIRECTORY ${ROOT_DIRECTORY}/game)
set(	GAMEMODE_DIRECTORY ${ROOT_DIRECTORY}/gamemodes)
set(	OBJECT_DIRECTORY ${GAME_DIRECTORY}/objects)
set(		OSU_OBJECT_DIRECTORY ${OBJECT_DIRECTORY}/osu)
set(	MAPPERS_DIRECTORY ${GAME_DIRECTORY}/mappers)
set(	STATES_DIRECTORY ${GAME_DIRECTORY}/states)
set(	COMPAT_DIRECTORY ${GAME_DIRECTORY}/compat)
set(EXTERNALS_DIRECTORY ${ROOT_DIRECTORY}/external)
set(	IMGUI_DIRECTORY ${EXTERNALS_DIRECTORY}/imgui)
set(	STB_DIRECTORY ${EXTERNALS_DIRECTORY}/stb)
set(	DATE_DIRECTORY ${EXTERNALS_DIRECTORY}/date)
set(	NAMEOF_DIRECTORY ${EXTERNALS_DIRECTORY}/nameof)
set(ASSET_DIRECTORY ${ROOT_DIRECTORY}/assets)
set(LOADER_DIRECTORY ${ROOT_DIRECTORY}/loaders)

set(DIRECTORIES
        ${ROOT_DIRECTORY}
        ${SYSTEM_DIRECTORY}
        	${VIDEO_DIRECTORY}
        	${INPUT_DIRECTORY}
        	${ANIMATORS_DIRECTORY}
        	${TYPES_DIRECTORY}
        	${UTIL_DIRECTORY}
			${GUI_DIRECTORY}
				${GUI_OBJECTS_DIRECTORY}
        	${AUDIO_DIRECTORY}
			${MATH_DIRECTORY}
        	${CORE_DIRECTORY}
        	${UTIL_DIRECTORY}
			${STATE_DIRECTORY}
			${TASKS_DIRECTORY}

		${EXTERNALS_DIRECTORY}
			${IMGUI_DIRECTORY}
			${STB_DIRECTORY}
			${DATE_DIRECTORY}/include
			${NAMEOF_DIRECTORY}/include

		${GAME_DIRECTORY}
        	${GAMEMODE_DIRECTORY}
        	${OBJECT_DIRECTORY}
        		${OSU_OBJECT_DIRECTORY}
			${COMPAT_DIRECTORY}
        	${STATES_DIRECTORY}
        	${MAPPERS_DIRECTORY}

		${ASSET_DIRECTORY}

		${LOADER_DIRECTORY}
        )

set(SOURCES
        ${CORE_DIRECTORY}/Timing.cpp
		${CORE_DIRECTORY}/Time.cpp
		${CORE_DIRECTORY}/Program.cpp
		${CORE_DIRECTORY}/Error.cpp
		${CORE_DIRECTORY}/ToFromString.cpp
		${CORE_DIRECTORY}/Log.cpp

        ${INPUT_DIRECTORY}/Keyboard.cpp
        ${INPUT_DIRECTORY}/Mouse.cpp

		${STATE_DIRECTORY}/Context.cpp
		${STATE_DIRECTORY}/BaseState.cpp
		${STATE_DIRECTORY}/BaseStateHandler.cpp

		${COMPAT_DIRECTORY}/Import.cpp

        ${MAPPERS_DIRECTORY}/HumanInput.cpp
        ${MAPPERS_DIRECTORY}/AutoPilot.cpp

		${TASKS_DIRECTORY}/Tasks.cpp

		${TYPES_DIRECTORY}/ConversionFunctions.cpp

        ${AUDIO_DIRECTORY}/Audio.cpp
        ${AUDIO_DIRECTORY}/AudioDevice.cpp
        ${AUDIO_DIRECTORY}/AudioUtil.cpp
        ${AUDIO_DIRECTORY}/Channel.cpp
        ${AUDIO_DIRECTORY}/Sound.cpp
        ${AUDIO_DIRECTORY}/SoundSample.cpp
        ${AUDIO_DIRECTORY}/SoundStream.cpp
        ${AUDIO_DIRECTORY}/AudioUtil.cpp

		${VIDEO_DIRECTORY}/Helpers.cpp
        ${VIDEO_DIRECTORY}/LambdaRender.cpp
		${VIDEO_DIRECTORY}/StandardDrawCalls.cpp
        ${VIDEO_DIRECTORY}/Shader.cpp
        ${VIDEO_DIRECTORY}/Texture.cpp
        ${VIDEO_DIRECTORY}/Sprite.cpp
        ${VIDEO_DIRECTORY}/Image.cpp
        ${VIDEO_DIRECTORY}/Mesh.cpp
        ${VIDEO_DIRECTORY}/Camera.cpp
        ${VIDEO_DIRECTORY}/GraphicsContext.cpp
		${VIDEO_DIRECTORY}/AnimationController.cpp
		${VIDEO_DIRECTORY}/IAnimation.cpp

        ${IMGUI_DIRECTORY}/imgui.cpp
        ${IMGUI_DIRECTORY}/imgui_draw.cpp
        ${IMGUI_DIRECTORY}/imgui_demo.cpp
        ${IMGUI_DIRECTORY}/backends/imgui_impl_sdl.cpp
        ${IMGUI_DIRECTORY}/backends/imgui_impl_opengl3.cpp
        ${IMGUI_DIRECTORY}/imgui_tables.cpp
        ${IMGUI_DIRECTORY}/imgui_widgets.cpp
        ${IMGUI_DIRECTORY}/misc/cpp/imgui_stdlib.cpp
        ${IMGUI_DIRECTORY}/misc/freetype/imgui_freetype.cpp

		${UTIL_DIRECTORY}/Setting.cpp
		${UTIL_DIRECTORY}/Files.cpp
		${UTIL_DIRECTORY}/Settings.cpp
		${UTIL_DIRECTORY}/Locale.cpp
		${UTIL_DIRECTORY}/df2.cpp
        ${UTIL_DIRECTORY}/Util.cpp
		${UTIL_DIRECTORY}/StrUtil.cpp

		${GUI_DIRECTORY}/GuiElement.cpp
		${GUI_DIRECTORY}/GuiRoot.cpp
		${GUI_DIRECTORY}/GuiState.cpp
		${GUI_DIRECTORY}/GuiUtil.cpp
		${GUI_DIRECTORY}/GuiCollections.cpp
		${GUI_DIRECTORY}/GuiPrototype.cpp
		${GUI_DIRECTORY}/GuiTask.cpp
		${GUI_DIRECTORY}/GuiModal.cpp
		${GUI_DIRECTORY}/GuiIContainsElements.cpp
		${GUI_DIRECTORY}/GuiIContainsModals.cpp		
		${GUI_DIRECTORY}/GuiVariable.cpp
		${GUI_DIRECTORY}/GuiRegistry.cpp

		${GUI_OBJECTS_DIRECTORY}/GuiRect.cpp
		${GUI_OBJECTS_DIRECTORY}/GuiButton.cpp
		${GUI_OBJECTS_DIRECTORY}/GuiBackground.cpp
		${GUI_OBJECTS_DIRECTORY}/GuiKeyBind.cpp

		${MATH_DIRECTORY}/Curve.cpp

        ${ANIMATORS_DIRECTORY}/MoveLinear.cpp
		${ANIMATORS_DIRECTORY}/BopToBpm.cpp
		${ANIMATORS_DIRECTORY}/CircleAbout.cpp
		${ANIMATORS_DIRECTORY}/LookAt.cpp

        ${LOADER_DIRECTORY}/OBJLoader.cpp
        ${LOADER_DIRECTORY}/MAPLoader.cpp
        ${LOADER_DIRECTORY}/OSULoader.cpp

        ${OSU_OBJECT_DIRECTORY}/Note.cpp
        ${OSU_OBJECT_DIRECTORY}/Slider.cpp
        ${OSU_OBJECT_DIRECTORY}/Spinner.cpp

        ${OBJECT_DIRECTORY}/BaseHitObject.cpp
        ${OBJECT_DIRECTORY}/BaseObjectTemplate.cpp

        ${STATES_DIRECTORY}/StateInGame.cpp
        ${STATES_DIRECTORY}/StateInit.cpp
        ${STATES_DIRECTORY}/StateMainMenu.cpp

        ${GAME_DIRECTORY}/GameManager.cpp
		${GAME_DIRECTORY}/MapManager.cpp
        ${GAME_DIRECTORY}/ObjectSprite.cpp
        ${GAME_DIRECTORY}/MapInfo.cpp
        ${GAME_DIRECTORY}/Skin.cpp
		${GAME_DIRECTORY}/SliderTrail.cpp
		${GAME_DIRECTORY}/GameTask.cpp
)

set(OpenGL_GL_PREFERENCE GLVND)

if (MSVC)

else()
	find_package(SDL2 REQUIRED)
	find_package(OpenGL REQUIRED)
	find_package(OpenAL REQUIRED)
	find_package(GLEW REQUIRED)
	find_package(Freetype REQUIRED)
	find_package(FFmpeg REQUIRED avcodec avformat avutil)

	set(SDL_HEADERS ${SDL2_INCLUDE_DIRS})
	set(SDL_LIBS SDL2::SDL2)

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
		string(REPLACE ".so" ".so " FFMPEG_LIBRARIES ${FFMPEG_LIBRARIES})
		set(FFMPEG_LIBS "${FFMPEG_LIBRARIES}")
		message(${FFMPEG_LIBRARIES})
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
		string(REPLACE " " ";" ${LIBRARIES} ${${LIBRARIES}})
		set(LIBS "${LIBS};${${LIBRARIES}}" PARENT_SCOPE)
	else()
		message(FATAL_ERROR "${LIBRARY_NAME} libraries not defined")
	endif()

endfunction()

ConfigureLibrary(OGL)
ConfigureLibrary(SDL)
ConfigureLibrary(OAL)
ConfigureLibrary(FFMPEG)
ConfigureLibrary(FT2)

add_library(date STATIC ${DATE_DIRECTORY}/src/tz.cpp)
target_compile_definitions(date PUBLIC USE_OS_TZDB)
set(LIBS "${LIBS};date")

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
		IMGUI_USER_CONFIG="IMGuiConfig.hpp"
        )

target_link_libraries(${TARGET_NAME} ${LIBS})

install(TARGETS ${TARGET_NAME} RUNTIME DESTINATION bin)
