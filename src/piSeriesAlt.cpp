#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>
#include <omp.h>

double approximate_pi(long long n, int thread_count)
{
    double sum_even = 0.0;
    double sum_odd = 0.0;

    #pragma omp parallel num_threads(thread_count)
    {
        #pragma omp for reduction(+:sum_even, sum_odd)
        for (long long i = 0; i < n; i += 2) {
            sum_even += 1.0 / (2 * i + 1);
            if (i + 1 < n) {
                sum_odd += 1.0 / (2 * (i + 1) + 1);
            }
        }
    }

    return 4.0 * (sum_even - sum_odd);
}

void run_experiment(long long n, int threads, int num_runs)
{
    std::cout << "Experimento con n = " << n << ", threads = " << threads << std::endl;
    std::cout << std::string(60, '-') << std::endl;
    std::cout << std::setw(15) << "Iteración" << std::setw(20) << "PI Aproximado" 
              << std::setw(20) << "Error" << std::setw(15) << "Tiempo (s)" << std::endl;
    std::cout << std::string(70, '-') << std::endl;

    for (int i = 0; i < num_runs; i++)
    {
        double start_time = omp_get_wtime();
        double pi = approximate_pi(n, threads);
        double end_time = omp_get_wtime();
        
        double error = std::abs(pi - M_PI);
        double time = end_time - start_time;

        std::cout << std::setw(15) << i + 1
                  << std::setw(20) << std::setprecision(15) << pi
                  << std::setw(20) << std::setprecision(10) << error
                  << std::setw(15) << std::setprecision(6) << time << std::endl;
    }
    std::cout << std::endl;
}

int main()
{
    const int NUM_RUNS = 5;
    std::vector<std::pair<long long, int>> configurations = {
        {1000000, 2},
        {1000000, 4},
        {1000000, 8},
        {1000000, 16},
        {10000000, 16}
    };

    for (const auto& config : configurations)
    {
        run_experiment(config.first, config.second, NUM_RUNS);
    }

    return 0;
}