#pragma once

#include <glm/gtc/random.hpp>
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"

double random01();
int markovTransition(double p0, double p1, double p2);
glm::vec3 randomPos();