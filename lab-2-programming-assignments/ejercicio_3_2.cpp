#include <iostream>
#include <random>
#include <mpi.h>

using namespace std;

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int comm_sz, my_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    long long number_of_tosses = 100000;

    if (my_rank == 0) {
        cout << "Usando " << number_of_tosses << " lanzamientos totales." << endl;
    }

    MPI_Bcast(&number_of_tosses, 1, MPI_LONG_LONG, 0, MPI_COMM_WORLD);

    long long local_number_of_tosses = number_of_tosses / comm_sz;
    long long number_in_circle = 0;

    random_device rd;
    mt19937 gen(rd() + my_rank);
    uniform_real_distribution<> dis(-1.0, 1.0);

    for (long long toss = 0; toss < local_number_of_tosses; toss++) {
        double x = dis(gen);
        double y = dis(gen);
        double distance_squared = x * x + y * y;
        if (distance_squared <= 1) {
            number_in_circle++;
        }
    }

    MPI_Finalize();
    return 0;
}