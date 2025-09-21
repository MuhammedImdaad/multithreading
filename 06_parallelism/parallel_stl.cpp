#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <random>
#include <execution>
#include <chrono>
#include <iomanip>
#include <functional>

// Helper function to print execution time and result
template <typename Func>
auto time_execution(const std::string &title, Func func)
{
    auto start = std::chrono::high_resolution_clock::now();
    func();
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << std::left << std::setw(25) << title << ": " << elapsed.count() << " seconds\n";
}

int main()
{
    const size_t size = 50'000'000;

    std::cout << "C++17 Parallel Algorithms - std::sort\n";

    // Generate random data
    std::cout << "Generating random data...\n";
    std::vector<int> v(size);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 100);
    auto generate_data = [&]()
    {
        std::generate(v.begin(), v.end(), [&]()
                      { return dis(gen); });
    };

    generate_data();
    std::vector<int> v_copy = v;

    // Sequential sort
    generate_data();
    time_execution("Sequential sort", [&]()
                   { std::sort(std::execution::seq, v.begin(), v.end()); });

    // Parallel sort
    v = v_copy;
    time_execution("Parallel sort", [&]()
                   { std::sort(std::execution::par, v.begin(), v.end()); });

    // Parallel unsequenced sort
    v = v_copy;
    time_execution("Parallel unseq sort", [&]()
                   { std::sort(std::execution::par_unseq, v.begin(), v.end()); });

    std::cout << "\nNote: Actual performance may vary based on hardware, compiler, and whether\n"
              << "      a parallel execution backend like Intel TBB is available.\n"
              << "      Some compilers may fall back to sequential execution if no\n"
              << "      parallel backend is available.\n";

    return 0;
}