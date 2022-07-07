#include "Skin.hpp"

#include <utility>

#include "Random.hpp"
#include "Resources.hpp"
#include "Util.hpp"

NS_BEGIN

bool Skin::load(const std::string &path, Resources *res)
{
    create(res);

    std::string dfPath = path + "/skin.df";

    df2 settings = df2::read(dfPath);

    for (const auto &entry: settings["textures"]) {
        if (textures.contains(entry.first)) {
            // apply overrides
            auto &existingObject = textures.at(entry.first);
            const auto &fields = entry.second;

            if (fields.find("texturePath") != fields.end()) {
                auto texPath =
                    std::filesystem::path(path) / fields["texturePath"].str();
                existingObject.texture = res->textures.get(texPath.string());
            }

            if (fields.find("fps") != fields.end())
                existingObject.animationFPS = fields["fps"].integer();

            if (fields.find("tints") != fields.end()) {
                existingObject.tints.clear();
                const auto &tints = fields["tints"];
                for (const auto &tint: tints) {
                    color8 col(tint.second.integer());
                    existingObject.tints.push_back(col);
                }
            }
        }
    }

    log::info("Loaded skin ", dfPath);

    return true;
}

bool Skin::create(Resources *res)
{
    bool success = true;

    auto loadTexture = [&](const std::string &path) -> bool
    {
        auto ptr = res->textures.get(path);
        textures[path] = {ptr};
        return bool(ptr);
    };

    auto loadShader = [&](const std::string &path) -> bool
    {
        auto ptr = res->shaders.get(path);
        shaders[path] = {ptr};
        return bool(ptr);
    };

    success &= loadTexture(NOTE_BASE_SPRITE);
    success &= loadTexture(NOTE_OVERLAY_SPRITE);
    success &= loadTexture(NOTE_UNDERLAY_SPRITE);
    success &= loadTexture(APPROACH_CIRCLE_SPRITE);
    success &= loadTexture(SLIDER_HEAD_SPRITE);
    success &= loadTexture(SLIDER_TAIL_SPRITE);
    success &= loadTexture(SLIDER_HEAD_REPEAT_SPRITE);
    success &= loadTexture(SLIDER_TAIL_REPEAT_SPRITE);
    success &= loadTexture(SLIDER_BODY_SPRITE);
    success &= loadTexture(SLIDER_HIT_POINT_SPRITE);
    success &= loadTexture(SPINNER_SPRITE);
    success &= loadTexture(SPINNER_CENTER_SPRITE);
    success &= loadTexture(SPINNER_METER_SPRITE);
    success &= loadTexture(SLIDER_BALL_SPRITE);
    success &= loadTexture(PLAY_FIELD_SPRITE);

    success &= loadShader(SLIDER_SHADER);

    return success;
}

FPS_t Skin::getAnimationFramerate(const std::string &object) const
{
    if (textures.contains(object))
        return textures.at(object).animationFPS;

    return DEFAULT_TEXTURE_FPS;
}

TextureP Skin::getTexture(const std::string &object) const
{
    if (textures.contains(object))
        return textures.at(object).texture;

    return nullptr;
}

color Skin::getTint(const std::string &object, unsigned int seed) const
{
    if (!textures.contains(object))
        return WHITE;

    const auto &tints = textures.at(object).tints;

    if (tints.empty())
        return WHITE;

    std::srand(seed);
    auto i = std::rand() % tints.size();

    return tints.at(i);
}

ShaderP Skin::getShader(const std::string &object) const
{
    if (shaders.contains(object)) {
        return shaders.at(object);
    }
    return nullptr;
}

NotOSUObjectSprite Skin::createObjectSprite(const std::string &object,
                                            unsigned int objectSeed,
                                            unsigned int comboSeed,
                                            unsigned int mapSeed) const
{
    NotOSUObjectSprite ret;

    if (textures.contains(object)) {
        ret.setFPS(textures.at(object).animationFPS);
        ret.setTexture(textures.at(object).texture);
        ret.setTint(getTint(object, objectSeed));
    }
    return ret;
}

NS_END