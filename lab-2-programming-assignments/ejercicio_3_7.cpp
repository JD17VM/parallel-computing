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

    MPI_Finalize();
    return 0;
}