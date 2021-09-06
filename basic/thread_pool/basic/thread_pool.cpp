#include "thread_pool.h"

#include <assert.h>

#include <functional>
#include <iostream>
#include <mutex>
#include <sstream>
#include <thread>

namespace thread_pool {

static std::string get_tid() {
    std::stringstream str;
    str << std::this_thread::get_id();
    return str.str();
}

ThreadPool::ThreadPool(int init_size) : thread_size_(init_size), is_started_(false) {
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

    threads_.reserve(thread_size_);
    for (int i = 0; i < thread_size_; i++) {
        // threads_.push_back(new std::thread(std::bind(&ThreadPool::thread_loop, this)));
        threads_.push_back(new std::thread(&ThreadPool::thread_loop, this));
    }
}

void ThreadPool::stop() {
    std::cout << "thread pool stop" << std::endl;
    {
        std::unique_lock<std::mutex> lock(mutex_);
        is_started_ = false;
        cv_.notify_all();
        std::cout << "thread pool stop notify all." << std::endl;
    }

    for (std::vector<std::thread*>::iterator it = threads_.begin(); it != threads_.end(); ++it) {
        (*it)->join();
        delete *it;
    }
    threads_.clear();
}

void ThreadPool::thread_loop() {
    std::cout << "tid :" << get_tid() << " start." << std::endl;
    while (is_started_) {
        task_t task = take();
        if (task) {
            task();
        }
    }
    std::cout << "tid : " << get_tid() << " exit." << std::endl;
}

void ThreadPool::add_task(const task_t& task) {
    std::unique_lock<std::mutex> lock(mutex_);
    tasks_.push_back(task);
    cv_.notify_one();
}

ThreadPool::task_t ThreadPool::take() {
    std::unique_lock<std::mutex> lock(mutex_);
    while (tasks_.empty() && is_started_) {
        std::cout << "method take(), tid : " + get_tid() << " wait." << std::endl;
        cv_.wait(lock);
    }

    std::cout << "method take(), tid :" << get_tid() << " wakeup." << std::endl;

    task_t task;
    std::deque<task_t>::size_type size = tasks_.size();
    if (!tasks_.empty() && is_started_) {
        task = tasks_.front();
        tasks_.pop_front();
        assert(tasks_.size() == size - 1);
    }
    return task;
}

}  // namespace thread_pool