#include <future>
#include <iostream>
#include <latch>
#include <string>
#include <thread>
#include <vector>

/*
    Latches are useful for coordinating the execution of different threads,
    ensuring that certain tasks or computations are completed before others begin.
*/

void do_work(unsigned num)
{
    std::cout << "working on: " << num << std::endl;
}

void do_more_stuff()
{
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << "more stuff" << std::endl;
}

int main()
{
    unsigned const thread_count = 3;
    std::latch work_done(thread_count);

    std::vector<std::future<void>> threads;

    for (unsigned i = 0; i < thread_count; ++i)
    {
        threads.push_back(std::async(std::launch::async, [&, i]
                                     {
            do_work(i);
            work_done.count_down();
            do_more_stuff(); }));
    }

    work_done.wait();
    std::cout << "work done" << std::endl;

    return 0;
}

// output:
// working on: 1
// working on: 2
// working on: 0
// work done
// more stuff
// more stuff
// more stuff