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
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    std::cout << "Rank " << rank << " is running." << std::endl;

    int num_rounds = 10;          // Number of rounds
    int total_points = 1000000;   // Total number of points
    int total_pi = 0;     // Number of points inside the circle

    // Perform the same number of rounds on each MPI rank
    double start_time = MPI_Wtime();
    for (int round = 0; round < num_rounds; ++round) {
        // Each process calculates its share of points for this round
        int local_points = count_points(total_points / size);
        double local_pi = 4*local_points/total_points/size;
        // Reduce local counts to get total count of points inside the circle
        MPI_Reduce(&local_pi, &total_pi, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
        std::cout << "Local Count: " << local_pi << " Total Count" << total_pi/size << std::endl;
        
        // Process 0 calculates pi for this round using the formula
    }
    double end_time = MPI_Wtime();

    double total_time;
    MPI_Reduce(&start_time, &total_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        std::cout << "Total runtime: " << end_time - total_time << " seconds." << std::endl;
    }

    MPI_Finalize();

    return 0;
}
