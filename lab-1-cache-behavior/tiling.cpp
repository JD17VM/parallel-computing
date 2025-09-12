#include <iostream>
#include <vector>
#include <chrono>

using namespace std;

const int N = 1024;

vector<vector<double>> A(N, vector<double>(N));
vector<vector<double>> B(N, vector<double>(N));
vector<vector<double>> C(N, vector<double>(N));

void initialize_matrices() {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            A[i][j] = i + j;
            B[i][j] = i - j;
            C[i][j] = 0;
        }
    }
}

void matrix_multiply_classic() {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            for (int k = 0; k < N; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

int main() {
    initialize_matrices();

    auto start_time = chrono::high_resolution_clock::now();
    matrix_multiply_classic();
    auto end_time = chrono::high_resolution_clock::now();

    chrono::duration<double, milli> duration = end_time - start_time;

    cout << "Multiplicacion N=" << N << ": " << duration.count() << " ms" << endl;

    return 0;
}