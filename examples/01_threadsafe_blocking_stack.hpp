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
    
    void push(T new_value)
    {
        std::scoped_lock lock(m);
        data.push(std::move(new_value));
    }

    void pop(T &value)
    {
        std::scoped_lock lock(m);
        if (data.empty())
            throw empty_stack();
        value = data.top();
        data.pop();
    }

    std::shared_ptr<T> pop()
    {
        std::scoped_lock lock(m);
        if (data.empty())
            throw empty_stack();
        auto const res = std::make_shared<T>(data.top());
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