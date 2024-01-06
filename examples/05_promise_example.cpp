#include <chrono>
#include <functional>
#include <future>
#include <iostream>
#include <thread>

struct Data
{
    int a;
};

void set_data(std::promise<Data> &data_promise)
{
    data_promise.set_value({5});
}

int main()
{
    std::promise<Data> data_promise;
    std::thread data_td(set_data, std::ref(data_promise));
    data_td.join();

    std::future result = data_promise.get_future();
    std::cout << "result: " << result.get().a << std::endl;

    return 0;
}

// output:
// result: 5