#include <barrier>
#include <iostream>
#include <string>
#include <syncstream>
#include <thread>
#include <vector>

/*
    The class template std::barrier provides a thread-coordination mechanism
    that blocks a group of threads of known size until all threads in that group have reached the barrier.
    Unlike std::latch, barriers are reusable: once a group of arriving threads are unblocked, the barrier can be reused.
    Unlike std::latch, barriers execute a possibly empty callable before unblocking threads.
*/

int main()
{
    const auto numbers = {1, 2, 3};

    auto on_completion = []() noexcept
    {
        // locking not needed here
        static auto phase =
            "... done\n"
            "Do other work on first completition...\n";
        std::cout << phase;
        phase = "... done\n";
    };

    std::barrier sync_point(std::ssize(numbers), on_completion);

    auto work = [&](unsigned num)
    {
        std::osyncstream(std::cout) << "working on: " << num << std::endl;
        sync_point.arrive_and_wait();

        std::osyncstream(std::cout) << "more stuff" << std::endl;
        sync_point.arrive_and_wait();
    };

    std::cout << "Starting...\n";
    std::vector<std::jthread> threads;
    threads.reserve(std::size(numbers));
    for (auto const num : numbers)
        threads.emplace_back(work, num);
}

/*
Output:

    Starting...
    working on: 3
    working on: 2
    working on: 1
    ... done
    Do other work on first completition...
    more stuff
    more stuff
    more stuff
    ... done
*/