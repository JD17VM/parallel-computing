#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <chrono>
#include <iomanip>

using namespace std;
using namespace std::chrono;


const long long num_steps = 100000000; 

const int num_threads = 4; 

double sum = 0.0;

volatile int flag = 0;

mutex mtx;


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


// --- 2. BUSY-WAITING (DENTRO DEL BUCLE) ---

void worker_busy_inside(long long start, long long end, int thread_id, double step) {
    for (long long i = start; i < end; ++i) {
        double x = (i + 0.5) * step;
        double partial_sum = 4.0 / (1.0 + x * x);
        
        while (flag != thread_id); 
        sum += partial_sum;

        flag = (thread_id + 1) % num_threads;
    }
}

void calculate_pi_busy_inside() {
    sum = 0.0;
    flag = 0;
    double step = 1.0 / num_steps;
    vector<thread> threads;
    long long steps_per_thread = num_steps / num_threads;

    for (int i = 0; i < num_threads; ++i) {
        long long start = i * steps_per_thread;
        long long end = (i == num_threads - 1) ? num_steps : start + steps_per_thread;
        threads.emplace_back(worker_busy_inside, start, end, i, step);
    }

    for (auto& t : threads) {
        t.join();
    }

    while(flag != 0);

    double pi = sum * step;
    cout << "  PI (Busy-Wait Dentro): " << setprecision(15) << pi << endl;
}


// --- 3. BUSY-WAITING (FUERA DEL BUCLE) ---
void worker_busy_outside(long long start, long long end, int thread_id, double step) {
    double local_sum = 0.0;
    for (long long i = start; i < end; ++i) {
        double x = (i + 0.5) * step;
        local_sum += 4.0 / (1.0 + x * x);
    }

    while (flag != thread_id);
    sum += local_sum;
    flag = (thread_id + 1) % num_threads;
}

void calculate_pi_busy_outside() {
    sum = 0.0;
    flag = 0;
    double step = 1.0 / num_steps;
    vector<thread> threads;
    long long steps_per_thread = num_steps / num_threads;

    for (int i = 0; i < num_threads; ++i) {
        long long start = i * steps_per_thread;
        long long end = (i == num_threads - 1) ? num_steps : start + steps_per_thread;
        threads.emplace_back(worker_busy_outside, start, end, i, step);
    }

    for (auto& t : threads) {
        t.join();
    }
    
    while(flag != 0);

    double pi = sum * step;
    cout << "  PI (Busy-Wait Fuera): " << setprecision(15) << pi << endl;
}


// --- 4. MUTEX ---
void worker_mutex(long long start, long long end, double step) {
    double local_sum = 0.0;

    for (long long i = start; i < end; ++i) {
        double x = (i + 0.5) * step;
        local_sum += 4.0 / (1.0 + x * x);
    }

    lock_guard<mutex> guard(mtx);
    sum += local_sum;
}

void calculate_pi_mutex() {
    sum = 0.0;
    double step = 1.0 / num_steps;
    vector<thread> threads;
    long long steps_per_thread = num_steps / num_threads;

    for (int i = 0; i < num_threads; ++i) {
        long long start = i * steps_per_thread;
        long long end = (i == num_threads - 1) ? num_steps : start + steps_per_thread;
        threads.emplace_back(worker_mutex, start, end, step);
    }

    for (auto& t : threads) {
        t.join();
    }

    double pi = sum * step;
    cout << "  PI (Mutex): " << setprecision(15) << pi << endl;
}




int main() {
    cout << "Calculando PI con " << num_steps << " pasos y " << num_threads << " hilos." << endl;

    auto start_time = high_resolution_clock::now();
    calculate_pi_sequential();
    auto end_time = high_resolution_clock::now();
    duration<double> diff = end_time - start_time;
    cout << "  Tiempo: " << diff.count() << " s\n" << endl;

    start_time = high_resolution_clock::now();
    calculate_pi_busy_inside();
    end_time = high_resolution_clock::now();
    diff = end_time - start_time;
    cout << "  Tiempo: " << diff.count() << " s\n" << endl;

    start_time = high_resolution_clock::now();
    calculate_pi_busy_outside();
    end_time = high_resolution_clock::now();
    diff = end_time - start_time;
    cout << "  Tiempo: " << diff.count() << " s\n" << endl;

    start_time = high_resolution_clock::now();
    calculate_pi_mutex();
    end_time = high_resolution_clock::now();
    diff = end_time - start_time;
    cout << "  Tiempo: " << diff.count() << " s\n" << endl;

    return 0;
}