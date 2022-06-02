#include "Texture.hpp"

#include <GL/glew.h>
#include "Util.hpp"

namespace GAME_TITLE {

    Texture::Texture() {
        img = nullptr;
        glTexture = nullptr;
    }

    Texture::Texture(const std::string &location) {
        glTexture = nullptr;
        img = nullptr;
        load(location);
    }

    bool Texture::load(const std::string &location) {
        Image texImg;
        if (!texImg.load(location)) {
            return false;
        }
        return setImage(texImg);
    }

    void Texture::use(unsigned int index) const {
        if (index > 31) {
            log::warning("Tried to set texture beyond index");
            return;
        }
        if (glTexture) {
            glActiveTexture(GL_TEXTURE0 + index);
            glBindTexture(GL_TEXTURE_2D, *glTexture);
        }
    }

    void Texture::unbind(unsigned int index) {
        if (index > 31) {
            log::warning("Tried to set texture beyond index");
            return;
        }

        glActiveTexture(GL_TEXTURE0 + index);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    int Texture::getWidth() const {
        if (img) return img->getWidth();
        return 0;
    }

    int Texture::getHeight() const {
        if (img) return img->getHeight();
        return 0;
    }

    uvec2d Texture::getResolution() const {
        if (img) return img->getResolution();
        return {0, 0};
    }

    int Texture::getChannels() const {
        if (img) return img->getChannels();
        return 0;
    }

    void Texture::GLTexDeleter(unsigned int *ptr) {
        log::debug("Deleting ", *ptr);
        glDeleteTextures(1, ptr);
    }

    bool Texture::setImage(Image &imgIn) {
        glTexture = std::shared_ptr<unsigned int>(new unsigned int, GLTexDeleter);
        auto pixels = imgIn.getPixels();

        glGenTextures(1, glTexture.get());
        CheckGLh("Generated texture");
        glBindTexture(GL_TEXTURE_2D, (*glTexture));
        CheckGLh("Bound texture");

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        CheckGLh("Set texture params");

        // you take GL_RGB and then add channels - 3 (those are the R, G and B channels)
        glTexImage2D(GL_TEXTURE_2D, 0,
                     GL_RGB + (imgIn.getChannels() - 3), imgIn.getWidth(),
                     imgIn.getHeight(), 0, GL_RGB + (imgIn.getChannels() - 3),
                     GL_UNSIGNED_BYTE, (uint8_t *) pixels
        );
        CheckGLh("Uploaded texture data");
        glGenerateMipmap(GL_TEXTURE_2D);
        CheckGLh("Generated mipmaps");
        glBindTexture(GL_TEXTURE_2D, 0);

        img = &imgIn;

        return true;
    }
}