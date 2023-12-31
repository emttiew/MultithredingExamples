#include <chrono>
#include <future>
#include <iostream>
#include <thread>

int wait_for_a_while_and_return_sum(int a, int b)
{
    std::cout << "waiting..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(5));
    return a + b;
}

int main()
{
    {
        auto start = std::chrono::high_resolution_clock::now();
        std::cout << "call wait function." << std::endl;
        int sum = wait_for_a_while_and_return_sum(10, 10);
        std::cout << "call wait function second time." << std::endl;
        sum += wait_for_a_while_and_return_sum(10, 10);
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::seconds>(stop - start);
        std::cout << "Result: " << sum << std::endl;
        std::cout << "Time taken by synchronous approach: "
                  << duration.count() << " seconds" << std::endl;
    }

    {
        auto start = std::chrono::high_resolution_clock::now();
        std::cout << "call wait function." << std::endl;
        std::future work = std::async(&wait_for_a_while_and_return_sum, 10, 10);
        std::cout << "call wait function second time." << std::endl;
        int sum = wait_for_a_while_and_return_sum(10, 10);
        sum += work.get();
        std::cout << "Result: " << sum << std::endl;
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::seconds>(stop - start);
        std::cout << "Time taken by asynchronous approach: "
                  << duration.count() << " seconds" << std::endl;
    }
    return 0;
}

// output:
// call wait function.
// waiting...
// call wait function second time.
// waiting...
// Result: 40
// Time taken by synchronous approach: 10 seconds

// call wait function.
// call wait function second time.
// waiting...
// waiting...
// Result: 40
// Time taken by asynchronous approach: 5 seconds