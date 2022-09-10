/*******************************************************************************
 * Copyright (c) 2022 sijh (s1Jh.199[at]gmail.com)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 ******************************************************************************/
#pragma once

#include "define.hpp"

/***************************************************************/
/* Linux OpenGL configuration and includes                     */
/***************************************************************/
#ifdef LINUX
#   include <dlfcn.h>
#   define GLDECL
#endif // LINUX

/***************************************************************/
/* Windows OpenGL configuration and includes                   */
/***************************************************************/
#ifdef WIN32
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <wingdi.h>
#define GLDECL WINGDIAPI

#define GL_ARRAY_BUFFER                     0x8892 // Acquired from:
#define GL_ARRAY_BUFFER_BINDING             0x8894 // https://www.opengl.org/registry/api/GL/glext.h
#define GL_COLOR_ATTACHMENT0                0x8CE0
#define GL_COMPILE_STATUS                   0x8B81
#define GL_CURRENT_PROGRAM                  0x8B8D
#define GL_DYNAMIC_DRAW                     0x88E8
#define GL_ELEMENT_ARRAY_BUFFER             0x8893
#define GL_ELEMENT_ARRAY_BUFFER_BINDING     0x8895
#define GL_FRAGMENT_SHADER                  0x8B30
#define GL_FRAMEBUFFER                      0x8D40
#define GL_FRAMEBUFFER_COMPLETE             0x8CD5
#define GL_FUNC_ADD                         0x8006
#define GL_INVALID_FRAMEBUFFER_OPERATION    0x0506
#define GL_MAJOR_VERSION                    0x821B
#define GL_MINOR_VERSION                    0x821C
#define GL_STATIC_DRAW                      0x88E4
#define GL_STREAM_DRAW                      0x88E0
#define GL_TEXTURE0                         0x84C0
#define GL_VERTEX_SHADER                    0x8B31
#define GL_LINK_STATUS                      0x8b82
#define GL_GEOMETRY_SHADER                  0x8dd9
#define GL_DEBUG_OUTPUT                     0x92e0
#define GL_SHADING_LANGUAGE_VERSION         0x8b8c
#define GL_NUM_EXTENSIONS                   0x821d
#define GL_ACTIVE_TEXTURE                   0x84e0
#define GL_VERTEX_ARRAY_BINDING             0x85b5
#define GL_BLEND_COLOR	                    0x8005
#define GL_BLEND_DST_ALPHA	                0x80ca
#define GL_BLEND_DST_RGB	                0x80c8
#define GL_BLEND_EQUATION	                0x8009
#define GL_BLEND_EQUATION_ALPHA	            0x883d
#define GL_BLEND_EQUATION_RGB	            0x8009
#define GL_BLEND_SRC_ALPHA	                0x80cb
#define GL_BLEND_SRC_RGB	                0x80c9
#define GL_INFO_LOG_LENGTH                  0x8b84


typedef char GLchar;
typedef unsigned int GLenum;
typedef unsigned char* GLubyteptr;
typedef int GLint;
typedef unsigned int GLuint;
typedef ptrdiff_t GLintptr;
typedef ptrdiff_t GLsizeiptr;

#endif // WIN32

/***************************************************************/
/* Includes                                                    */
/***************************************************************/
#include <GL/gl.h>

#ifdef X11
#include <GL/glx.h>
#endif // X11

/***************************************************************/
/* Function list defines                                       */
/***************************************************************/
#define GL_FUNC_LIST_SHARED \
    GLEXT ( GLuint, CreateProgram, void ) \
    GLEXT ( void, LinkProgram, GLuint program ) \
    GLEXT ( void, UseProgram, GLuint program ) \
    GLEXT ( void, DeleteProgram, GLuint program ) \
    GLEXT ( void, GetProgramiv, GLuint program, GLenum pname, GLint *params ) \
    GLEXT ( void, GetProgramInfoLog, GLuint program, GLsizei maxLength, GLsizei *length, GLchar *infoLog) \
    GLEXT ( GLuint, CreateShader, GLenum type) \
    GLEXT ( void, AttachShader, GLuint program, GLuint shader) \
    GLEXT ( void, CompileShader, GLuint shader) \
    GLEXT ( void, DetachShader, GLuint program, GLuint shader) \
    GLEXT ( void, ShaderSource, GLuint shader, GLsizei count, const GLchar **string, const GLint *length) \
    GLEXT ( void, DeleteShader, GLuint shader ) \
    GLEXT ( void, GetShaderiv, GLuint shader, GLenum pname, GLint *params ) \
    GLEXT ( void, GetShaderInfoLog, GLuint shader, GLsizei maxLength, GLsizei *length, GLchar *infoLog) \
    GLEXT ( GLint, GetUniformLocation, GLuint program, const GLchar *name ) \
    GLEXT ( void, Uniform1f, GLint location, GLfloat v0 ) \
    GLEXT ( void, Uniform2f, GLint location, GLfloat v0, GLfloat v1 ) \
    GLEXT ( void, Uniform3f, GLint location, GLfloat v0, GLfloat v1, GLfloat v2 ) \
    GLEXT ( void, Uniform4f, GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3 ) \
    GLEXT ( void, Uniform1i, GLint location, GLint v0 ) \
    GLEXT ( void, Uniform2i, GLint location, GLint v0, GLint v1 ) \
    GLEXT ( void, Uniform3i, GLint location, GLint v0, GLint v1, GLint v2 ) \
    GLEXT ( void, Uniform4i, GLint location, GLint v0, GLint v1, GLint v2, GLint v3 ) \
    GLEXT ( void, Uniform1ui, GLint location, GLuint v0 ) \
    GLEXT ( void, Uniform2ui, GLint location, GLuint v0, GLuint v1 ) \
    GLEXT ( void, Uniform3ui, GLint location, GLuint v0, GLuint v1, GLuint v2 ) \
    GLEXT ( void, Uniform4ui, GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3 ) \
    GLEXT ( void, Uniform1fv, GLint location, GLsizei count, const GLfloat *value ) \
    GLEXT ( void, Uniform2fv, GLint location, GLsizei count, const GLfloat *value ) \
    GLEXT ( void, Uniform3fv, GLint location, GLsizei count, const GLfloat *value ) \
    GLEXT ( void, Uniform4fv, GLint location, GLsizei count, const GLfloat *value ) \
    GLEXT ( void, Uniform1iv, GLint location, GLsizei count, const GLint *value ) \
    GLEXT ( void, Uniform2iv, GLint location, GLsizei count, const GLint *value ) \
    GLEXT ( void, Uniform3iv, GLint location, GLsizei count, const GLint *value ) \
    GLEXT ( void, Uniform4iv, GLint location, GLsizei count, const GLint *value ) \
    GLEXT ( void, Uniform1uiv, GLint location, GLsizei count, const GLuint *value ) \
    GLEXT ( void, Uniform2uiv, GLint location, GLsizei count, const GLuint *value ) \
    GLEXT ( void, Uniform3uiv, GLint location, GLsizei count, const GLuint *value ) \
    GLEXT ( void, Uniform4uiv, GLint location, GLsizei count, const GLuint *value ) \
    GLEXT ( void, UniformMatrix2fv, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value ) \
    GLEXT ( void, UniformMatrix3fv, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value ) \
    GLEXT ( void, UniformMatrix4fv, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value ) \
    GLEXT ( void, UniformMatrix2x3fv, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value ) \
    GLEXT ( void, UniformMatrix3x2fv, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value ) \
    GLEXT ( void, UniformMatrix2x4fv, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value ) \
    GLEXT ( void, UniformMatrix4x2fv, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value ) \
    GLEXT ( void, UniformMatrix3x4fv, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value ) \
    GLEXT ( void, UniformMatrix4x3fv, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value ) \
    GLEXT ( void, GenVertexArrays, GLsizei n, GLuint *arrays) \
    GLEXT ( void, GenBuffers, GLsizei n, GLuint *arrays ) \
    GLEXT ( void, BindVertexArray, GLuint array ) \
    GLEXT ( void, BindBuffer, GLenum target, GLuint buffer ) \
    GLEXT ( void, BufferData, GLenum target, GLsizeiptr size, const GLvoid *data, GLenum usage ) \
    GLEXT ( void, EnableVertexAttribArray, GLuint index ) \
    GLEXT ( void, DisableVertexAttribArray, GLuint index ) \
    GLEXT ( void, VertexAttribPointer, GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid * pointer ) \
    GLEXT ( void, VertexAttribIPointer, GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid * pointer ) \
    GLEXT ( void, DeleteBuffers, GLsizei n, const GLuint *buffers ) \
    GLEXT ( void, DeleteVertexArrays, GLsizei n, const GLuint *arrays ) \
	GLEXT ( void, GenerateMipmap, GLenum target ) \
	GLEXT ( void, GenerateTextureMipmap, GLuint texture )

#ifdef LINUX
#   define GL_FUNC_LIST_WIN32
#   define GL_FUNC_LIST_LINUX
#endif // LINUX

#ifdef WIN32
#   define GL_FUNC_LIST_LINUX
#   define GL_FUNC_LIST_WIN32 \
    GLEXT( void, ActiveTexture, GLenum texture ) \
    GLEXT( void, BlendEquation, GLenum mode ) \
    GLEXT( void, BlendFuncSeparate, GLenum dstRGB, GLenum dstAlpha, GLenum srcRGB, GLenum srcAlpha ) \
    GLEXT( void, BlendEquationSeparate, GLenum rgbMode, GLenum alphaMode ) \
    GLEXT( void, BufferSubData, GLenum target,GLintptr offset, GLsizeiptr size, const void* data) \
    GLEXT( GLint, GetAttribLocation, GLuint program, const GLchar *name) \
    GLEXT( const GLubyteptr, GetStringi, GLenum value, GLuint index)

#endif // WIN32

/***************************************************************/
/* Function declarations                                       */
/***************************************************************/
#define GLEXT(ret, name, ...) \
typedef ret GLDECL name##proc(__VA_ARGS__); \
extern name##proc * gl##name;

#ifdef WIN32
GL_FUNC_LIST_WIN32
#endif

#ifdef LINUX
GL_FUNC_LIST_LINUX
#endif

GL_FUNC_LIST_SHARED

#undef GLEXT