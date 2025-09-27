#include <iostream>
#include <vector>
#include <random>
#include <numeric>
#include <mpi.h>

using namespace std;

void generate_data(vector<float>& data, int size) {
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(0.0, 5.0);
    data.resize(size);
    for (int i = 0; i < size; ++i) {
        data[i] = dis(gen);
    }
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int comm_sz, my_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    const int data_count = 100000;
    const int bin_count = 5;
    const float min_meas = 0.0;
    const float max_meas = 5.0;

    vector<float> data;
    if (my_rank == 0) {
        generate_data(data, data_count);
    }

    int local_data_count = data_count / comm_sz;
    vector<float> local_data(local_data_count);

    MPI_Scatter(data.data(), local_data_count, MPI_FLOAT, local_data.data(), local_data_count, MPI_FLOAT, 0, MPI_COMM_WORLD);

    MPI_Finalize();
    return 0;
}