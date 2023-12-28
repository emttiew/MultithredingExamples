#pragma once

#include <exception>
#include <iostream>
#include <memory>
#include <mutex>
#include <stack>

struct empty_stack : std::exception
{
    const char *what() const noexcept;
};

template <typename T>
class threadsafe_stack
{
public:
    threadsafe_stack() {}
    threadsafe_stack &operator=(const threadsafe_stack &) = delete;
    threadsafe_stack(const threadsafe_stack &other)
    {
        std::scoped_lock lock(other.m);
        data = other.data;
    }

    void push(T new_value) // using passing-by-value to avoid code bloat but gaining additional move operation
    {
        std::scoped_lock lock(m);
        data.push(std::move(new_value));
    }

    void pop(T &value) // combine top() and pop() functions to ensure thread safety
    {
        std::scoped_lock lock(m);
        if (data.empty())
            throw empty_stack();
        value = data.top(); // copy data before removing it from the stack
        data.pop();
    }

    std::shared_ptr<T> pop()
    {
        std::scoped_lock lock(m);
        if (data.empty())
            throw empty_stack();
        auto const res = std::make_shared<T>(data.top()); // allocate data before removing it from the stack
        data.pop();
        return res;
    }

    bool empty()
    {
        std::scoped_lock lock(m);
        return data.empty();
    }

private:
    std::stack<T> data;
    mutable std::mutex m;
};