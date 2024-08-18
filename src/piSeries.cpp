#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>
#include <omp.h>

double approximate_pi(long long n, int thread_count)
{
    double sum = 0.0;
    double factor;
    #pragma omp parallel for num_threads(thread_count) reduction(+:sum) private(factor)
    for (long long k = 0; k < n; k++)
    {
        factor = (k % 2 == 0) ? 1.0 : -1.0;
        sum += factor / (2 * k + 1);
    }
    return 4.0 * sum;
}

void run_experiment(long long n, int threads, int num_runs)
{
    std::cout << "\nRunning experiment with " << n << " iterations and " << threads << " threads:" << std::endl;
    std::cout << std::string(85, '-') << std::endl;
    std::cout << std::setw(15) << "Iterations"
              << std::setw(10) << "Threads"
              << std::setw(20) << "PI"
              << std::setw(20) << "Error"
              << std::setw(20) << "Time (s)" << std::endl;
    std::cout << std::string(85, '-') << std::endl;


    double total_time = 0;

    for (int i = 0; i < num_runs; i++)
    {
        double start_time = omp_get_wtime();
        double pi = approximate_pi(n, threads);
        double end_time = omp_get_wtime();
        
        double error = std::abs(pi - M_PI);
        double time = end_time - start_time;
        total_time += time;

        std::cout << std::setw(15) << n
                  << std::setw(10) << threads
                  << std::setw(20) << std::setprecision(10) << pi
                  << std::setw(20) << std::setprecision(10) << error
                  << std::setw(20) << std::setprecision(6) << time << std::endl;
    }

    double avg_time = total_time / num_runs;

    std::cout << "Average Time (s): " << std::setprecision(6) << avg_time << std::endl;
}

int main()
{
    const int NUM_RUNS = 5;

    std::vector<std::pair<int, long long>> configurations = {
        {1, 1000000},    // threads 1, iteraciones 10^6
        {16, 1000000},   // threads 16, iteraciones 10^6
        {32, 1000000},   // threads 32, iteraciones 10^6
        {16, 10000000}   // threads 16, iteraciones 10^7
    };

    for (const auto& config : configurations)
    {
        int threads = config.first;
        long long iterations = config.second;
        run_experiment(iterations, threads, NUM_RUNS);
    }

    return 0;
}