#pragma once

#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <utility>
#include <vector>

#include "safe_queue.h"

using task_t = std::function<void()>;

class ThreadPool {
private:
    class ThreadWorker {
    private:
        int id_;
        ThreadPool* tpool_;

    public:
        ThreadWorker(ThreadPool* pool, const int id) : tpool_(pool), id_(id) {
        }
        void operator()() {
            task_t task;
            bool dequeued;
            while (!tpool_->shutdown_) {
                {
                    std::unique_lock<std::mutex> lock(tpool_->cv_mutex_);
                    if (tpool_->task_queue_.empty()) {
                        tpool_->cv_.wait(lock);
                    }
                    dequeued = tpool_->task_queue_.dequeue(task);
                }
                if (dequeued) {
                    task();
                }
            }
        }
    };

    bool shutdown_;
    SafeQueue<task_t> task_queue_;
    std::vector<std::thread> threads_;
    std::mutex cv_mutex_;
    std::condition_variable cv_;

public:
    ThreadPool(const int n_threads) : threads_(std::vector<std::thread>(n_threads)), shutdown_(false) {
    }
    ThreadPool(const ThreadPool&) = delete;
    ThreadPool(ThreadPool&&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&&) = delete;

    void init() {
        for (int i = 0; i < threads_.size(); i++) {
            threads_[i] = std::thread(ThreadWorker(this, i));
        }
    }

    void shutdown() {
        shutdown_ = true;
        cv_.notify_all();

        for (int i = 0; i < threads_.size(); ++i) {
            if (threads_[i].joinable()) {
                threads_[i].join();
            }
        }
    }

    template <typename F, typename... Args>
    auto submit(F&& f, Args&&... args) -> std::future<decltype(f(args...))> {
        std::future<decltype(f(args...))()> func = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
        auto task_ptr = std::make_shared<std::packaged_task<decltype(f(args...))()>>(func);

        std::function<void()> wrapper_func = [task_ptr]() { (*task_ptr)(); };

        task_queue_.enqueue(wrapper_func);

        cv_.notify_one();

        return task_ptr->get_future();
    }
};