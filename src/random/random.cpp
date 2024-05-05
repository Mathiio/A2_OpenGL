#define _USE_MATH_DEFINES
#include "random.hpp"
#include <p6/p6.h>
#include <cmath>
#include <cstdlib>
#include <glm/gtc/random.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "glm/fwd.hpp"

float randMarkov(glm::vec3 transition, float initial)
{
    // Initialisation du générateur de nombres aléatoires
    std::random_device                    rd;
    std::mt19937                          gen(rd());
    std::uniform_real_distribution<float> dis(0.0f, 1.0f);

    // État initial
    glm::vec3 currentState = glm::vec3(initial, 0.0f, 0.0f);
    float     sequence     = initial;

    // Générer la séquence basée sur les probabilités de transition
    while (sequence < 1.0f)
    {
        // Générer un nombre aléatoire entre 0 et 1
        float const randNum = dis(gen);

        // Recherche de la transition basée sur le nombre aléatoire
        float cumulativeProb = 0.0f;
        for (int nextState = 0; nextState < 3; ++nextState)
        {
            cumulativeProb += transition[nextState];
            if (randNum <= cumulativeProb)
            {
                // Ajouter la transition à la séquence
                sequence += transition[nextState];
                currentState[nextState] = sequence;
                break;
            }
        }
    }
    return sequence - initial;
}

bool randBernoulli(float p)
{
    std::random_device               rd;
    std::mt19937                     gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0); // Distribution uniforme entre 0 et 1

    // Générer un nombre aléatoire entre 0 et 1
    double rand_num = dis(gen);

    // Si le nombre aléatoire est inférieur ou égal à la probabilité de succès,
    // alors l'expérience est un succès, sinon c'est un échec
    return rand_num <= p;
}

float randBinomial(int n, float p)
{
    // Initialisation du générateur de nombres aléatoires
    std::random_device                     rd;
    std::mt19937                           gen(rd());
    std::uniform_real_distribution<double> dis(0.0, 1.0);

    int successes = 0;

    // Génération d'un échantillon binomial
    for (int i = 0; i < n; ++i)
    {
        double const randNum = dis(gen);
        if (randNum < p)
        {                // Comparaison avec la probabilité de succès
            successes++; // Incrémenter le nombre de succès si le nombre aléatoire est inférieur à la probabilité de succès
        }
    }

    // Retourner la proportion de succès dans l'échantillon
    return static_cast<float>(successes) / static_cast<float>(n);
}

float randUniform(float min, float max)
{
    std::random_device                    rd;
    std::mt19937                          gen(rd());
    std::uniform_real_distribution<float> dis(0.0, 1.0);
    return min + static_cast<float>(dis(gen)) * (max - min);
}

float randGeometric(double p, float minBound, float maxBound)
{
    // Initialisation du générateur de nombres aléatoires
    std::random_device                     rd;
    std::mt19937                           gen(rd());
    std::uniform_real_distribution<double> dis(0.0, 1.0);

    float       timeToSuccess = 0.0f;
    float const maxTrials     = maxBound / minBound;

    // Génération d'un échantillon selon la loi géométrique
    while (true)
    {
        std::uniform_real_distribution<float> distribution(minBound, maxBound);
        timeToSuccess += distribution(gen);
        double const randNum = dis(gen);
        if (randNum < p)
        {          // Comparaison avec la probabilité de succès
            break; // Sortir de la boucle dès que le succès est obtenu
        }
        if (timeToSuccess >= maxTrials)
        { // Si le temps écoulé atteint la borne supérieure, retourner la borne supérieure
            return maxBound;
        }
    }

    // Si le temps écoulé est inférieur à la borne inférieure, retourner la borne inférieure
    return (timeToSuccess < minBound) ? minBound : timeToSuccess;
}

float randExponential(float min, float max)
{
    std::random_device rd;
    std::mt19937       gen(rd());

    float const lambda = 0.5;

    std::uniform_real_distribution<float> uniform_dist(0.0f, 1.0f);
    float const                           uniform_random = uniform_dist(gen);

    float randomNumber = -log(1 - uniform_random) / lambda;
    randomNumber       = min + (max - min) * (1 - exp(-lambda * randomNumber));

    return randomNumber;
}

float randBeta(float min, float max, float threshold)
{
    if (min >= max)
    {
        std::cerr << "Erreur : bornes invalides !\n";
        return 0.0;
    }

    std::random_device rd;
    std::mt19937       gen(rd());

    float const alpha = 1.5f;
    float const beta  = 1.0f;

    std::uniform_real_distribution<float> uniform_dist(0.0f, 1.0f);

    float randomNumber = 0;

    while (randomNumber <= threshold)
    {
        float const u = uniform_dist(gen);
        float const v = uniform_dist(gen);

        randomNumber = pow(u, 1.0f / alpha);
        randomNumber /= pow(u, 1.0f / alpha) + pow(v, 1.0f / beta);
        randomNumber = min + (max - min) * randomNumber;
    }

    return randomNumber;
}

float randCauchy(float min, float max)
{
    // Initialisation du générateur de nombres aléatoires
    std::random_device rd;
    std::mt19937       gen(rd());

    // Générer un nombre aléatoire selon la distribution de Cauchy centrée autour de la moyenne de min et max
    float const                      center = (max + min) / 2;
    float const                      scale  = (max - min) / 2;
    std::cauchy_distribution<double> distribution(center, scale);
    double                           randomNumber = distribution(gen);

    // Assurer que le nombre aléatoire généré est entre min et max
    while (randomNumber < min || randomNumber > max)
    {
        randomNumber = distribution(gen);
    }

    return static_cast<float>(randomNumber);
}

bool randPoisson(float lambda)
{
    // Initialisation du générateur de nombres aléatoires
    std::random_device                    rd;
    std::mt19937                          gen(rd());
    std::uniform_real_distribution<float> uniform_dist(0.0f, 1.0f);

    // Génération d'un échantillon selon la loi de Poisson
    float const L = exp(-lambda);
    int         k = 0;
    float       p = 1;

    while (p > L)
    {
        k++;
        p *= static_cast<float>(uniform_dist(gen)) / RAND_MAX;
    }

    // Retourner l'échantillon de Poisson
    return (k == 1);
}