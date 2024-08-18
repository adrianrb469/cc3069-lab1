#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>
#include <omp.h>

float approximate_pi(int n, int thread_count)
{
    double sum = 0.0;
#pragma omp parallel num_threads(thread_count)
    {
        double factor = 1.0;
#pragma omp for reduction(+ : sum)
        for (int k = 0; k < n; k++)
        {
            sum += factor / (2 * k + 1);
            factor = -factor;
        }
    }

    return 4.0 * sum;
}

int main()
{
    std::vector<int> iterations = {1000, 10000, 100000, 1000000, 10000000};
    std::vector<int> thread_counts = {2, 4, 6, 8, 10}; 

    std::cout << std::setw(12) << "Iterations" << std::setw(12) << "Threads"
              << std::setw(20) << "Approximation" << std::setw(20) << "Error"
              << std::setw(20) << "Time (s)" << std::endl;
    std::cout << std::string(84, '-') << std::endl;

    for (int n : iterations)
    {
        for (int threads : thread_counts)
        {
            try
            {
                double start_time = omp_get_wtime();
                float pi = approximate_pi(n, threads);
                double end_time = omp_get_wtime();
                double elapsed_time = end_time - start_time;

                float error = std::abs(pi - M_PI);

                std::cout << std::setw(12) << n
                          << std::setw(12) << threads
                          << std::setw(20) << std::setprecision(10) << pi
                          << std::setw(20) << std::setprecision(10) << error
                          << std::setw(20) << std::setprecision(6) << elapsed_time << std::endl;
            }
            catch (const std::exception &e)
            {
                std::cerr << "Error for n = " << n << ", threads = " << threads << ": " << e.what() << std::endl;
            }
        }
        std::cout << std::string(84, '-') << std::endl;
    }

    return 0;
}