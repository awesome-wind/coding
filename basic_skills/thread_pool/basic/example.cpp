#include <chrono>
#include <condition_variable>
#include <cstdio>
#include <iostream>
#include <mutex>
#include <thread>

#include "thread_pool.h"

std::mutex g_mutex;

void test_func() {
    for (int i = 1; i < 4; i++) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::lock_guard<std::mutex> lock(g_mutex);
        std::cout << "test_func() [" << i << "] at thread [" << std::this_thread::get_id() << "] executed."
                  << std::endl;
    }
}

int main() {
    thread_pool::ThreadPool thread_pool;

    for (int i = 0; i < 10; i++) {
        thread_pool.add_task(test_func);
    }

    // getchar();
    std::this_thread::sleep_for(std::chrono::seconds(20));
    return 0;
}