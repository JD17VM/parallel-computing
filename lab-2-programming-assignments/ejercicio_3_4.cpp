#include <iostream>
#include <vector>
#include <cmath>
#include <mpi.h>

using namespace std;

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int comm_sz, my_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    if ( (comm_sz & (comm_sz - 1)) != 0 && comm_sz != 0) {
        if (my_rank == 0) {
            cerr << "Error: El número de procesos debe ser una potencia de dos para este algoritmo." << endl;
        }
        MPI_Finalize();
        return 1;
    }

    double my_val = (double)my_rank;
    double sum = my_val;

    for (int i = 0; i < log2(comm_sz); ++i) {
        int partner = my_rank ^ (1 << i);
        double received_val;
        MPI_Sendrecv(&sum, 1, MPI_DOUBLE, partner, 0,
                     &received_val, 1, MPI_DOUBLE, partner, 0,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        sum += received_val;
    }
    
    cout << "Proceso " << my_rank << " tiene la suma global (mariposa): " << sum << endl;

    MPI_Finalize();
    return 0;
}