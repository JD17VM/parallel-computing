#include <iostream>
#include <vector>
#include <mpi.h>

using namespace std;

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int comm_sz, my_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    const int n = 120;
    vector<double> A;
    vector<double> x;

    if (my_rank == 0) {
        A.resize(n * n);
        x.resize(n);
        for(int i = 0; i < n * n; ++i) A[i] = 1.0;
        for(int i = 0; i < n; ++i) x[i] = 1.0;
    }

    int local_n = n / comm_sz;
    vector<double> local_A(n * local_n);
    vector<double> local_x(local_n);
    
    vector<int> sendcounts(comm_sz, n * local_n);
    vector<int> displs(comm_sz);
    for(int i = 0; i < comm_sz; ++i) displs[i] = i * local_n;

    vector<int> sendcounts_x(comm_sz, local_n);
    vector<int> displs_x(comm_sz);
    for(int i = 0; i < comm_sz; ++i) displs_x[i] = i * local_n;
    
    MPI_Datatype col, col_type;
    MPI_Type_vector(n, local_n, n, MPI_DOUBLE, &col);
    MPI_Type_commit(&col);
    MPI_Type_create_resized(col, 0, local_n * sizeof(double), &col_type);
    MPI_Type_commit(&col_type);

    if(my_rank == 0){
        MPI_Scatterv(A.data(), sendcounts.data(), displs.data(), col_type,
                     local_A.data(), n * local_n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    } else {
        MPI_Scatterv(nullptr, nullptr, nullptr, col_type,
                     local_A.data(), n*local_n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    }

    MPI_Scatter(x.data(), local_n, MPI_DOUBLE, local_x.data(), local_n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    MPI_Type_free(&col);
    MPI_Type_free(&col_type);
    MPI_Finalize();
    return 0;
}