#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <mpi.h>

using namespace std;

vector<int> merge(const vector<int>& a, const vector<int>& b) {
    vector<int> result;
    result.reserve(a.size() + b.size());
    merge(a.begin(), a.end(), b.begin(), b.end(), back_inserter(result));
    return result;
}

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
    
    for (int i = 1; i < comm_sz; i *= 2) {
        if (my_rank % (2 * i) == 0) {
            if (my_rank + i < comm_sz) {
                int partner_size;
                MPI_Status status;
                MPI_Probe(my_rank + i, 0, MPI_COMM_WORLD, &status);
                MPI_Get_count(&status, MPI_INT, &partner_size);

                vector<int> partner_keys(partner_size);
                MPI_Recv(partner_keys.data(), partner_size, MPI_INT, my_rank + i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                local_keys = merge(local_keys, partner_keys);
            }
        } else if (my_rank % i == 0) {
            MPI_Send(local_keys.data(), local_keys.size(), MPI_INT, my_rank - i, 0, MPI_COMM_WORLD);
            break; 
        }
    }

    if (my_rank == 0) {
        cout << "Ordenamiento por mezcla paralelo completado." << endl;
    }

    MPI_Finalize();
    return 0;
}