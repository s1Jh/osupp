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

#include "ObjectSprite.hpp"
#include "Resource.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "df2.hpp"
#include "ResourcePile.hpp"
#include "SoundSample.hpp"
#include "HitObjectArguments.hpp"

#include <unordered_map>
#include <filesystem>

NS_BEGIN

class Skin
{
	friend Resource<Skin> Load<Skin>(const std::filesystem::path &path);
public:
    enum class ColourAssignmentMode: uint8_t
    {
        OBJECT = 0 << 0,        // bit 0 and 1
        COMBO = 1 << 0,
        MAP = 2 << 0,
        RANDOM = 0 << 2,        // bit 2
        SEQUENTIAL = 1 << 2,
    };

    Skin() = default;

    FPS_t getAnimationFramerate(const std::string &object) const;
    Resource<Texture> getTexture(const std::string &object) const;
    color getTint(const std::string &object, unsigned int seed = 0) const;
	int getFrameCount(const std::string &object) const;

	Resource<Shader> getShader(const std::string &object) const;

	Resource<SoundSample> getSound(const std::string &object);

    ObjectSprite createObjectSprite(const std::string &object,
                                    const HitObjectArguments& args) const;

private:

    std::filesystem::path directory;
    struct TextureInfo
    {
        std::string path;
		Resource<Texture> texture;
        std::vector<color8> tints = {WHITE};
        FPS_t animationFPS = DEFAULT_TEXTURE_FPS;
		int animationFrames = -1;
    };
    struct ShaderInfo
    {
        std::string path;
		Resource<Shader> shader;
    };
	struct SoundInfo
	{
		std::string path;
		Resource<SoundSample> sound;
	};

    std::unordered_map<std::string, TextureInfo> textures;
    std::unordered_map<std::string, ShaderInfo> shaders;
	std::unordered_map<std::string, SoundInfo> sounds;
};

template<>
Resource<Skin> Load(const std::filesystem::path &path);

NS_END