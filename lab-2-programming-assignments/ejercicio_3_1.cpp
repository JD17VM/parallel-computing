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

    vector<int> local_bin_counts(bin_count, 0);
    float bin_width = (max_meas - min_meas) / bin_count;

    for (float val : local_data) {
        int bin = (val - min_meas) / bin_width;
        if (bin >= 0 && bin < bin_count) {
            local_bin_counts[bin]++;
        }
    }

    vector<int> global_bin_counts(bin_count, 0);
    MPI_Reduce(local_bin_counts.data(), global_bin_counts.data(), bin_count, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (my_rank == 0) {
        cout << "Histograma final:" << endl;
        for (int i = 0; i < bin_count; ++i) {
            float bin_start = min_meas + i * bin_width;
            float bin_end = bin_start + bin_width;
            cout << "Bin " << i << " [" << bin_start << ", " << bin_end << "): " << global_bin_counts[i] << " elementos" << endl;
        }
    }

    MPI_Finalize();
    return 0;
}