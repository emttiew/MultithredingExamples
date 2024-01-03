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
    std::packaged_task<int()> task(std::bind(sum, 5, 5));
    std::future result = task.get_future();

    std::thread task_td(std::move(task));
    task_td.join();

    std::cout << "result: " <<  result.get() << std::endl;

    return 0;
}