//
// Created by sijh on 10.04.22.
//

#include "Image.hpp"

#define STB_IMAGE_IMPLEMENTATION

#include "stb/stb_image.h"

#include "Util.hpp"

namespace GAME_TITLE {
    Image::Image() {
        stbi_set_flip_vertically_on_load(true);
    }

    int Image::getWidth() const {
        return width;
    }

    int Image::getHeight() const {
        return height;
    }

    int Image::getChannels() const {
        return channels;
    }

    color8 *Image::getPixels() {
        return data.get();
    }

    bool Image::load(const std::string &path) {

        data.reset((color8 *) stbi_load(
                path.c_str(), &width, &height,
                &channels, STBI_rgb_alpha), deleteImage);

        if (!data) {
            return false;
        }

        log::info(
                "Loaded image ", path,
                " Dimensions: ", width, " x ", std::to_string(height),
                " Channels: ", channels
        );

        return true;
    }

    isize Image::getResolution() const {
        return NotOSU::isize{width, height};
    }

    bool Image::resize(int widthIn, int heightIn) {
        auto mem = (color8 *) std::malloc(widthIn * heightIn * sizeof(color8));
        if (mem) {
            width = widthIn;
            height = heightIn;
            channels = 4;
            data.reset(mem, deleteImage);
            clear(WHITE);
            return true;
        }
        return false;
    }

    void Image::clear(color col) {
        for (int i = 0; i < width * getHeight(); i++) {
            data.get()[i] = col;
        }
    }

    void Image::setPixel(uvec2d pos, color8 col) {
        data.get()[(pos.y % height) * width + (pos.x % width)] = col;
    }

    void Image::setRectArea(irect area, color col) {
        for (int x = area.position.x; x < area.position.x + area.size.w; x++)
            for (int y = area.position.y; y < area.position.y + area.size.h; y++) {
                setPixel(isize{x, y}, col);
            }
    }

    void Image::deleteImage(color8 *ptr) {
        stbi_image_free(ptr);
    }

}