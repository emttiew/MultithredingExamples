#include "06_simple_parallel_quick_sort.hpp"
#include <chrono>
#include <iostream>
#include <random>
#include <thread>

int main()
{
    std::list<int> list{5, 7, 10, 11, 12, 1, 3, 6, 4, 2};
    std::random_device rd;
    std::mt19937 gen(rd());

    std::list<int> randomNumbers;
    std::uniform_int_distribution<> dis(1, 100000);
    int numberOfElements = 1000;

    for (int i = 0; i < numberOfElements; ++i)
    {
        randomNumbers.push_back(dis(gen));
    }

    {
        auto start = std::chrono::high_resolution_clock::now();
        std::list<int> sequential_sorted = sequential_quick_sort(randomNumbers);
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
        std::cout << "Time taken by synchronous approach: "
                  << duration.count() << " miliseconds" << std::endl;
    }
    {
        auto start = std::chrono::high_resolution_clock::now();
        std::list<int> parallel_sorted = parallel_quick_sort(randomNumbers);
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
        std::cout << "Time taken by asynchronous approach: "
                  << duration.count() << " miliseconds" << std::endl;
    }

    return 0;
}

// Time taken by synchronous approach: 0 miliseconds
// Time taken by asynchronous approach: 54 miliseconds