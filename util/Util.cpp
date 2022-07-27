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
} // namespace detail

unsigned int DumpGlErrors()
{ return glGetError(); }

/**
 * String trimming functions from https://stackoverflow.com/questions/216823/how-to-trim-a-stdstring/217605#217605
 */

void LTrim(std::string &s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch)
    {
        return !std::isspace(ch);
    }));
}

void RTrim(std::string &s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch)
    {
        return !std::isspace(ch);
    }).base(), s.end());
}

void Trim(std::string &s)
{
    LTrim(s);
    RTrim(s);
}

std::string LTrimCopy(std::string s)
{
    LTrim(s);
    return s;
}

std::string RTrimCopy(std::string s)
{
    RTrim(s);
    return s;
}

std::string TrimCopy(std::string s)
{
    Trim(s);
    return s;
}

std::string ToUTF8(const std::u16string &s)
{
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> conv;
    return conv.to_bytes(s);
}

std::string ToUTF8(const std::u32string &s)
{
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> conv;
    return conv.to_bytes(s);
}

std::u16string ToUTF16(const std::string &s)
{
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> conv;
    return conv.from_bytes(s);
}

std::u16string ToUTF16(const std::u32string &s)
{
    std::wstring_convert<std::codecvt_utf16<char32_t>, char32_t> conv;
    std::string bytes = conv.to_bytes(s);
    return {reinterpret_cast<const char16_t *>(bytes.c_str()), bytes.length() / sizeof(char16_t)};
}

std::u32string ToUTF32(const std::string &s)
{
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> conv;
    return conv.from_bytes(s);
}

std::u32string ToUTF32(const std::u16string &s)
{
    const char16_t *pData = s.c_str();
    std::wstring_convert<std::codecvt_utf16<char32_t>, char32_t> conv;
    return conv.from_bytes(reinterpret_cast<const char *>(pData), reinterpret_cast<const char *>(pData + s.length()));
}

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

NS_END
