#include "random.hpp"
#include <cstdlib>
#include <iostream>

double RAND()
{
    thread_local std::default_random_engine gen{std::random_device{}()};
    thread_local auto                       distrib = std::uniform_real_distribution<float>{0.0, 1.0};

    return distrib(gen);
}

/************ LOI UNIFORME ************/

double uniform(double a, double b)
{
    return a + (b - a) * RAND();
}

/************* Loi hypergeometrique **********/

std::vector<float> generateHyperGeometric(int nombreTotal, int tries, int nombreSucces)
{
    std::vector<float> result;
    std::vector<int>   TotalArray;

    // Initialiser TotalArray avec les succès et les échecs
    TotalArray.reserve(nombreSucces);
    for (int i = 0; i < nombreSucces; i++)
    {
        TotalArray.push_back(1);
    }
    for (int i = 0; i < (nombreTotal - nombreSucces); i++)
    {
        TotalArray.push_back(0);
    }

    // Générer les résultats
    for (int i = 0; i < tries; i++)
    {
        int randIndex = rand() % nombreTotal;
        result.push_back(TotalArray[randIndex]);
        TotalArray.erase(TotalArray.begin() + randIndex);
        nombreTotal--;
    }
    return result;
}

// Bernoulli
std::vector<float> generateBernoulliSchema(double probaSuccess, int tries)
{
    std::vector<float> result;

    if (probaSuccess > 1 || probaSuccess < 0)
    {
        std::cout << "La probabilité doit être comprise entre 0 et 1" << '\n';
        return result;
    }

    for (int i = 0; i < tries; i++)
    {
        if (RAND() <= probaSuccess)
        {
            result.push_back(1);
        }
        else
        {
            result.push_back(0);
        }
    }

    return result;
}

// Loi exponentielle
double generateExponential(double lambda)
{
    return -log(1 - RAND()) / lambda;
}

// Loi géométrique
double geometricTrial(double p)
{
    int count = 0;

    if (p > 1 || p < 0)
    {
        std::cout << "La probabilité doit être comprise entre 0 et 1" << '\n';
        return count;
    }

    while (RAND() < p)
    {
        count++;
    }

    return count;
}

// Loi de poisson
int poissonRandom(double lambda)
{
    double L = exp(-lambda);
    double p = 1.0;
    int    k = 0;

    while (p > L)
    {
        k++;
        p *= RAND();
    }

    return k - 1;
}

// Loi de Laplace
double laplaceRandom(double pos, double b)
{
    double p   = RAND();
    int    sgn = (p < 0.5) ? -1 : 1; // Fonction signe basée sur p
    return pos - b * sgn * log(1 - 2 * fabs(p - 0.5));
}

void calculateMarkovState(glm::vec3& currentState, glm::mat3 matrix)
{
    glm::vec3 proba = matrix * currentState;

    float randomValue = RAND();
    float sum         = 0.f;
    int   state       = 0;

    for (state = 0; state < 3; ++state)
    {
        sum += proba[state];
        if (randomValue <= sum)
        {
            break;
        }
    }

    switch (state)
    {
    case 0:
        currentState = {1.f, 0.f, 0.f};
        break;
    case 1:
        currentState = {0.f, 1.f, 0.f};
        break;
    case 2:
        currentState = {0.f, 0.f, 1.f};
        break;
    default: break;
    }
}
