//
// Created by sijh on 10.04.22.
//
#pragma once

#include "define.hpp"
#include "Color.hpp"
#include "Rect.hpp"

#include <memory>

namespace GAME_TITLE {

    class Image {
    public:
        Image();

        bool load(const std::string &path);

        bool resize(int width, int height);

        void clear(color col = WHITE);

        void setPixel(uvec2d pos, color8 col);

        void setRectArea(irect area, color col);

        [[nodiscard]] int getWidth() const;

        [[nodiscard]] int getHeight() const;

        [[nodiscard]] int getChannels() const;

        [[nodiscard]] isize getResolution() const;

        [[nodiscard]] color8 *getPixels();

    private:
        static void deleteImage(color8 *);

        std::shared_ptr<color8> data;
        int width{-1}, height{-1}, channels{0};
    };

}
