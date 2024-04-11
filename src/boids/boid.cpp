#include "boids/boid.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"
#include "meshs/mesh.hpp"

Boid::Boid(glm::vec3 position, glm::vec3 velocity, float radius)
    : position(position)
    , velocity(velocity)
    , radius(radius)
{}

Boid::Boid()
    : position(glm::vec3{
        p6::random::number(-0.8f, 0.8f),
        p6::random::number(-0.8f, 0.8f),
        p6::random::number(-0.8f, 0.8f)
    })
    , velocity(glm::vec3{0.01f, 0.01f, 0.01f})
    , radius(0.01f)
{}

void Boid::update(float delta_time)
{
    position += velocity * delta_time;
}

void Boid::drawMesh(GLuint uMVPMatrixLocation, GLuint uMVMatrixLocation, GLuint uNormalMatrixLocation, glm::mat4 ProjMatrix, glm::mat4 viewMatrix, Mesh mesh, GLuint textName) const
{
    glm::vec3 direction(velocity.x, velocity.y, velocity.z);
    direction = glm::normalize(direction);
    glm::vec3 directionOrthogonale(-direction.y, direction.x, 0.0f);
    directionOrthogonale      = glm::normalize(directionOrthogonale);
    glm::vec3 directionFinale = glm::cross(direction, directionOrthogonale);
    directionFinale           = glm::normalize(directionFinale);

    glm::mat4 rotationMatrix(
        glm::vec4(directionFinale, 0.0f),
        glm::vec4(direction, 0.0f),
        glm::vec4(directionOrthogonale, 0.0f),
        glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
    );

    glm::mat4 MVMatrix = viewMatrix * glm::translate(glm::mat4{1.f}, position) * glm::scale(glm::mat4{1.f}, glm::vec3(0.015f)) * rotationMatrix;

    glm::mat4 NormalMatrix = glm::transpose(glm::inverse(MVMatrix));

    glUniformMatrix4fv(uMVPMatrixLocation, 1, GL_FALSE, glm::value_ptr(ProjMatrix * MVMatrix));
    glUniformMatrix4fv(uMVMatrixLocation, 1, GL_FALSE, glm::value_ptr(MVMatrix));
    glUniformMatrix4fv(uNormalMatrixLocation, 1, GL_FALSE, glm::value_ptr(NormalMatrix));

    mesh.draw(position, glm::vec3{1.}, ProjMatrix, viewMatrix, uMVPMatrixLocation, uMVMatrixLocation, uNormalMatrixLocation, textName);
}

glm::vec3 Boid::getPosition() const
{
    return position;
}

glm::vec3 Boid::getVelocity() const
{
    return velocity;
}