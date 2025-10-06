#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>

using namespace std;
using namespace std::chrono;


const long long num_steps = 100000000; 

const int num_threads = 4; 

double sum = 0.0;


void calculate_pi_sequential() {
    sum = 0.0;
    double step = 1.0 / num_steps;
    for (long long i = 0; i < num_steps; ++i) {
        double x = (i + 0.5) * step;
        sum += 4.0 / (1.0 + x * x);
    }
    double pi = sum * step;
    cout << "  PI (Secuencial): " << setprecision(15) << pi << endl;
}

int main() {
    cout << "Calculando PI con " << num_steps << " pasos y " << num_threads << " hilos." << endl;

    auto start_time = high_resolution_clock::now();
    calculate_pi_sequential();
    auto end_time = high_resolution_clock::now();
    duration<double> diff = end_time - start_time;
    cout << "  Tiempo: " << diff.count() << " s\n" << endl;

    return 0;
}