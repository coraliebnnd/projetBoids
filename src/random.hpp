#ifndef RANDOM_HPP
#define RANDOM_HPP

#include <p6/p6.h>
#include <cstdlib>
#include <random>

double             RAND();
double             uniform(double a, double b);
std::vector<float> generateHyperGeometric(int nombreTotal, int tries, int nombreSucces);
std::vector<float> generateBernoulliSchema(double probaSuccess, int tries);
double             generateExponential(double lambda);
double             geometricTrial(double p);
int                poissonRandom(double lambda);
double             laplaceRandom(double mu, double b);
void               calculateMarkovState(glm::vec3& currentState, glm::mat3 matrix);

#endif // RANDOM_HPP
