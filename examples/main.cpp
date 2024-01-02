#include "01_threadsafe_blocking_stack.hpp"
#include "02_threadsafe_blocking_queue.hpp"

int main()
{
    threadsafe_stack<int> a;
    threadsafe_stack<int> b(a);

    threadsafe_stack<int> c(std::move(b));
    threadsafe_stack<int> d = std::move(c);
    threadsafe_stack<int> e = d;

    threadsafe_queue<int> q;
    return 0;
}