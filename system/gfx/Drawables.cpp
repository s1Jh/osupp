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

#include "Drawables.hpp"

#include "define.hpp"

#include "Math.hpp"
#include "Renderer.dpp"
#include "Generics.hpp"
#include "GL.hpp"

NS_BEGIN

constexpr const char *STATIC_SHAPE_SHADER_VERT =
	"#version 330 core\n"
	"layout (location = 0) in vec2 aPos;"
	"layout (location = 1) in vec2 aUV;"
	""
	"uniform mat3 transform;"
	"uniform mat3 shape;"
	"uniform mat3 camera;"
	"uniform float z;"
	""
	"out vec2 UV;"
	""
	"void main()"
	"{"
	    "UV = aUV;"
	    "gl_Position = vec4(camera * transform * shape * vec3(aPos, 1.0f), 1.f);\n"
	"}";
constexpr const char *STATIC_SHAPE_SHADER_FRAG =
	"#version 330 core\n"
	""
	"uniform vec4 fill;"
	"uniform bool useTexture;"
	"uniform int blendMode;"
	"uniform mat3 textureTransform;"
	"uniform sampler2D fillTexture;"
	""
	"in vec2 UV;"
	""
	"out vec4 FragColor;"
	""
	"void main()"
	"{"
	    "if (!useTexture)"
	    "{"
	        "FragColor = fill;"
	        "return;"
	    "}"
	""
	    "FragColor = texture(fillTexture, (textureTransform * vec3(UV, 1.0)).xy);\n"
	""
	    "switch (blendMode)"
	    "{"
	        "case 0: break;"
	        "case 1: FragColor *= fill; break;"
	        "case 2: FragColor += fill; break;"
	        "case 3: FragColor -= fill; break;"
	    "}"
	"}";

constexpr const char *LINE_SHADER_VERT =
	"#version 330 core\n"
	""
	"layout (location=0) in vec2 aPos;"
	""
	"uniform mat3 camera;"
	"uniform mat3 transform;"
	"uniform mat3 shape;"
	""
	"uniform float z;"
	""
	"void main()"
	"{"
	    "gl_Position = vec4(camera * transform * shape * vec3(aPos, 1.0f), 1.f);"
	"}";

constexpr const char *LINE_SHADER_FRAG =
	"#version 330 core\n"
	""
	"layout(pixel_center_integer) in vec4 gl_FragCoord;"
	""
	"uniform vec2 resolution;"
	"uniform vec2 A;"
	"uniform vec2 B;"
	"uniform mat3 camera;"
	"uniform mat3 transform;"
	"uniform float thickness;"
	"uniform vec4 fill;"
	""
	"out vec4 FragColor;"
	""
	"vec2 normal(vec2 inputVector) {"
		"return vec2(inputVector.y, -inputVector.x);"
	"}"
	""
	"float aspectRatio = resolution.x / resolution.y;"
	""
	"float drawLine(vec2 p1, vec2 p2, float thickness) {"
	""	// our fragment positioned in normalized space
	    "vec2 uv = gl_FragCoord.xy / resolution.xy;"
	    "uv *= 2.0f;"
	    "uv -= 1.0f;"
	    "uv.x *= aspectRatio;"
	""
	""   // we need the normal vector to calculate the distance of this point to the drawn line
	    "vec2 normalVector = normal(p1 - p2);"
	""   // the intersection line is normal to the original drawn line
	    "vec2 intersectNormal = p1 - p2;"
	""
	""   // drawn line
	    "float a1 = normalVector.x;"
	    "float b1 = normalVector.y;"
	    "float c1 = -(a1*p1.x + b1 * p1.y);"
	""
	    "float a2 = intersectNormal.x;"
	    "float b2 = intersectNormal.y;"
	    "float c2 = -(a2*uv.x + b2 * uv.y);"
	""
	    "vec2 intersect;"
	""
	    "intersect.x = (b1*c2 - b2*c1) / (a1*b2 - a2*b1);"
	    "intersect.y = (a2*c1 - a1*c2) / (a1*b2 - a2*b1);"
	""
		"const float margin = 0.000001f;"
	""
	    "float smallerX = min(p1.x, p2.x) - margin;"
	    "float smallerY = min(p1.y, p2.y) - margin;"
	    "float biggerX = max(p1.x, p2.x) + margin;"
	    "float biggerY = max(p1.y, p2.y) + margin;"
	""
	    "float factor = 1.0f;"
	""
	    "if (((smallerX <= intersect.x) && (intersect.x <= biggerX)) &&"
	        "((smallerY <= intersect.y) && (intersect.y <= biggerY))) {"
	""   	// IN RANGE
	        "factor = abs(a1 * uv.x + b1 * uv.y + c1) / sqrt(pow(a1, 2) + pow(b1, 2));"
	    "} else {"
	        "if (intersect.x < smallerX) {"
	            "if (intersect.y < smallerY) {"
	                "factor = distance(uv, vec2(smallerX, smallerY));"
	            "} else {"
	                "factor = distance(uv, vec2(smallerX, biggerY));"
	            "}"
	        "} else {"
	            "if (intersect.y < smallerY) {"
	                "factor = distance(uv, vec2(biggerX, smallerY));"
	            "} else {"
	                "factor = distance(uv, vec2(biggerX, biggerY));"
	            "}"
	        "}"
	    "}"
	""
	    "if (factor > thickness)"
	        "return -1.0f;"
	    "return factor;"
	"}"
	""
	"void main()"
	"{"
	    "vec2 tA = vec3(transform * vec3(A, 1.0f)).xy;"
	    "vec2 tB = vec3(transform * vec3(B, 1.0f)).xy;"
	    "float factor = drawLine(tA, tB, thickness);"
	    "if (factor < 0.0f)"
	        "discard;"
	    "gl_FragColor = fill * mix(1.0, 0.0, smoothstep(0.5 * thickness, 1.5 * thickness, factor));"
	"}";


Transform2D::operator Mat3f() const
{
    return math::MakeTranslationMatrix(translate) *
        math::MakeRotationMatrix<float>(rotate, rotationCenter) *
        math::MakeScaleMatrix(scale) * math::MakeShearMatrix(shear);
}

void detail::DrawGeneric2DShape(const Renderer &renderer,
                                const Mesh &mesh, const Mat3f &shape,
                                const VisualAppearance &appearance,
                                const Mat3f &transform, RenderMode mode)
{
	static bool wasInit;
	static Shader shader;
	if (!wasInit) {
   		shader.fromString(STATIC_SHAPE_SHADER_VERT, STATIC_SHAPE_SHADER_FRAG);
		wasInit = true;
	}

    shader.use();
    shader.set("fill", appearance.fillColor);
    shader.set("outline", appearance.outlineColor);
    shader.set("outline_width", appearance.outlineWidth);
    shader.set("blendMode", (int) appearance.blendMode);
    shader.set("z", appearance.zIndex);

    shader.set("transform", transform);
    shader.set("camera", renderer.camera.getMatrix());
    CheckGLh("Set shader matrices");

    shader.set("shape", shape);
    CheckGLh("Set shader shape matrix");

    if (appearance.texture) {
        appearance.texture->use(0);
        shader.set("useTexture", true);
    }
    else {
        shader.set("useTexture", false);
    }
    CheckGLh("set texture");

    if (appearance.uvTransform) {
        shader.set("textureTransform", *appearance.uvTransform);
    }
    else if (appearance.texture) {
        shader.set("textureTransform", appearance.texture->getUVTransform());
    }
    else {
        shader.set("textureTransform", MAT3_NO_TRANSFORM<float>);
    }
    CheckGLh("set texture UV transform");

    glBindVertexArray(mesh.getVAO());
    glDrawElements(static_cast<unsigned int>(mode), mesh.getElementCount(),
                   GL_UNSIGNED_INT, nullptr);
    CheckGLh("draw");

    Shader::unbind();
    Texture::unbind(0);
    CheckGLh("Unbound");
}

// FIXME: Dirty hack to get a rectangle mesh for the DrawLineSegment function, should be rectified
//        once we move to render batching and using geometry shaders for creating geometry of primitives.
//        In the future, we would like to just throw the A and B points of the line segment into a VBO batch
//        and use the geometry shader to fill in the rest.

void detail::DrawLineSegment(const Renderer &renderer, const dline &seg,
                             const VisualAppearance &appearance, const Mat3f &transform)
{
	static bool wasInit;
	static Shader lineShader;
    if (!wasInit) {
		lineShader.fromString(LINE_SHADER_VERT, LINE_SHADER_FRAG);
        wasInit = true;
    }

	const auto& rectShape = GetGenericMeshes().rectMask;

    lineShader.use();
    CheckGLh("Set shader");

    lineShader.set("resolution", (fvec2d) renderer.getSize());
    lineShader.set("transform", transform);
    lineShader.set("camera", renderer.camera.getMatrix());
    CheckGLh("Set shader matrices");

	auto offset = appearance.outlineWidth;

    auto right = (float) math::Max(seg.A.x, seg.B.x) + offset;
    auto left = (float) math::Min(seg.A.x, seg.B.x) - offset;
    auto top = (float) math::Max(seg.A.y, seg.B.y) + offset;
    auto bottom = (float) math::Min(seg.A.y, seg.B.y) - offset;

    frect rect = {
        {
            left - right,
            top - bottom
        },
        {
            (left + right) / 2.0f,
            (top + bottom) / 2.0f,
        }
    };

    Mat3f shape = math::MakeScaleMatrix<float>(rect.size) *
        math::MakeTranslationMatrix<float>(rect.position);

    lineShader.set("z", appearance.zIndex);

    lineShader.set("shape", shape);
    CheckGLh("Set shader matrix");

    lineShader.set("thickness", appearance.outlineWidth);
    lineShader.set("fill", appearance.fillColor);
    lineShader.set("A", seg.A);
    lineShader.set("B", seg.B);
    CheckGLh("Set shader params");

    glBindVertexArray(rectShape.getVAO());
    glDrawElements(static_cast<unsigned int>(GL_TRIANGLES), rectShape.getElementCount(), GL_UNSIGNED_INT, nullptr);
    CheckGLh("draw");

    Shader::unbind();
    Texture::unbind(0);
    CheckGLh("Unbound");
}

NS_END
