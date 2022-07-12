#pragma once

#include "define.hpp"

#include "NotOSUObjectSprite.hpp"
#include "Resource.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "df2.hpp"
#include "ResourcePile.hpp"

#include <unordered_map>
#include <filesystem>

NS_BEGIN

class Skin: public detail::Resource
{
public:
    Skin() = default;

    bool load(const std::filesystem::path &path, Resources *res);
    bool create();

    FPS_t getAnimationFramerate(const std::string &object) const;
    TextureP getTexture(const std::string &object) const;
    color getTint(const std::string &object, unsigned int seed = 0) const;

    ShaderP getShader(const std::string &object) const;

    NotOSUObjectSprite createObjectSprite(const std::string &object,
                                          unsigned int objectSeed = 0,
                                          unsigned int comboSeed = 0,
                                          unsigned int mapSeed = 0) const;

private:
    bool load([[maybe_unused]] const std::filesystem::path &path) override
    { return false; }

    std::string directory;
    struct TextureInfo
    {
        std::string path;
        TextureP texture;
        std::vector<color8> tints = {WHITE};
        FPS_t animationFPS = DEFAULT_TEXTURE_FPS;
    };
    struct ShaderInfo
    {
        std::string path;
        ShaderP shader;
    };

    std::unordered_map<std::string, TextureInfo> textures;
    std::unordered_map<std::string, ShaderInfo> shaders;
};

template<>
void detail::ResourcePile<Skin>::loadOne(const std::string &name, const std::filesystem::path &path);

using SkinP = std::shared_ptr<Skin>;

NS_END