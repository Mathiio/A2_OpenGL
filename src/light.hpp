#pragma once

#include <glm/glm.hpp>
#include "character/character.hpp"
#include "glimac/common.hpp"
#include "glm/fwd.hpp"
#include "p6/p6.h"
#include "random/random.hpp"

class Light {
public:
    int       index;
    glm::vec3 color;
    glm::vec3 position;
    GLuint    uKdLocation;
    GLuint    uKsLocation;
    GLuint    uShininessLocation;
    GLuint    uLightPosLocation;
    GLuint    uLightIntensityLocation;

    Light(int index, const p6::Shader& shader)
        : index(index), color(randUniform(0.5f, 1.0f), randUniform(0.5f, 1.0f), randUniform(0.5f, 1.0f))
    {
        setup(shader);
    }

    void setup(const p6::Shader& shader)
    {
        if (index == 1)
        {
            uKdLocation             = glGetUniformLocation(shader.id(), "uKd");
            uKsLocation             = glGetUniformLocation(shader.id(), "uKs");
            uShininessLocation      = glGetUniformLocation(shader.id(), "uShininess");
            uLightPosLocation       = glGetUniformLocation(shader.id(), "uLightPos_vs");
            uLightIntensityLocation = glGetUniformLocation(shader.id(), "uLightIntensity");
        }
        else if (index == 2)
        {
            uKdLocation             = glGetUniformLocation(shader.id(), "uKd2");
            uKsLocation             = glGetUniformLocation(shader.id(), "uKs2");
            uShininessLocation      = glGetUniformLocation(shader.id(), "uShininess2");
            uLightPosLocation       = glGetUniformLocation(shader.id(), "uLightPos_vs2");
            uLightIntensityLocation = glGetUniformLocation(shader.id(), "uLightIntensity2");
        }
    }

    void update(glm::mat4 viewMatrix, Character& character)
    {
        glm::vec4 lightPos(0.0f);
        if (index == 1)
        {
            lightPos = {1.1f, 1.1f, 1.1f, 1.0f};
        }
        else if (index == 2)
        {
            lightPos = {character.getPosition().x - 0.5f, character.getPosition().y + 0.5f, character.getPosition().z + 0.5f, 1.0f};
        }
        position = glm::vec3(viewMatrix * lightPos);
        glUniform3f(uKdLocation, color.x, color.y, color.z);
        glUniform3f(uKsLocation, color.x, color.y, color.z);
        glUniform1f(uShininessLocation, 4.0f);
        glUniform3f(uLightPosLocation, position.x, position.y, position.z);
        glUniform3f(uLightIntensityLocation, 2.0f, 2.0f, 2.0f);
    }
};