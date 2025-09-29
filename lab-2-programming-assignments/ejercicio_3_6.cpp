#include <iostream>
#include <vector>
#include <cmath>
#include <mpi.h>

using namespace std;

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int world_sz, world_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &world_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    const int n = 120; 
    int q = sqrt(world_sz);
    if (q * q != world_sz) {
        if (world_rank == 0)
            cerr << "El número de procesos debe ser un cuadrado perfecto." << endl;
        MPI_Abort(MPI_COMM_WORLD, 1);
    }
    int local_n = n / q;

    MPI_Comm grid_comm;
    int dims[2] = {q, q};
    int periods[2] = {0, 0};
    MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, 1, &grid_comm);

    int my_coords[2];
    MPI_Cart_coords(grid_comm, world_rank, 2, my_coords);

    vector<double> local_A(local_n * local_n);
    vector<double> x_chunk(local_n);
    vector<double> local_x(local_n);

    if (my_coords[0] == my_coords[1]) {
        for (int i = 0; i < local_n; ++i) x_chunk[i] = 1.0;
    }
    for (int i = 0; i < local_n * local_n; ++i) local_A[i] = 1.0;

    MPI_Comm row_comm;
    MPI_Comm_split(grid_comm, my_coords[0], my_coords[1], &row_comm);
    MPI_Bcast(x_chunk.data(), local_n, MPI_DOUBLE, my_coords[0], row_comm);

    vector<double> local_y_partial(local_n, 0.0);
    for (int i = 0; i < local_n; i++) {
        for (int j = 0; j < local_n; j++) {
            local_y_partial[i] += local_A[i * local_n + j] * x_chunk[j];
        }
    }

    vector<double> local_y_final(local_n);
    MPI_Comm col_comm;
    MPI_Comm_split(grid_comm, my_coords[1], my_coords[0], &col_comm);
    MPI_Reduce(local_y_partial.data(), local_y_final.data(), local_n, MPI_DOUBLE, MPI_SUM, 0, col_comm);

    if (my_coords[1] == 0) {
        cout << "Proceso (" << my_coords[0] << ",0) tiene su parte del resultado." << endl;
    }

    MPI_Comm_free(&row_comm);
    MPI_Comm_free(&col_comm);
    MPI_Comm_free(&grid_comm);
    MPI_Finalize();
    return 0;
}