#include "Skin.hpp"

#include <utility>

#include "Random.hpp"
#include "Resources.hpp"
#include "Util.hpp"

NS_BEGIN

bool Skin::load(const std::filesystem::path &path, Resources *res)
{
    LOG_ENTER();

    bool success = true;
    success &= create();

    if (!success) {
        log::error("Failed to create skin objects (one or more was null)");
        return false;
    }

    directory = path.parent_path();

    df2 settings = df2::read(path);

    for (const auto &entry: settings["textures"]) {
        if (!textures.contains(entry.first)) {
            log::warning("Attempted to style unknown element: \"", entry.first, '\"');
            continue;
        }

        auto &object = textures[entry.first];
        // apply settings
        const auto &fields = entry.second;

        std::filesystem::path texPath = directory;
        if (fields.find("texturePath") != fields.end()) {
            texPath /= fields["texturePath"].str(entry.first);
            object.path = texPath.string();
        }

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
        texture.second.texture = res->load<Texture>(texture.second.path, directory);
        success &= bool(texture.second.texture);
    }
    for (auto &shader: shaders) {
        shader.second.shader = res->load<Shader>(shader.second.path, directory);
        success &= bool(shader.second.shader);
    }

    log::info("Loaded skin ", path);
    return success;
}

bool Skin::create()
{
    LOG_ENTER();

    bool success = true;

    // Try to load from the default skin, which should always be present.
    // Always make sure to load to reload the texture.
    auto loadTexture = [&](const std::string &path) -> bool
    {
        TextureP ptr = std::make_unique<Texture>();
        ptr->create();
        textures[path] = {path, ptr};
        return bool(ptr);
    };

    auto loadShader = [&](const std::string &path) -> bool
    {
        ShaderP ptr = std::make_unique<Shader>();
        ptr->create();
        shaders[path] = {path, ptr};
        return bool(ptr);
    };

    textures.clear();
    shaders.clear();

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
    LOG_ENTER();

    if (textures.contains(object))
        return textures.at(object).animationFPS;

    return DEFAULT_TEXTURE_FPS;
}

TextureP Skin::getTexture(const std::string &object) const
{
    LOG_ENTER();

    if (textures.contains(object))
        return textures.at(object).texture;

    return nullptr;
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
    return nullptr;
}

ObjectSprite Skin::createObjectSprite(const std::string &object,
                                      unsigned int objectSeed,
                                      unsigned int comboSeed,
                                      unsigned int mapSeed) const
{
    LOG_ENTER();

    ObjectSprite ret;

    if (textures.contains(object)) {
        ret.setFPS(textures.at(object).animationFPS);
        ret.setTexture(textures.at(object).texture);
        ret.setTint(getTint(object, objectSeed));
    }
    return ret;
}

// FIXME: a hack to pass Resources reference to the Skin class which is needed whilst loading skins
template<>
void detail::ResourcePile<Skin>::loadOne(const std::string &name, const std::filesystem::path &path)
{
    log::info("Custom loader called!");
    StorageT object = std::make_shared<Skin>(*null);
    loadedAssets[name] = object;

    if (!object->load(path, &resourceRef)) {
        log::error("Failed to load ", typeid(Skin).name(), ' ', path);
    }
}

NS_END