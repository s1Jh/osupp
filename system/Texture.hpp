#pragma once

#include "Vec2.hpp"
#include "define.hpp"
#include "Image.hpp"

#include <memory>

NS_BEGIN

class Texture {
public:
    Texture();

    explicit Texture(const std::string &location);

    bool load(const std::string &location);

    bool setImage(Image &img);

    [[nodiscard]] int getWidth() const;

    [[nodiscard]] int getHeight() const;

    [[nodiscard]] uvec2d getResolution() const;

    [[nodiscard]] int getChannels() const;

    void use(unsigned int index) const;

    static void unbind(unsigned int index);

private:
    static void GLTexDeleter(unsigned int *ptr);

    std::shared_ptr<unsigned int> glTexture;
    Image *img;
};

NS_END