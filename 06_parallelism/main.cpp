#include <iostream>
#include <cmath>
#include <future>
#include <vector>
#include <iomanip>
#include <mutex>
#include <chrono>

std::mutex mutex;

double calculatePi(int terms, int start, int skip)
{
    double result{0};
    for (int i = start; i < terms; i += skip)
    {
        int sign = std::pow(-1, i);
        double term = 1.0 / (i * 2 + 1);
        result += sign * term;
    }

    // std::lock_guard<std::mutex> lk(mutex);
    // std::cout << std::setprecision(30) << start << " - " << result * 4 << std::endl;
    return result * 4;
}

int main()
{
    int terms = 1'000'000'000;
    int hardware_threads = std::thread::hardware_concurrency();
    std::cout << "hardware thread count - " << hardware_threads << std::endl;
    std::vector<std::future<double>> futures;

    auto start = std::chrono::high_resolution_clock::now();
    for (int thread = 0; thread < hardware_threads; thread++)
    {
        std::future<double> future = std::async(std::launch::async, 
            calculatePi, terms, thread, hardware_threads);
        futures.push_back(std::move(future));
    }

    double total = 0;
    for (auto& f : futures)
        total += f.get();
    
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration<double>(end - start);
    std::cout << "Parallel execution time: " << elapsed.count() << " seconds\n";
    std::cout << std::setprecision(30) << total << std::endl;

    start = std::chrono::high_resolution_clock::now();
    std::cout << std::setprecision(30) << calculatePi(terms, 0, 1) << std::endl;
    end = std::chrono::high_resolution_clock::now();
    elapsed = end - start;
    std::cout << "Single-thread execution time: " << elapsed.count() << " seconds\n";
}