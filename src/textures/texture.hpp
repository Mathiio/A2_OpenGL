#pragma once

#include <string>
#include <unordered_map>
#include "glimac/common.hpp"
#include "p6/p6.h"


class Texture {
public:
    static Texture& instance() {
        static Texture instance;
        return instance;
    }

    GLuint loadTexture(const std::string& filename);
    void bindTexture(GLuint textureId) const;
    void deleteTexture(GLuint textureId);

private:
    std::unordered_map<std::string, GLuint> m_textureCache;
};