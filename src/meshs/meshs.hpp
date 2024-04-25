#pragma once

#include "mesh.hpp"
#include "random/random.hpp"
#include "textures/texture.hpp"

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
            mesh.setPos(randGaussian(0, 0.34, -0.8, 0.8, 0.98, -0.8, 0.8));
        }
    }

    void randomScale()
    {
        for (auto& mesh : m_meshs)
        {
            mesh.setScale(randBimodale(0.6, 1.4, 0.8, 1.20, 0.1, 0.15));
        }
    }

    void draw(glm::mat4 ProjMatrix, glm::mat4 viewMatrix, GLuint uMVPMatrixLocation, GLuint uMVMatrixLocation, GLuint uNormalMatrixLocation, float angle)
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