#include "boids/boid.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glm/fwd.hpp"
#include "meshs/mesh.hpp"
#include "random/random.hpp"

Boid::Boid(glm::vec3 position, glm::vec3 velocity, glm::vec3 scale, float rotation)
    : position(position)
    , velocity(velocity)
    , scale(scale)
    , rotation(rotation)
{}

Boid::Boid()
    : position(glm::vec3{
        p6::random::number(-0.8f, 0.8f),
        p6::random::number(-0.8f, 0.8f),
        p6::random::number(-0.8f, 0.8f)
    })
    , velocity(glm::vec3{0.01f, 0.01f, 0.01f})
    , scale(glm::vec3(randCauchy(0.5f, 1.2f)))
    , rotation(0.0f)
{}

void Boid::update(float delta_time)
{
    position += velocity * delta_time;
}

void Boid::drawMesh(GLuint uMVPMatrixLocation, GLuint uMVMatrixLocation, GLuint uNormalMatrixLocation, glm::mat4 ProjMatrix, glm::mat4 viewMatrix, Mesh mesh) const
{
    glm::mat4 MVMatrix = viewMatrix * glm::translate(glm::mat4{1.f}, position) * glm::scale(glm::mat4{1.f}, glm::vec3(0.015f));

    glm::mat4 NormalMatrix = glm::transpose(glm::inverse(MVMatrix));

    glUniformMatrix4fv(uMVPMatrixLocation, 1, GL_FALSE, glm::value_ptr(ProjMatrix * MVMatrix));
    glUniformMatrix4fv(uMVMatrixLocation, 1, GL_FALSE, glm::value_ptr(MVMatrix));
    glUniformMatrix4fv(uNormalMatrixLocation, 1, GL_FALSE, glm::value_ptr(NormalMatrix));

    mesh.draw(position, scale, ProjMatrix, viewMatrix, uMVPMatrixLocation, uMVMatrixLocation, uNormalMatrixLocation, rotation);
}