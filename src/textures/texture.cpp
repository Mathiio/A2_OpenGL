#include "textures/texture.hpp"
#include "glimac/common.hpp"
#include "p6/p6.h"



GLuint Texture::loadTexture(const std::string& filename) {
    auto it = m_textureCache.find(filename);
    if (it != m_textureCache.end()) {
        return it->second;
    }

    img::Image image = p6::load_image_buffer(filename);

    GLuint textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width(), image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    m_textureCache[filename] = textureId;

    return textureId;
}

void Texture::bindTexture(GLuint textureId) const {
    glBindTexture(GL_TEXTURE_2D, textureId);
}

void Texture::deleteTexture(GLuint textureId) {
    glDeleteTextures(1, &textureId);
}
