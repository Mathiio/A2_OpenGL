#define _USE_MATH_DEFINES
#include "random.hpp"
#include <p6/p6.h>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <glm/gtc/random.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"

float markov(glm::vec3 transition, float initial)
{
    float sequence = initial;

    // Générateur de nombres aléatoires
    std::random_device               rd;
    std::mt19937                     gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    while (true)
    {
        float next = 0.0f;

        // Trouve la prochaine valeur en fonction des probabilités de transition
        double randNum        = dis(gen);
        double cumulativeProb = 0.0;
        for (int k = 0; k < 3; ++k)
        {
            cumulativeProb += transition[k];
            if (randNum <= cumulativeProb)
            {
                next = (k + randNum) / 3.0f; // Utilise l'indice k pour obtenir la prochaine valeur
                break;
            }
        }

        // Ajoute la valeur actuelle à la séquence
        sequence += next;

        if (sequence <= 1.0f)
            break;
    }
    return sequence - initial;
}

// Fonction pour générer un nombre aléatoire selon une distribution bimodale entre min et max
glm::vec3 randBimodale(double min, double max, double peak1, double peak2, double stdDev1, double stdDev2)
{
    double u = static_cast<double>(rand()) / RAND_MAX;
    double randNumX, randNumY, randNumZ;

    if (u < 0.5)
    {
        randNumX = peak1 + stdDev1 * sqrt(-2 * log(1 - static_cast<double>(rand()) / RAND_MAX)) * cos(2 * M_PI * static_cast<double>(rand()) / RAND_MAX);
    }
    else
    {
        randNumX = peak2 + stdDev2 * sqrt(-2 * log(1 - static_cast<double>(rand()) / RAND_MAX)) * cos(2 * M_PI * static_cast<double>(rand()) / RAND_MAX);
    }

    u = static_cast<double>(rand()) / RAND_MAX;

    if (u < 0.5)
    {
        randNumY = peak1 + stdDev1 * sqrt(-2 * log(1 - static_cast<double>(rand()) / RAND_MAX)) * cos(2 * M_PI * static_cast<double>(rand()) / RAND_MAX);
    }
    else
    {
        randNumY = peak2 + stdDev2 * sqrt(-2 * log(1 - static_cast<double>(rand()) / RAND_MAX)) * cos(2 * M_PI * static_cast<double>(rand()) / RAND_MAX);
    }

    u = static_cast<double>(rand()) / RAND_MAX;

    if (u < 0.5)
    {
        randNumZ = peak1 + stdDev1 * sqrt(-2 * log(1 - static_cast<double>(rand()) / RAND_MAX)) * cos(2 * M_PI * static_cast<double>(rand()) / RAND_MAX);
    }
    else
    {
        randNumZ = peak2 + stdDev2 * sqrt(-2 * log(1 - static_cast<double>(rand()) / RAND_MAX)) * cos(2 * M_PI * static_cast<double>(rand()) / RAND_MAX);
    }

    // Assurer que les nombres générés sont dans l'intervalle spécifié
    randNumX = std::min(std::max(min, randNumX), max);
    randNumY = std::min(std::max(min, randNumY), max);
    randNumZ = std::min(std::max(min, randNumZ), max);

    return glm::vec3(randNumX, randNumY, randNumZ);
}

double randomUniform(double min, double max)
{
    return min + static_cast<double>(rand()) / (RAND_MAX / (max - min));
}

// Random number selon distribution gaussienne avec moyenne mu et écart-type sigma et un intervalle
glm::vec3 randGaussian(double mu, double sigma, double xmin, double xmax, double height, double zmin, double zmax)
{
    double x, z;
    do
    {
        double u1 = randomUniform(0, 1);
        double u2 = randomUniform(0, 1);
        x         = sqrt(-2 * log(u1)) * cos(2 * M_PI * u2);
    } while (x < xmin && x > xmax);

    do
    {
        double u1 = randomUniform(0, 1);
        double u2 = randomUniform(0, 1);
        z         = sqrt(-2 * log(u1)) * cos(2 * M_PI * u2);
    } while (z < zmin && z > zmax);

    return glm::vec3(mu + sigma * x, height, mu + sigma * z);
}