#include "Resources.hpp"

#include "Util.hpp"

namespace GAME_TITLE {
    template<>
    const std::vector<std::string> ResourcePile<Texture>::persistentAssets = {
            NOTE_BASE_SPRITE,
            NOTE_OVERLAY_SPRITE,
            APPROACH_CIRCLE_SPRITE,
            SLIDER_HEAD_SPRITE,
            SLIDER_TAIL_SPRITE,
            SLIDER_HEAD_REPEAT_SPRITE,
            SLIDER_TAIL_REPEAT_SPRITE,
            SLIDER_BODY_SPRITE,
            SPINNER_SPRITE,
            SPINNER_CENTER_SPRITE
    };

    template<>
    const std::vector<std::string> ResourcePile<Shader>::persistentAssets = {
            SLIDER_SHADER
    };

    template<>
    const std::vector<std::string> ResourcePile<Mesh>::persistentAssets = {
    };

    template<>
    const std::vector<std::string> ResourcePile<MapInfo>::persistentAssets = {
    };

    int Resources::loadPersistentAssets() {
        int success = 0;

        success += textures.loadPersistent();
        success += shaders.loadPersistent();
        success += meshes.loadPersistent();

        return success;
    }

    void Resources::addSearchPath(const std::filesystem::path &path) {
        log::debug("Adding search path ", path.string());

        if (std::filesystem::exists(path)) {
            searchPaths.push_back(path);
            return;
        }
        log::warning("Failed to add search path, path doesn't exist");
    }

    void Resources::clearSearchPaths() {
        searchPaths.clear();
    }

    std::filesystem::path Resources::findFile(std::filesystem::path pathIn) {
        for (auto &path: searchPaths) {
            auto concatPath = path / pathIn.parent_path();

            for (auto &fileInPath: std::filesystem::directory_iterator(concatPath)) {
                auto fileName = fileInPath.path().stem().string();

                if (fileName == pathIn.stem().string()) {
                    return fileInPath.path();
                }
            }
        }

        return pathIn;
    }

    Resources::Resources() :
            textures(*this),
            shaders(*this),
            meshes(*this),
            maps(*this) {}

    template<>
    bool ResourcePile<Texture>::loadOne(const std::string &path) {
        Texture tex;
        if (!tex.load(resourceRef.findFile(path))) {
            log::error("Failed to load texture ", path);
            return false;
        }
        loadedAssets[path] = std::move(tex);
        return true;
    }

    template<>
    Texture ResourcePile<Texture>::makeDefault() {
        Texture tex;
        Image img;
        img.resize(128, 128);
        img.setRectArea({{64, 64},
                         {0,  0}}, BLACK);
        img.setRectArea({{64, 64},
                         {0,  64}}, PINK);
        img.setRectArea({{64, 64},
                         {64, 0}}, PINK);
        img.setRectArea({{64, 64},
                         {64, 64}}, BLACK);
        tex.setImage(img);
        return tex;
    }

    template<>
    bool ResourcePile<Shader>::loadOne(const std::string &path) {
        Shader shader;
        if (!shader.load(resourceRef.findFile(path))) {
            log::error("Failed to load shader ", path);
            return false;
        }
        loadedAssets[path] = shader;
        return true;
    }

    template<>
    Shader ResourcePile<Shader>::makeDefault() {
        Shader shader;
        shader.fromString(
                std::string("#version ") + GL_VERSION_STR + '\n' + GL_DEFAULT_VERTEX_SHADER,
                std::string("#version ") + GL_VERSION_STR + '\n' + GL_DEFAULT_FRAGMENT_SHADER
        );
        return shader;
    }

    template<>
    bool ResourcePile<Mesh>::loadOne(const std::string &path) {
        Mesh mesh;
        if (!mesh.load(resourceRef.findFile(path))) {
            log::error("Failed to load mesh ", path);
            return false;
        }
        loadedAssets[path] = mesh;
        return true;
    }

    template<>
    Mesh ResourcePile<Mesh>::makeDefault() {
        Mesh mesh;
        mesh.setAttributeDescriptors({AttributeType::Vec2});
        mesh.insertVertices({{-1.f, -1.f},
                             {0.f,  1.f},
                             {1.f,  -1.f}});
        mesh.insertIndices({0, 1, 2});
        return mesh;
    }

    template<>
    bool ResourcePile<MapInfo>::loadOne(const std::string &path) {
        MapInfo map;
        if (!map.loadMap(resourceRef.findFile(path), MAP_LOAD_ALL)) {
            log::error("Failed to load map ", path);
            return false;
        }
        loadedAssets[path] = map;
        return true;
    }

    template<>
    MapInfo ResourcePile<MapInfo>::makeDefault() {
        return MapInfo{};
    }
}