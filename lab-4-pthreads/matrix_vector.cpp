#include "matrix_vector.h"
#include <iostream>
#include <vector>
#include <pthread.h>
#include <random>
#include <chrono>

using namespace std;

void* mat_vect_thread_func(void* args) {
    ThreadData* data = (ThreadData*)args;
    
    int n = data->A->size();
    int m = (*data->x).size();
    
    int rows_per_thread = n / data->num_threads;
    int start_row = data->thread_id * rows_per_thread;
    int end_row = (data->thread_id == data->num_threads - 1) ? n : start_row + rows_per_thread;

    for (int i = start_row; i < end_row; ++i) {
        (*data->y)[i] = 0.0;
        for (int j = 0; j < m; ++j) {
            (*data->y)[i] += (*data->A)[i][j] * (*data->x)[j];
        }
    }
    
    return NULL;
}

void parallel_matrix_vector_mult(int num_threads, int n, int m) {
    vector<vector<double>> A(n, vector<double>(m));
    vector<double> x(m);
    vector<double> y(n);

    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(0.0, 1.0);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            A[i][j] = dis(gen);
        }
    }
    for (int j = 0; j < m; ++j) {
        x[j] = dis(gen);
    }

    pthread_t* threads = new pthread_t[num_threads];
    ThreadData* thread_args = new ThreadData[num_threads];

    auto start_time = chrono::high_resolution_clock::now();

    for (int i = 0; i < num_threads; ++i) {
        thread_args[i] = {i, num_threads, &A, &x, &y};
        pthread_create(&threads[i], NULL, mat_vect_thread_func, (void*)&thread_args[i]);
    }

    for (int i = 0; i < num_threads; ++i) {
        pthread_join(threads[i], NULL);
    }

    auto end_time = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end_time - start_time;
    cout << "Multiplicacion Matriz-Vector (" << n << "x" << m << ") con " << num_threads << " threads tomo " << elapsed.count() << " segundos." << endl;

    delete[] threads;
    delete[] thread_args;
}