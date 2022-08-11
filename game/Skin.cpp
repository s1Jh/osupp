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

bool Skin::load(const std::filesystem::path &path)
{
    LOG_ENTER();

	auto& res = GetContext().resources;

    bool success = true;
    success &= create();

    if (!success) {
        log::error("Failed to create skin objects (one or more was null)");
        return false;
    }

    directory = path.parent_path();

    df2 settings = df2::read(path);

    for (const auto &entry: settings["textures"]) {
        auto &object = textures[entry.first];
        // apply settings
        const auto &fields = entry.second;

        std::filesystem::path texPath = GetContext().resources.findFile(entry.first, directory);
        if (fields.find("texturePath") != fields.end()) {
            texPath = directory / fields["texturePath"].str(entry.first);
        }
		object.path = texPath.string();

        if (fields.find("fps") != fields.end())
            object.animationFPS = fields["fps"].integer();

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

    for (auto &texture: textures) {
        texture.second.texture = res.load<Texture>(texture.second.path, directory);
        success &= bool(texture.second.texture);
    }
    for (auto &shader: shaders) {
        shader.second.shader = res.load<Shader>(shader.second.path, directory);
        success &= bool(shader.second.shader);
    }
	for (auto &sound: sounds) {
		sound.second.sound = res.load<SoundSample>(sound.second.path, directory);
		success &= bool(sound.second.sound);
	}

    log::info("Loaded skin ", path);
    return success;
}

bool Skin::create()
{
    LOG_ENTER();

    bool success = true;

    textures.clear();
    shaders.clear();

    return success;
}

FPS_t Skin::getAnimationFramerate(const std::string &object) const
{
    LOG_ENTER();

    if (textures.contains(object))
        return textures.at(object).animationFPS;

    return DEFAULT_TEXTURE_FPS;
}

TextureP Skin::getTexture(const std::string &object) const
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

ShaderP Skin::getShader(const std::string &object) const
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

    return ret;
}

SoundSampleP Skin::getSound(const std::string &object)
{
	if (sounds.contains(object)) {
		return sounds.at(object).sound;
	}
	return GetContext().resources.get<SoundSample>(object, directory, false);
}

NS_END