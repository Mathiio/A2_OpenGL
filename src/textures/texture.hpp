#pragma once

#include <string>
#include <unordered_map>
#include "glimac/common.hpp"
#include "p6/p6.h"

class Texture {
public:
    static Texture& instance()
    {
        static Texture instance;
        return instance;
    }

    GLuint loadTexture(const std::string& filename)
    {
        std::string const inputfile = "assets/textures/" + filename;
        auto        it        = m_textureCache.find(inputfile);
        if (it != m_textureCache.end())
        {
            return it->second;
        }

        img::Image image = p6::load_image_buffer(inputfile);

        GLuint textureId;
        glGenTextures(1, &textureId);
        glBindTexture(GL_TEXTURE_2D, textureId);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width(), image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.data());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);

        m_textureCache[inputfile] = textureId;

        return textureId;
    }

    void bindTexture(GLuint textureId) const
    {
        glBindTexture(GL_TEXTURE_2D, textureId);
    }

    void deleteTexture(GLuint textureId)
    {
        glDeleteTextures(1, &textureId);
    }

private:
    std::unordered_map<std::string, GLuint> m_textureCache;
};