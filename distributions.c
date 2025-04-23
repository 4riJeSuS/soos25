#define _USE_MATH_DEFINES

#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <assert.h>
#include "probsched.h"


void seed_rng(unsigned int seed)
{
    srand(seed);
}

int uniform_random(int min, int max)
{
    assert(min <= max); 
    return min + rand() % (max - min + 1);
}

int exponential_random(double lambda)
{
    assert(lambda > 0); 
    double r = ((double)rand() / (RAND_MAX + 1.0)); 
    return (int)(-log(1 - r) / lambda);
}

int normal_random(double mean, double stddev)
{
    assert(stddev > 0); 
    double u1 = ((double)rand() / (RAND_MAX + 1.0));
    double u2 = ((double)rand() / (RAND_MAX + 1.0));
    double z0 = sqrt(-2.0 * log(u1)) * cos(2 * M_PI * u2);
    return (int)(mean + stddev * z0);
}

int poisson_random(double lambda)
{
    assert(lambda > 0); 
    double L = exp(-lambda);
    int k = 0;
    double p = 1.0;
    do
    {
        k++;
        p *= ((double)rand() / (RAND_MAX + 1.0));
    } while (p > L);
    return k - 1;
}