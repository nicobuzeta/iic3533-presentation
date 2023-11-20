#include <climits>
#include <math.h>
#include <omp.h>
#include <random>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 1000       // Size of the lattice
#define TEMP 2.0        // Temperature
#define STEPS 100000000 // Number of Monte Carlo steps

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

void monteCarloStep(std::uniform_int_distribution<int> &distribution,
                    std::mt19937 &generator) {
  int x = distribution(generator) % SIZE;
  int y = distribution(generator) % SIZE;
  double deltaE = calculateEnergyChange(x, y);

  // Metropolis-Hastings criterion
  if (deltaE < 0 || exp(-deltaE / TEMP) > (static_cast<double>(distribution(generator)) / INT_MAX)) {
    lattice[x][y] *= -1;
  }
}

int main() {
  srand(time(NULL));

  initializeLattice();

#pragma omp parallel
  {
    std::mt19937 generator(omp_get_thread_num());
    std::uniform_int_distribution<int> distribution(0, INT_MAX);

#pragma omp for
    for (int i = 0; i < STEPS; i++) {
        monteCarloStep(distribution, generator);
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
