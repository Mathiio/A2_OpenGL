#include <glm/gtc/random.hpp>
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "random.hpp"
#include <p6/p6.h>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>


// double random01() {
//     static std::random_device rd;
//     static std::mt19937 gen(rd());
//     static std::uniform_real_distribution<double> dis(0.0, 1.0);
//     return dis(gen);
// }

// int markovTransition(double p0, double p1, double p2) {
//     double r = random01();
//     if (r < p0 / 2.0) return -1; // Transition vers l'état -0.8 avec une probabilité réduite
//     else if (r < p0 + p1 * 2.0) return 0; // Reste dans l'état actuel avec une probabilité augmentée
//     else return 1; // Transition vers l'état 0.8 avec une probabilité normale
// }

// glm::vec3 randomPos() {
//     // Probabilités de transition pour chaque composante (arbitrairement égales pour cet exemple)
//     double transitionProbability = 1.0 / 3.0;
    
//     // États possibles pour chaque composante
//     double states[] = {-0.8, 0, 0.8};
    
//     // Simulation de la chaîne de Markov pour chaque composante
//     int stateX = markovTransition(transitionProbability, transitionProbability, transitionProbability);
//     int stateY = markovTransition(transitionProbability, transitionProbability, transitionProbability);
//     int stateZ = markovTransition(transitionProbability, transitionProbability, transitionProbability);
    
//     // Sélection des valeurs correspondant aux états pour chaque composante
//     double x = states[stateX];
//     double y = states[stateY];
//     double z = states[stateZ];
    
//     // Génération du glm::vec3 avec la valeur y ajustée pour être entre 1 et 1.2
//     double yMin = 0.9;
//     double yMax = 1.2;
//     double yRange = yMax - yMin;
//     y = yMin + (y + 1.0) * (yRange / 2.0); // Remapping de y de l'intervalle [-1, 1] à [yMin, yMax]
    
//     return glm::vec3(x, y, z);
// }




// Fonction pour générer un nombre aléatoire selon une distribution bimodale entre min et max
glm::vec3 randBimodale(double min, double max, double peak1, double peak2, double stdDev1, double stdDev2) {
    double u = static_cast<double>(rand()) / RAND_MAX;
    double randNumX, randNumY, randNumZ;

    if (u < 0.5) {
        randNumX = peak1 + stdDev1 * sqrt(-2 * log(1 - static_cast<double>(rand()) / RAND_MAX)) * cos(2 * M_PI * static_cast<double>(rand()) / RAND_MAX);
    } else {
        randNumX = peak2 + stdDev2 * sqrt(-2 * log(1 - static_cast<double>(rand()) / RAND_MAX)) * cos(2 * M_PI * static_cast<double>(rand()) / RAND_MAX);
    }

    u = static_cast<double>(rand()) / RAND_MAX;

    if (u < 0.5) {
        randNumY = peak1 + stdDev1 * sqrt(-2 * log(1 - static_cast<double>(rand()) / RAND_MAX)) * cos(2 * M_PI * static_cast<double>(rand()) / RAND_MAX);
    } else {
        randNumY = peak2 + stdDev2 * sqrt(-2 * log(1 - static_cast<double>(rand()) / RAND_MAX)) * cos(2 * M_PI * static_cast<double>(rand()) / RAND_MAX);
    }

    u = static_cast<double>(rand()) / RAND_MAX;

    if (u < 0.5) {
        randNumZ = peak1 + stdDev1 * sqrt(-2 * log(1 - static_cast<double>(rand()) / RAND_MAX)) * cos(2 * M_PI * static_cast<double>(rand()) / RAND_MAX);
    } else {
        randNumZ = peak2 + stdDev2 * sqrt(-2 * log(1 - static_cast<double>(rand()) / RAND_MAX)) * cos(2 * M_PI * static_cast<double>(rand()) / RAND_MAX);
    }

    // Assurer que les nombres générés sont dans l'intervalle spécifié
    randNumX = std::min(std::max(min, randNumX), max);
    randNumY = std::min(std::max(min, randNumY), max);
    randNumZ = std::min(std::max(min, randNumZ), max);

    return glm::vec3(randNumX, randNumY, randNumZ);
}

double randomUniform(double min, double max) {
    return min + static_cast<double>(rand()) / (RAND_MAX / (max - min));
}

// Random number selon distribution gaussienne avec moyenne mu et écart-type sigma et un intervalle
glm::vec3 randGaussian(double mu, double sigma, double xmin, double xmax, double height, double zmin, double zmax) {
    double x, z;
    do {
        double u1 = randomUniform(0, 1);
        double u2 = randomUniform(0, 1);
        x = sqrt(-2 * log(u1)) * cos(2 * M_PI * u2);
    } while (x < xmin && x > xmax); 
    
    do {
        double u1 = randomUniform(0, 1);
        double u2 = randomUniform(0, 1);
        z = sqrt(-2 * log(u1)) * cos(2 * M_PI * u2);
    } while (z < zmin && z > zmax); 

    return glm::vec3(mu + sigma * x, height, mu + sigma * z);
}