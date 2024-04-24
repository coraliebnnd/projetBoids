#include "random.hpp"

double RAND()
{
    return static_cast<double>(rand()) / RAND_MAX;
}

/************ LOI UNIFORME ************/

double uniform(double a, double b)
{
    return a + (b - a) * RAND();
}

/************** BERNOULLI ****************/

int bernoulli(double proba)
{
    if (RAND() < proba)
        return 1;
    return 0;
}

/************** BINOMIALE *************/

int binomial(int iteration, double proba)
{
    int x = 0;
    for (int i = 0; i < iteration; ++i)
    {
        if (RAND() < proba)
            x++;
    }
    return x;
}

/***************** HYPERGEOMETRIC *****************/

int hypergeometric(int population, int succes, int iteration)
{
    int x = 0;
    for (int i = 0; i < iteration; ++i)
    {
        if (RAND() < static_cast<double>(succes) / population)
        {
            ++x;
            --succes;
        }
        --population;
    }
    return x;
}

/********************* GEOMETRIC **********************/

int geometric(double p)
{
    int x = 1;
    while (RAND() > p)
    {
        ++x;
    }
    return x;
}
