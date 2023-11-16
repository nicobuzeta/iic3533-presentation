#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include <time.h>
#include <unistd.h>
#include <random>

// Define the function to integrate
double f(double x) {
    return sin(x);
}

int main() {
    long num_samples = 100000000; // Number of random samples
    double a = 0; // Lower bound of the interval
    double b = M_PI; // Upper bound of the interval (integrating sin(x) from 0 to PI)
    double integral = 0.0;
    double sum = 0.0;

    #pragma omp parallel reduction(+:sum)
    {
        // Create a thread-local random number generator
        std::mt19937 generator(omp_get_thread_num());
        std::uniform_real_distribution<double> distribution(a, b);

        #pragma omp for
        for (long i = 0; i < num_samples; ++i) {
            double x = distribution(generator); // Generate a random x value
            sum += f(x);
        }
    }
    // Calculate the final integral value
    integral = (b - a) * sum / num_samples;

    printf("Estimated integral: %.15f\n", integral);

    return 0;
}

