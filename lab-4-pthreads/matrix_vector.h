#ifndef MATRIX_VECTOR_H
#define MATRIX_VECTOR_H

#include <vector>

struct ThreadData {
    int thread_id;
    int num_threads;
    const std::vector<std::vector<double>>* A; 
    const std::vector<double>* x;             
    std::vector<double>* y;                
};

void parallel_matrix_vector_mult(int num_threads, int n, int m);

void* mat_vect_thread_func(void* args);

#endif