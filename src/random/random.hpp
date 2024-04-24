#pragma once

#include <glm/gtc/random.hpp>
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"

float     randMarkov(glm::vec3 transition, float initial);
int       randBernoulli(float p);
double    randUniform(double min, double max);
glm::vec3 randGaussian(double mu, double sigma, double xmin, double xmax, double height, double zmin, double zmax);
glm::vec3 randBimodale(double min, double max, double peak1, double peak2, double stdDev1, double stdDev2);
float     randExponential(int min, int max);
float     randBeta(float min, float max, float threshold);