#pragma once

#include <glimac/common.hpp>
#include "glm/fwd.hpp"
#include "p6/p6.h"

class Mesh {
protected:
    GLuint m_vbo;
    GLuint m_vao;
    // rendering::Texture m_texture;
    std::vector<glimac::ShapeVertex> m_vertices;
    std::vector<int>                 m_index;
    GLsizei                          m_vertexCount;
    float                            m_Rotation;

public:
    Mesh() = default;

    ~Mesh()
    {
        glDeleteBuffers(1, &m_vbo);
        glDeleteVertexArrays(1, &m_vao);
    };

    void drawArray(GLuint textName);
    void setRotation(float angle);

    void draw(glm::vec3 pos, glm::vec3 scale, glm::mat4 ProjMatrix, glm::mat4 viewMatrix, GLuint uMVPMatrixLocation, GLuint uMVMatrixLocation, GLuint uNormalMatrixLocation, GLuint textName);

    void setVao();

    void setVbo();

    void setBuffers();

    std::vector<glimac::ShapeVertex> getData() const;

    const glimac::ShapeVertex* getDataPointer() const;

    GLsizei getVertexCount() const
    {
        return m_vertexCount;
    }

    const int* getIndexPointer() const;
    void       loadModel(const std::string& fileName);
};