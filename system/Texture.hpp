#pragma once

#include "Image.hpp"
#include "Resource.hpp"
#include "Vec2.hpp"
#include "define.hpp"

#include <memory>
#include <filesystem>

NS_BEGIN

class Texture: public detail::Resource
{
public:
    Texture();

    bool load(const std::filesystem::path &location) override;

    bool create() override;

    bool setImage(Image &img);

    [[nodiscard]] int getWidth() const;

    [[nodiscard]] int getHeight() const;

    [[nodiscard]] isize getSize() const;

    [[nodiscard]] int getChannels() const;

    void use(unsigned int index) const;

    void setClipArea(const frect &rect);
    [[nodiscard]] const Mat3f &getUVTransform() const;

    static void unbind(unsigned int index);

private:
    static void GLTexDeleter(unsigned int *ptr);

    Mat3f clip;
    std::shared_ptr<unsigned int> glTexture;
    Image *img;
    uint8_t channels;
    isize pixelSize;
};

using TextureP = std::shared_ptr<Texture>;

NS_END