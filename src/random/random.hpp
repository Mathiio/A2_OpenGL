#pragma once

#include <glm/gtc/random.hpp>
#include "glm/fwd.hpp"

float randMarkov(glm::vec3 transition, float initial);
float randBernoulli(float p);
float randUniform(float min, float max);
float randGeometric(double p, float minBound, float maxBound);
float randBinomial(int n, float p);
float randExponential(int min, int max);
float randBeta(float min, float max, float threshold);
float randCauchy(float min, float max);
bool  randPoisson(float lambda);