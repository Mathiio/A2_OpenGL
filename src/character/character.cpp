#include "character.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"
#include "meshs/mesh.hpp"

Character::Character(glm::vec3 position, glm::vec3 velocity, float rotation)
    : position(position)
    , velocity(velocity)
    , rotation(rotation)
{}

Character::Character()
    : position(glm::vec3{0.0, -0.2, -1.5})
    , velocity(glm::vec3{0.01f, 0.01f, 0.01f})
    , rotation(180.0f)
{}

void Character::draw(GLuint uMVPMatrixLocation, GLuint uMVMatrixLocation, GLuint uNormalMatrixLocation, glm::mat4 ProjMatrix, glm::mat4 viewMatrix, Mesh mesh, GLuint textName) const
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

    mesh.draw(position, glm::vec3{1.}, ProjMatrix, viewMatrix, uMVPMatrixLocation, uMVMatrixLocation, uNormalMatrixLocation, textName, rotation);
}

glm::vec3 Character::getPosition() const
{
    return position;
}

glm::vec3 Character::getVelocity() const
{
    return velocity;
}

float Character::getRotation() const
{
    return rotation;
}