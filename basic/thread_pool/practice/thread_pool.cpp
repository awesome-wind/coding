#include "thread_pool.h"

#include <cassert>
#include <chrono>
#include <iostream>
#include <mutex>
#include <sstream>
#include <thread>
#include <vector>

static std::string get_tid() {
    std::stringstream sm;
    sm << "current thread id :" << std::this_thread::get_id() << std::endl;
    return sm.str();
}

ThreadPool::ThreadPool(int size) : size_(size), is_started_(false) {
    start();
}

ThreadPool::~ThreadPool() {
    if (is_started_) {
        stop();
    }
}

void ThreadPool::start() {
    assert(threads_.empty());
    is_started_ = true;

    threads_.reserve(size_);
    for (int i = 0; i < size_; i++) {
        threads_.push_back(new std::thread(&ThreadPool::thread_loop, this));
    }
}

void ThreadPool::stop() {
    std::cout << "thread pool stop." << std::endl;
    {
        std::unique_lock<std::mutex> lock(mutex_);
        is_started_ = false;
        cv_.notify_all();
        std::cout << "stop notify all." << std::endl;
    }

    for (std::vector<std::thread*>::iterator it = threads_.begin(); it != threads_.end(); ++it) {
        (*it)->join();
        delete *it;
    }
    threads_.clear();
}

void ThreadPool::thread_loop() {
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    std::cout << "thread:" << get_tid() << " is running." << std::endl;
    while (is_started_) {
        task_t task = take_task();
        if (task) {
            task();
        }
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    std::cout << "thread:" << get_tid() << " stopped." << std::endl;
}

void ThreadPool::add_task(const task_t& task) {
    std::unique_lock<std::mutex> lock(mutex_);
    tasks_.push_back(task);
    cv_.notify_one();
}

ThreadPool::task_t ThreadPool::take_task() {
    std::unique_lock<std::mutex> lock(mutex_);
    while (tasks_.empty() && is_started_) {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        std::cout << "thread:" << get_tid() << " enter wait." << std::endl;
        cv_.wait(lock);
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    std::cout << "thread:" << get_tid() << " wakeup." << std::endl;

    task_t task;
    std::deque<task_t>::size_type size = tasks_.size();
    if (!threads_.empty() && is_started_) {
        task = tasks_.front();
        tasks_.pop_front();
        assert(size - 1 == tasks_.size());
    }
    return task;
}
