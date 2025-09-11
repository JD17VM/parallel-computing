#include <iostream>
#include <vector>
#include <chrono>

using namespace std;

const int N = 1000;

vector<vector<double>> A(N, vector<double>(N));
vector<double> x(N);
vector<double> y(N);

void initialize_data() {
    for (int i = 0; i < N; ++i) {
        x[i] = i; 
        y[i] = 0;
        for (int j = 0; j < N; ++j) {
            A[i][j] = i + j; 
        }
    }
}

void algorithm_row_major() { // Acceso por filas
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            y[i] += A[i][j] * x[j];
        }
    }
}

void algorithm_column_major() { // Acceso por columnas
    for (int j = 0; j < N; ++j) {
        for (int i = 0; i < N; ++i) {
            y[i] += A[i][j] * x[j];
        }
    }
}

int main() {
    initialize_data();

    auto start_row = chrono::high_resolution_clock::now();
    algorithm_row_major();
    auto end_row = chrono::high_resolution_clock::now();
    
    chrono::duration<double, milli> duration_row = end_row - start_row;
    cout << "Algoritmo por Filas (Row-Major) N=" << N << ": " << duration_row.count() << " ms" << endl;

    initialize_data();

    auto start_col = chrono::high_resolution_clock::now();
    algorithm_column_major();
    auto end_col = chrono::high_resolution_clock::now();
    
    chrono::duration<double, milli> duration_col = end_col - start_col;
    cout << "Algoritmo por Columnas (Column-Major) N=" << N << ": " << duration_col.count() << " ms" << endl;

    return 0;
}