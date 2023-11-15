#include <mpi.h>
#include <iostream>
#include <cstdlib>
#include <ctime>

long long modular_pow(long long base, int exponent, int modulus) {
    long long result = 1;
    base = base % modulus;

    while (exponent > 0) {
        if (exponent % 2 == 1) {
            result = (result * base) % modulus;
        }
        exponent = exponent >> 1;
        base = (base * base) % modulus;
    }

    return result;
}

bool isPrime(int n, long long k) {
    if (n <= 1 || n == 4) return false;
    if (n <= 3) return true;

    while (k > 0) {
        int a = 2 + rand() % (n - 4);
        if (modular_pow(a, n - 1, n) != 1)
            return false;
        k--;
    }

    return true;
}

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    srand(time(NULL) + world_rank); // Seed for randomness

    int number;
    if (world_rank == 0) {
        number = atoi(argv[1]);
    }

    MPI_Bcast(&number, 1, MPI_INT, 0, MPI_COMM_WORLD);

    bool local_prime = isPrime(number, 30000000 / world_size);

    bool global_prime;
    MPI_Reduce(&local_prime, &global_prime, 1, MPI_C_BOOL, MPI_LAND, 0, MPI_COMM_WORLD);

    if (world_rank == 0) {
        if (global_prime)
            std::cout << "Number is prime" << std::endl;
        else
            std::cout << "Number is not prime" << std::endl;
    }

    MPI_Finalize();
    return 0;
}

