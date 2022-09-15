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

#include "Skin.hpp"

#include <utility>

#include "Random.hpp"
#include "Resources.hpp"
#include "Util.hpp"
#include "Context.hpp"

NS_BEGIN


const std::map<std::string, std::pair<std::string,std::string>> Skin::StaticGameShaders = {
	{SLIDER_SHADER,
	 {
		// VERTEX SHADER
		std::string("#version 330 core\n"
		"layout (location=0) in vec2 aPos;"
		
		"uniform mat3 camera;"
		"uniform mat3 transform;"
		"uniform mat3 shape;"
		
		"void main()"
		"{"
		    "gl_Position = vec4(camera * transform * shape * vec3(aPos, 1.0f), 1.f);"
		"}"),

		 // FRAGMENT SHADER
		std::string(
		"#version 330 core\n"
		
		"layout(pixel_center_integer) in vec4 gl_FragCoord;"
		
		"uniform sampler2D shadeTexture;"
		
		"uniform vec2 resolution;"
		"uniform vec2 A;"
		"uniform vec2 B;"
		"uniform float thickness;"
		"uniform vec4 fill;"
		
		"uniform mat3 camera;"
		"uniform mat3 transform;"
		
		"out vec4 FragColor;"
		
		"vec2 normal(vec2 inputVector) {"
		    "return vec2(inputVector.y, -inputVector.x);"
		"}"
		
		"float aspectRatio = resolution.x / resolution.y;"
		
		"float findDistance(vec2 p1, vec2 p2, float thickness) {"
		
		    "vec2 uv = gl_FragCoord.xy / resolution.xy;"
		    "uv *= 2.0f;"
		    "uv -= 1.0f;"
		    "uv.x *= aspectRatio;"
		
		    "vec2 normalVector = normal(p1 - p2);"
		    "vec2 intersectNormal = p1 - p2;"
		
		    "float a1 = normalVector.x;"
		    "float b1 = normalVector.y;"
		    "float c1 = -(a1*p1.x + b1 * p1.y);"
		
		    "float a2 = intersectNormal.x;"
		    "float b2 = intersectNormal.y;"
		    "float c2 = -(a2*uv.x + b2 * uv.y);"
		
		    "vec2 intersect;"
		
		    "intersect.x = (b1*c2 - b2*c1) / (a1*b2 - a2*b1);"
		    "intersect.y = (a2*c1 - a1*c2) / (a1*b2 - a2*b1);"

			"const float margin = 0.000001f;"
		
		    "float smallerX = min(p1.x, p2.x) - margin;"
		    "float smallerY = min(p1.y, p2.y) - margin;"
		    "float biggerX = max(p1.x, p2.x) + margin;"
		    "float biggerY = max(p1.y, p2.y) + margin;"
		
		    "float factor = 1.0f;"
		
		    "if (((smallerX <= intersect.x) && (intersect.x <= biggerX)) &&"
		    "((smallerY <= intersect.y) && (intersect.y <= biggerY))) {"
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
		
		    "if (factor > thickness)"
		        "return -1.0f;"
		    "return (factor / thickness);"
		"}"
		
		"void main()"
		"{"
		    "vec2 tA = vec3(transform * vec3(A, 1.0f)).xy;"
		    "vec2 tB = vec3(transform * vec3(B, 1.0f)).xy;"
		    "float factor = findDistance(tA, tB, thickness);"
		    "if (factor < 0.0f) {"
		        "discard;"
			"}"
			"gl_FragDepth = factor;"
		    "gl_FragColor = texture(shadeTexture, vec2(factor, 0.0f)) * fill;"
		"}"
		)
	 }}
};

template<>
Resource<Skin> Load(const std::filesystem::path &path)
{
    LOG_ENTER();

	auto& res = GetContext().resources;

	Resource<Skin> r;

    r->directory = path.parent_path();

    df2 settings = df2::read(path);

	int failed = 0;

    for (const auto &entry: settings["textures"]) {
        auto &object = r->textures[entry.first];
        // apply settings
        const auto &fields = entry.second;

        std::filesystem::path texPath = GetContext().resources.findFile(entry.first, r->directory);
        if (fields.find("texturePath") != fields.end()) {
            texPath = r->directory / fields["texturePath"].str(entry.first);
        }
		object.path = texPath.string();

        if (fields.find("fps") != fields.end())
            object.animationFPS = fields["fps"].integer();

		if (fields.find("frames") != fields.end())
			object.animationFrames = fields["frames"].integer();

        if (fields.find("tints") != fields.end()) {
            object.tints.clear();
            const auto &tints = fields["tints"];
            for (const auto &tint: tints) {
                object.tints.push_back(tint.second.col());
            }
        }

        log::info("Added object texture override ", entry.first, " (texture: ", texPath, ", @",
                  object.animationFPS, " FPS, ", object.tints.size(), " tint(s)).");
    }

    log::info("Loading skin assets...");

    for (auto &texture: r->textures) {
        texture.second.texture = res.load<Texture>(texture.second.path, r->directory);
		failed += !bool(texture.second.texture);
    }
    for (const auto& shader : Skin::StaticGameShaders) {
		const auto& vertex = shader.second.first;
		const auto& fragment = shader.second.second;
		const auto& shaderObj = r->shaders[shader.first].shader;
		failed += !bool(shaderObj->fromString(vertex, fragment));
    }
	for (auto &sound: r->sounds) {
		sound.second.sound = res.load<SoundSample>(sound.second.path, r->directory);
		failed += !bool(sound.second.sound);
	}

    log::info("Loaded skin ", path);
	if (failed != 0) {
		log::info("Failed to load ", failed, " assets");
	}
	return r;
}

FPS_t Skin::getAnimationFramerate(const std::string &object) const
{
    LOG_ENTER();

    if (textures.contains(object))
        return textures.at(object).animationFPS;

    return DEFAULT_TEXTURE_FPS;
}

int Skin::getFrameCount(const std::string &object) const
{
	LOG_ENTER();

	if (textures.contains(object))
		return textures.at(object).animationFrames;

	return -1;
}

Resource<Texture> Skin::getTexture(const std::string &object) const
{
    LOG_ENTER();

    if (textures.contains(object)) {
		return textures.at(object).texture;
	}

	return GetContext().resources.get<Texture>(object, directory, false);
}

color Skin::getTint(const std::string &object, unsigned int seed) const
{
    LOG_ENTER();

    if (!textures.contains(object))
        return WHITE;

    const auto &tints = textures.at(object).tints;

    if (tints.empty())
        return WHITE;

    std::srand(seed);
    auto i = static_cast<unsigned int>(std::rand()) % tints.size();

    return tints.at(i);
}

Resource<Shader> Skin::getShader(const std::string &object) const
{
    LOG_ENTER();

    if (shaders.contains(object)) {
        return shaders.at(object).shader;
    }
	return GetContext().resources.get<Shader>(object, directory, false);
}

ObjectSprite Skin::createObjectSprite(const std::string &object,
									  const HitObjectArguments& args) const
{
    LOG_ENTER();

    ObjectSprite ret;

	ret.setTexture(getTexture(object));
	ret.setFPS(getAnimationFramerate(object));
	ret.setTint(getTint(object, args.objectSeed));
	ret.setTotalFrames(getFrameCount(object));

    return ret;
}

Resource<SoundSample> Skin::getSound(const std::string &object)
{
	if (sounds.contains(object)) {
		return sounds.at(object).sound;
	}
	return GetContext().resources.get<SoundSample>(object, directory, false);
}

NS_END