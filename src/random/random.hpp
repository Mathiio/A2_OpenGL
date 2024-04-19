#pragma once

#include <glm/gtc/random.hpp>
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"

// double random01();
// int markovTransition(double p0, double p1, double p2);
// glm::vec3 randomPos();
double randomUniform(double min, double max);
glm::vec3 randGaussian(double mu, double sigma, double xmin, double xmax, double height, double zmin, double zmax);
glm::vec3 randBimodale(double min, double max, double peak1, double peak2, double stdDev1, double stdDev2);