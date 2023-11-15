#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include <time.h>

#define SIZE 1000 // Size of the lattice
#define TEMP 2.0 // Temperature
#define STEPS 1000000000 // Number of Monte Carlo steps

int lattice[SIZE][SIZE];

void initializeLattice() {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            lattice[i][j] = (rand() % 2) * 2 - 1; // Randomly set to +1 or -1
        }
    }
}

double calculateEnergyChange(int x, int y) {
    // Periodic boundary conditions
    int left = lattice[x == 0 ? SIZE - 1 : x - 1][y];
    int right = lattice[(x + 1) % SIZE][y];
    int up = lattice[x][y == 0 ? SIZE - 1 : y - 1];
    int down = lattice[x][(y + 1) % SIZE];

    // Calculate energy change if this spin is flipped
    return 2 * lattice[x][y] * (left + right + up + down);
}

void monteCarloStep(unsigned int *seed) {
    int x = rand_r(seed) % SIZE;
    int y = rand_r(seed) % SIZE;
    double deltaE = calculateEnergyChange(x, y);

    // Metropolis-Hastings criterion
    if (deltaE < 0 || exp(-deltaE / TEMP) > ((double)rand_r(seed) / RAND_MAX)) {
        lattice[x][y] *= -1;
    }
}

int main() {
    srand(time(NULL));

    initializeLattice();

    #pragma omp parallel
    {
        unsigned int seed = time(NULL) ^ omp_get_thread_num();
        
        #pragma omp for
        for (int i = 0; i < STEPS; i++) {
            monteCarloStep(&seed);
        }
    }

    // Output the final lattice state for analysis
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            printf("%d ", lattice[i][j]);
        }
        printf("\n");
    }

    return 0;
}

