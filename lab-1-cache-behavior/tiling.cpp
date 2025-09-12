#include <iostream>
#include <vector>
#include <chrono>

using namespace std;

const int N = 1024; 

const int BLOCK_SIZE = 32;

vector<vector<double>> A(N, vector<double>(N));
vector<vector<double>> B(N, vector<double>(N));
vector<vector<double>> C(N, vector<double>(N));

void initialize_data() {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            A[i][j] = i + j;
            B[i][j] = i - j;
            C[i][j] = 0.0;
        }
    }
}

void multiply_classic() {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            for (int k = 0; k < N; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

void multiply_blocked() {
    for (int i0 = 0; i0 < N; i0 += BLOCK_SIZE) {
        for (int j0 = 0; j0 < N; j0 += BLOCK_SIZE) {
            for (int k0 = 0; k0 < N; k0 += BLOCK_SIZE) {
                for (int i = i0; i < i0 + BLOCK_SIZE; ++i) {
                    for (int j = j0; j < j0 + BLOCK_SIZE; ++j) {
                        for (int k = k0; k < k0 + BLOCK_SIZE; ++k) {
                            C[i][j] += A[i][k] * B[k][j];
                        }
                    }
                }
            }
        }
    }
}

int main() {
    initialize_data();
    
    auto start_classic = chrono::high_resolution_clock::now();
    multiply_classic();
    auto end_classic = chrono::high_resolution_clock::now();
    
    chrono::duration<double, milli> duration_classic = end_classic - start_classic;
    cout << "Algoritmo Clasico (3 bucles) N=" << N << ": " 
              << duration_classic.count() << " ms" << endl;

    initialize_data();

    auto start_blocked = chrono::high_resolution_clock::now();
    multiply_blocked();
    auto end_blocked = chrono::high_resolution_clock::now();
    
    chrono::duration<double, milli> duration_blocked = end_blocked - start_blocked;
    cout << "Algoritmo por Bloques (6 bucles) N=" << N << ": " 
              << duration_blocked.count() << " ms" << endl;

    return 0;
}