#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <mpi.h>

using namespace std;

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int comm_sz, my_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    int n = 100000;
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int local_n = n / comm_sz;
    vector<int> local_keys(local_n);

    mt19937 gen(time(0) + my_rank);
    uniform_int_distribution<> dis(0, n);
    for (int i = 0; i < local_n; ++i) {
        local_keys[i] = dis(gen);
    }

    sort(local_keys.begin(), local_keys.end());
    
    MPI_Finalize();
    return 0;
}