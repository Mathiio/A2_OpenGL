#pragma once

#include <glimac/common.hpp>
#include "glm/fwd.hpp"
#include "p6/p6.h"
#include "textures/texture.hpp"

class Mesh {
protected:
    GLuint m_vbo;
    GLuint m_vao;
    glm::vec3 m_pos;
    glm::vec3 m_scale;
    std::vector<glimac::ShapeVertex> m_vertices;
    std::vector<int>                 m_index;
    GLsizei                          m_vertexCount;
    float                            m_Rotation;
    GLuint m_texture;

public:
    Mesh(const std::string& modelMesh, const std::string& modelTexture)
    {
        loadModel(modelMesh);
        m_texture = Texture::instance().loadTexture(modelTexture);
    }

    ~Mesh()
    {
        glDeleteBuffers(1, &m_vbo);
        glDeleteVertexArrays(1, &m_vao);
    };
    
    glm::vec3 getPos() const { return m_pos; }
    glm::vec3 getScale() const { return m_scale; }
    GLuint getTexture() const { return m_texture; }
    void setPos(const glm::vec3& pos) { m_pos = pos; }
    void setScale(const glm::vec3& scale) { m_scale = scale; }

    void drawArray(GLuint textName);
    void setRotation(float angle);

    void draw(glm::vec3 pos, glm::vec3 scale, glm::mat4 ProjMatrix, glm::mat4 viewMatrix, GLuint uMVPMatrixLocation, GLuint uMVMatrixLocation, GLuint uNormalMatrixLocation, float angle);

    void setVao();
    void setVbo();
    void setBuffers();

    std::vector<glimac::ShapeVertex> getData() const;

    const glimac::ShapeVertex* getDataPointer() const;

    GLsizei getVertexCount() const { return m_vertexCount; }

    const int* getIndexPointer() const;
    void       loadModel(const std::string& fileName);
};