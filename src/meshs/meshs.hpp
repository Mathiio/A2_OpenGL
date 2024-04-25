#pragma once

#include "mesh.hpp"
#include "random/random.hpp"

class Meshs {
public:
    Meshs(const std::string& modelMesh, const std::string& modelTexture, int numMeshs)
        : m_numMeshs(numMeshs), m_mesh(modelMesh, modelTexture)
    {
        for (int i = 0; i < m_numMeshs; ++i)
        {
            m_meshs.emplace_back(m_mesh);
        }
    }

    void randomPos()
    {
        for (auto& mesh : m_meshs)
        {
            mesh.setPos(glm::vec3(randGeometric(0.5f, -0.8f, 0.8f), 0.98f, randGeometric(0.5f, -0.8f, 0.8f)));
        }
    }

    void randomScale()
    {
        for (auto& mesh : m_meshs)
        {
            mesh.setScale(glm::vec3(randBinomial(10, 0.7f), randBinomial(10, 0.7f), randBinomial(10, 0.7f)));
        }
    }

    void draw(glm::mat4 ProjMatrix, glm::mat4 viewMatrix, GLuint uMVPMatrixLocation, GLuint uMVMatrixLocation, GLuint uNormalMatrixLocation)
    {
        for (auto& mesh : m_meshs)
        {
            mesh.draw(mesh.getPos(), mesh.getScale(), ProjMatrix, viewMatrix, uMVPMatrixLocation, uMVMatrixLocation, uNormalMatrixLocation, 0.0f);
        }
    }

private:
    int               m_numMeshs;
    std::vector<Mesh> m_meshs;
    Mesh              m_mesh;
};