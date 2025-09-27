#include <iostream>
#include <vector>
#include <ctime>
#include <mpi.h>

using namespace std;

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int comm_sz, my_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    if (comm_sz < 2) {
        if (my_rank == 0) {
            cerr << "Se necesitan al menos 2 procesos." << endl;
        }
        MPI_Finalize();
        return 1;
    }

    const int PING_PONG_LIMIT = 100;
    const int MSG_SIZE = 1; 
    vector<int> msg(MSG_SIZE, 0);

    double mpi_wtime_start, mpi_wtime_finish;
    clock_t c_start, c_finish;

    MPI_Barrier(MPI_COMM_WORLD);
    
    if (my_rank == 0) {
        mpi_wtime_start = MPI_Wtime();
        c_start = clock();

        for (int i = 0; i < PING_PONG_LIMIT; ++i) {
            MPI_Send(msg.data(), MSG_SIZE, MPI_INT, 1, 0, MPI_COMM_WORLD);
            MPI_Recv(msg.data(), MSG_SIZE, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

        mpi_wtime_finish = MPI_Wtime();
        c_finish = clock();
        
        cout << "Tiempo total (MPI_Wtime): " << (mpi_wtime_finish - mpi_wtime_start) << " s" << endl;
        cout << "Tiempo total (clock): " << ((double)(c_finish - c_start) / CLOCKS_PER_SEC) << " s" << endl;

    } else if (my_rank == 1) {
        for (int i = 0; i < PING_PONG_LIMIT; ++i) {
            MPI_Recv(msg.data(), MSG_SIZE, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Send(msg.data(), MSG_SIZE, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
    }

    MPI_Finalize();
    return 0;
}