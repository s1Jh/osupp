/*====================================================================================================================*/
/*  define.h                                                                                                          */
/*                                                                                                                    */
/*  This file contains all compile startTime definitions that notOSU uses to determine it's namespace names,               */
/*  default settings, build info, paths, etc.                                                                         */
/*                                                                                                                    */
/*  Last edited: 29.4.2022                                                                                            */
/*                                                                                                                    */
/*--------------------------------------------------------------------------------------------------------------------*/
#pragma once
/*====================================================================================================================*/
/*  Build information                                                                                                 */
/*--------------------------------------------------------------------------------------------------------------------*/
#define GAME_TITLE NotOSU

#if !defined(__PRETTY_FUNCTION__) && !defined(__GNUC__)
#define __PRETTY_FUNCTION__ __FUNCSIG__
#endif

#define ARRAY_SIZE(ARR) sizeof(ARR) / sizeof(ARR[0])

#define _STRINGIFY(x) #x
#define TOSTRING(x) _STRINGIFY(x)

#ifdef NDEBUG
constexpr const char *BUILD_TYPE = "Release";
#elif DEBUG
constexpr const char *BUILD_TYPE = "Debug";
#else
#error("No build type defined")
#endif

#ifdef ARCH_X86
constexpr const char *ARCH = "x86";
#elif ARCH_AMD64
constexpr const char *ARCH = "AMD64 / x64_86";
#elif ARCH_ARM
constexpr const char *ARCH = "arm";
#elif ARCH_IA64
constexpr const char *ARCH = "IA-64";
#else
#error("No processor architecture defined")
#endif

#ifdef WINDOWS
constexpr const char *PLATFORM = "Windows";
#elif LINUX
constexpr const char *PLATFORM = "Linux";
#else
#error("No platform defined")
#endif

constexpr int VERSION_MAJOR = 0;
constexpr int VERSION_MINOR = 6;
constexpr int VERSION_PATCH = 0;

constexpr const char *GL_VERSION_STR = "330 core";

constexpr const char *GL_DEFAULT_FRAGMENT_SHADER =
        "out vec4 fragColor;"
        "void main()"
        "{"
        "    fragColor = vec4(1.0f, 0.0f, 1.0f, 1.0f);"
        "}";

constexpr const char *GL_DEFAULT_VERTEX_SHADER =
        "layout(location = 0) in vec3 vPos;"
        "void main()"
        "{"
        "    gl_Position = vec4(vPos, 1.0f);"
        "}";

/*====================================================================================================================*/
/*  Default settings                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/
constexpr int SPRITE_ANIMATION_FPS = 20;
constexpr int SOUND_CHANNEL_COUNT = 16;

/*====================================================================================================================*/
/*  Constant asset name definitions                                                                                   */
/*--------------------------------------------------------------------------------------------------------------------*/
constexpr const char *NOTE_BASE_SPRITE = "note";
constexpr const char *NOTE_OVERLAY_SPRITE = "note_overlay";
constexpr const char *APPROACH_CIRCLE_SPRITE = "approach_circle";
constexpr const char *SLIDER_HEAD_SPRITE = "slider_head";
constexpr const char *SLIDER_TAIL_SPRITE = "slider_tail";
constexpr const char *SLIDER_HEAD_REPEAT_SPRITE = "slider_head_repeat";
constexpr const char *SLIDER_TAIL_REPEAT_SPRITE = "slider_tail_repeat";
constexpr const char *SLIDER_BODY_SPRITE = "slider_body";
constexpr const char *SLIDER_HITPOINT_SPRITE = "slider_hitpoint";
constexpr const char *SPINNER_SPRITE = "spinner";
constexpr const char *SPINNER_CENTER_SPRITE = "spinner_center";
constexpr const char *SPINNER_METER_SPRITE = "spinner_meter";
constexpr const char *SLIDER_BALL_SPRITE = "slider_ball";
constexpr const char *PLAY_FIELD_SPRITE = "play_field";

constexpr const char *SLIDER_SHADER = "slider";

constexpr const char *HIT_SOUND = "hitsound";

/*====================================================================================================================*/