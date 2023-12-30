#pragma once

#include <condition_variable>
#include <memory>
#include <mutex>
#include <queue>

template <typename T>
class threadsafe_queue
{
public:
    threadsafe_queue() {}
    threadsafe_queue &operator=(threadsafe_queue const &) = delete; // excluding assignment for simplicity
    threadsafe_queue(threadsafe_queue const &other)
    {
        std::scoped_lock lock(other.mutex);
        data_queue = other.data_queue;
    }

    void push(T new_value)
    {
        std::scoped_lock lock(mutex);
        data_queue.push(std::move(new_value));
        data_cond.notify_one();
    }

    void wait_and_pop(T &value)
    {
        std::unique_lock lock(mutex);
        data_cond.wait(lock, [this]
                       { return !data_queue.empty(); });
        value = data_queue.front();
        data_queue.pop();
    }

    std::shared_ptr<T> wait_and_pop()
    {
        std::unique_lock lock(mutex);
        data_cond.wait(lock, [this]
                       { return !data_queue.empty(); });
        std::shared_ptr<T> res = std::make_shared<T>(data_queue.front());
        data_queue.pop();
        return res;
    }

    bool try_pop(T &value)
    {
        std::scoped_lock lock(mutex);
        if (data_queue.empty())
            return false;
        value = data_queue.front();
        data_queue.pop();
        return true;
    }

    std::shared_ptr<T> try_pop()
    {
        std::scoped_lock lock(mutex);
        if (data_queue.empty())
            return nullptr;
        std::shared_ptr<T> res = std::make_shared<T>(data_queue.front());
        data_queue.pop();
        return res;
    }

    bool empty() const
    {
        std::scoped_lock lock(mutex);
        return data_queue.empty();
    }

private:
    mutable std::mutex mutex;
    std::queue<T> data_queue;
    std::condition_variable data_cond;
};