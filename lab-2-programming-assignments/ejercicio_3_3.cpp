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

    double local_value = (double)my_rank + 1.0;
    double sum = local_value;

    for (int i = 1; i < comm_sz; i *= 2) {
        if (my_rank % (2 * i) == 0) {
            if (my_rank + i < comm_sz) {
                double received_value;
                MPI_Recv(&received_value, 1, MPI_DOUBLE, my_rank + i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                sum += received_value;
            }
        } else if (my_rank % i == 0) {
            MPI_Send(&sum, 1, MPI_DOUBLE, my_rank - i, 0, MPI_COMM_WORLD);
        }
    }

    if (my_rank == 0) {
        cout << "Suma global calculada (árbol): " << sum << endl;
    }

    MPI_Finalize();
    return 0;
}