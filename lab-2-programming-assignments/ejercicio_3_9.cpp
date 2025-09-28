#include <iostream>
#include <vector>
#include <numeric>
#include <mpi.h>

using namespace std;

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int comm_sz, my_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    int n = 100000;
    int local_n = n / comm_sz;

    vector<int> block_data(local_n);
    iota(block_data.begin(), block_data.end(), my_rank * local_n);

    vector<int> cyclic_data(local_n);
    
    MPI_Barrier(MPI_COMM_WORLD);
    double start_time = MPI_Wtime();

    vector<int> send_buf_b2c;
    for(int i = 0; i < local_n; ++i) {
        if ((block_data[i] % comm_sz) == my_rank) {
            // Keep it
        } else {
            send_buf_b2c.push_back(block_data[i]);
        }
    }

    MPI_Alltoall(send_buf_b2c.data(), send_buf_b2c.size()/comm_sz, MPI_INT, 
                 cyclic_data.data(), local_n/comm_sz, MPI_INT, MPI_COMM_WORLD);

    double end_time = MPI_Wtime();
    if (my_rank == 0) {
        cout << "Tiempo de Bloque -> Cíclico: " << end_time - start_time << " s" << endl;
    }

    MPI_Finalize();
    return 0;
}