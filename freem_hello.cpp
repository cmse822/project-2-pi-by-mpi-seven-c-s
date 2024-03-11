#include "mpi.h"
#include <iostream>
using namespace std;

int main(int argc, char *argv[]) 
{
    int numtasks, rank;
    int name_length = MPI_MAX_PROCESSOR_NAME;
    char proc_name[name_length];
    
    // Project requested this print line
    cout << "Pre-Initialize Output" << endl;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Project requested this print line
    cout << "Mid-Initialize Output" << endl;

    // Verifying multiple processors can output to console
    // if (rank == 2) {cout << "Hello from Processor " << rank << endl;}

    // Book problem 2.3
    MPI_Get_processor_name(proc_name, &name_length);
    cout << proc_name << endl;

    // Book problem 2.4
    cout << "I am processor " << rank << " out of " << numtasks << endl;

    // Book problem 2.5
    if(rank == 0) {cout << "I, processor " << rank << " out of " << numtasks << " am unique!" << endl;}

    MPI_Finalize();

    // Project requested this print line
    cout << "Post-Finalize Output" << endl;

    return 0;
}