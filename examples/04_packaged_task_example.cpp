#include <chrono>
#include <future>
#include <iostream>
#include <thread>
#include <functional>

int sum(int a, int b)
{
    return a + b;
}

int main()
{
    std::packaged_task<int()> task(std::bind(sum, 5, 5));  // wrap the function in packaged_task
    std::future result = task.get_future();                // save the future to use it later in the code

    std::thread task_td(std::move(task));                  // move the packaged function to be processed in another thread
    task_td.join();

    std::cout << "result: " <<  result.get() << std::endl; // get the value using the future object

    return 0;
}