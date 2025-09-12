#include <iostream>
#include <vector>

using namespace std;

const int N = 500;

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


int main() {
    initialize_matrices();

    return 0;
}