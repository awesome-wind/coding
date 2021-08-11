#pragma once

#include <condition_variable>
#include <deque>
#include <functional>
#include <mutex>
#include <thread>
#include <vector>

namespace thread_pool {

static std::string get_tid();

class ThreadPool {
public:
    using task_t = std::function<void()>;

    ThreadPool(int init_size = 3);
    ~ThreadPool();

    void stop();
    void add_task(const task_t&);

private:
    ThreadPool(const ThreadPool&) = delete;
    const ThreadPool& operator=(const ThreadPool&) = delete;

    bool is_started() {
        return is_started_;
    }
    void start();

    void thread_loop();
    task_t take();

    int thread_size_ = 0;

    std::vector<std::thread*> threads_;
    std::deque<task_t> tasks_;

    std::mutex mutex_;
    std::condition_variable cv_;
    bool is_started_;
};

}  // namespace thread_pool
