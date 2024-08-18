#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>
#include <chrono>

float approximate_pi(int n)
{
    double factor = 1.0;
    double sum = 0.0;
    for (int k = 0; k < n; k++)
    {
        sum += factor / (2 * k + 1);
        factor = -factor;
    }

    return 4.0 * sum;
}

int main()
{
    std::vector<int> iterations = {1000, 10000, 100000, 1000000, 10000000};

    std::cout << std::setw(12) << "Iterations" << std::setw(20) << "Approximation"
              << std::setw(20) << "Error" << std::setw(20) << "Time (s)" << std::endl;
    std::cout << std::string(72, '-') << std::endl;

    for (int n : iterations)
    {
        try
        {
            auto start = std::chrono::high_resolution_clock::now();

            float pi = approximate_pi(n);

            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> duration = end - start;

            float error = std::abs(pi - M_PI);

            std::cout << std::setw(12) << n
                      << std::setw(20) << std::setprecision(10) << pi
                      << std::setw(20) << std::setprecision(10) << error
                      << std::setw(20) << std::setprecision(6) << duration.count() << std::endl;
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error for n = " << n << ": " << e.what() << std::endl;
        }
    }

    return 0;
}