#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>
#include <string>
#include <omp.h>
#include <numeric>
#include <algorithm>

double approximate_pi(long long n, int thread_count, const std::string& schedule, int block_size)
{
    double sum = 0.0;
    double factor;

    if (schedule == "auto") {
        #pragma omp parallel for num_threads(thread_count) reduction(+:sum) private(factor) schedule(auto)
        for (long long k = 0; k < n; k++) {
            factor = (k % 2 == 0) ? 1.0 : -1.0;
            sum += factor / (2 * k + 1);
        }
    } else {
        #pragma omp parallel for num_threads(thread_count) reduction(+:sum) private(factor) schedule(runtime)
        for (long long k = 0; k < n; k++) {
            factor = (k % 2 == 0) ? 1.0 : -1.0;
            sum += factor / (2 * k + 1);
        }
    }

    return 4.0 * sum;
}

void run_experiment(long long n, int threads, const std::string& schedule, int block_size, int num_runs)
{
    std::vector<double> times;
    double pi_sum = 0.0;

    for (int i = 0; i < num_runs; i++) {
        double start_time = omp_get_wtime();
        double pi = approximate_pi(n, threads, schedule, block_size);
        double end_time = omp_get_wtime();

        times.push_back(end_time - start_time);
        pi_sum += pi;
    }

    double avg_time = std::accumulate(times.begin(), times.end(), 0.0) / num_runs;
    double avg_pi = pi_sum / num_runs;
    double error = std::abs(avg_pi - M_PI);

    std::cout << std::setw(10) << n
              << std::setw(8) << threads
              << std::setw(10) << schedule
              << std::setw(12) << block_size
              << std::setw(20) << std::setprecision(10) << avg_pi
              << std::setw(20) << std::setprecision(10) << error
              << std::setw(15) << std::setprecision(6) << avg_time << std::endl;
}

int main()
{
    const int NUM_CORES = 16;  
    const int NUM_RUNS = 5;
    const long long N = 1000000; 

    std::vector<std::string> schedules = {"static", "dynamic", "guided", "auto"};
    std::vector<int> block_sizes = {16, 64, 128};

    std::cout << std::setw(10) << "N"
              << std::setw(8) << "Threads"
              << std::setw(10) << "Schedule"
              << std::setw(12) << "Block Size"
              << std::setw(20) << "Avg PI"
              << std::setw(20) << "Avg Error"
              << std::setw(15) << "Avg Time (s)" << std::endl;
    std::cout << std::string(95, '-') << std::endl;

    for (const auto& schedule : schedules) {
        if (schedule == "auto") {
            run_experiment(N, NUM_CORES, schedule, 0, NUM_RUNS);
        } else {
            for (int block_size : block_sizes) {
                std::string env_var = "OMP_SCHEDULE=" + schedule + "," + std::to_string(block_size);
                putenv(const_cast<char*>(env_var.c_str()));
                run_experiment(N, NUM_CORES, schedule, block_size, NUM_RUNS);
            }
        }
    }

    return 0;
}