#pragma once

#include "define.hpp"

#include "NotOSUObjectSprite.hpp"
#include "Resource.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "df2.hpp"

#include <unordered_map>

NS_BEGIN

class Skin: public detail::Resource
{
public:
    Skin() = default;

    bool load(const std::string &path, Resources *res = nullptr) override;
    bool create(Resources *res = nullptr) override;

    FPS_t getAnimationFramerate(const std::string &object) const;
    TextureP getTexture(const std::string &object) const;
    color getTint(const std::string &object, unsigned int seed = 0) const;

    ShaderP getShader(const std::string &object) const;

    NotOSUObjectSprite createObjectSprite(const std::string &object,
                                          unsigned int objectSeed = 0,
                                          unsigned int comboSeed = 0,
                                          unsigned int mapSeed = 0) const;

private:
    struct TextureInfo
    {
        TextureP texture;
        std::vector<color8> tints = {WHITE};
        FPS_t animationFPS = DEFAULT_TEXTURE_FPS;
    };

    std::unordered_map<std::string, TextureInfo> textures;
    std::unordered_map<std::string, ShaderP> shaders;
};

using SkinP = std::shared_ptr<Skin>;

NS_END