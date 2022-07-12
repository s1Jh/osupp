#include "Util.hpp"
#include "AnimateTexture.hpp"

#include <GL/glew.h>

#define GLFW_DLL

#include <GLFW/glfw3.h>

#include <cstdlib>
#include <fstream>

NS_BEGIN
namespace detail
{
int CheckGLFWErrors(const std::string &file, int line,
                    const std::string &helper)
{
    LOG_ENTER("GLFW");
    const char *buff[256];
    int err = glfwGetError(buff);
    if (err != GLFW_NO_ERROR)
        log::error("@(", helper, ") Near line ", line, " In File: ", file, ": ",
                   *buff);
    return err;
}

unsigned int CheckErrors(const std::string &file, int line,
                         const std::string &helper)
{
    LOG_ENTER("GL");
    GLenum error = glGetError();
    const char *err_str;
    if (error != 0) {
        switch (error) {
            case GL_INVALID_ENUM:
                err_str = "GL_INVALID_ENUM";
                break;
            case GL_INVALID_VALUE:
                err_str = "GL_INVALID_VALUE";
                break;
            case GL_INVALID_OPERATION:
                err_str = "GL_INVALID_OPERATION";
                break;
            case GL_STACK_OVERFLOW:
                err_str = "GL_STACK_OVERFLOW";
                break;
            case GL_STACK_UNDERFLOW:
                err_str = "GL_STACK_UNDERFLOW";
                break;
            case GL_OUT_OF_MEMORY:
                err_str = "GL_OUT_OF_MEMORY";
                break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:
                err_str = "GL_INVALID_FRAMEBUFFER_OPERATION";
                break;
            default:
                err_str = "!INVALID ERROR!";
                break;
        }

        log::error("@(", helper, ") Near line: ", line, " In File: ", file, ": ",
                   err_str);
    }
    return error;
}

SectionEntry::~SectionEntry()
{
    ::PROJECT_NAMESPACE::log::sections.erase(
        ::PROJECT_NAMESPACE::log::sections.end() - 1);
}

SectionEntry::SectionEntry(const std::string &section)
{
    ::PROJECT_NAMESPACE::log::sections.push_back(section);
}
} // namespace detail

unsigned int DumpGlErrors()
{ return glGetError(); }

std::vector<std::string> GetCharacterSeparatedValues(const std::string &in, char sep)
{
    std::vector<std::string> results;

    std::stringstream ss(in);
    std::string str;
    while (getline(ss, str, sep)) {
        results.push_back(str);
    }

    return results;
}

std::recursive_mutex log::coutMutex;

#ifdef NDEBUG
bool log::enableDebug = false;
#else

bool log::enableDebug = true;

#endif

bool log::enabled = true;

std::vector<std::string> log::sections;

[[maybe_unused]] void log::setDebug(bool ns)
{ enableDebug = ns; }

void log::enable()
{ enabled = true; }

void log::disable()
{ enabled = false; }

NS_END
