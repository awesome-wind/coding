#pragma once

#include <condition_variable>
#include <deque>
#include <functional>
#include <thread>
#include <vector>

class ThreadPool {
public:
    using task_t = std::function<void()>;

    ThreadPool(int init_size);
    ~ThreadPool();

    void stop();
    void add_task(const task_t&);

private:
    ThreadPool(const ThreadPool&) = delete;
    const ThreadPool& operator=(const ThreadPool&) = delete;

    int size_;
    bool is_started_;

    task_t take_task();
    void thread_loop();
    void start();

    std::vector<std::thread*> threads_;
    std::deque<task_t> tasks_;

    std::mutex mutex_;
    std::condition_variable cv_;
};