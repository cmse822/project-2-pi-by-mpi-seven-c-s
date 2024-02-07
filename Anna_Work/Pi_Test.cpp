#include <iostream>
#include <cstdlib>
#include <cmath>
#include <mpi.h>

// Function to generate random points and count those inside the circle
int count_points(int total_points) {
    int count = 0;
    double x, y;
    for (int i = 0; i < total_points; ++i) {
        x = (double)rand() / RAND_MAX;  // Generate random x coordinate
        y = (double)rand() / RAND_MAX;  // Generate random y coordinate
        if (x * x + y * y <= 1) {       // Check if point is inside the circle
            count++;
        }
    }
    return count;
}

int main(int argc, char *argv[]) {
    int rank, size;
    int num_rounds = 10;         // Number of rounds
    int total_points = 1000000;  // Total number of points
    int points_in_circle = 0;    // Number of points inside the circle
    int throws_per_round = total_points / num_rounds; // Number of throws per round

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    srand(rank);  // Seed random number generator with process rank

    for (int round = 0; round < num_rounds; ++round) {
        // Each process calculates its share of points for this round
        int local_count = count_points(throws_per_round);

        // Reduce local counts to get total count of points inside the circle
        MPI_Reduce(&local_count, &points_in_circle, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

        // Process 0 calculates pi for this round using the formula
        if (rank == 0) {
            double pi = 4.0 * ((double)points_in_circle / (throws_per_round * size));
            std::cout << "Round " << round + 1 << ": Approximated value of pi: " << pi << std::endl;
        }
    }

    MPI_Finalize();

    return 0;
}
