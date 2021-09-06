#include <cmath>
#include <future>
#include <iostream>

#include "base/base.h"

void thread_set(std::promise<int>& pr, int k) {
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    std::cout << "input key:" << k << " res:" << std::pow(2, k) << std::endl;
    pr.set_value(std::pow(2, k));
}

int Base::promise_task(int k) {
    std::promise<int> pr;
    std::future<int> fu = pr.get_future();

    std::thread t1(thread_set, std::ref(pr), k);
    t1.join();  //join will block

    return fu.get();  //get() will block
}

int Base::packaged_task(int k) {
    std::packaged_task<int(int)> task([](int k) { return std::pow(2, k); });
    std::future<int> fu = task.get_future();

    std::thread task_td(std::move(task), k);
    task_td.join();

    return fu.get();
}

int Base::async_task(int k) {
    auto res = std::async(std::launch::async, [k]() { return std::pow(2, k); });
    return res.get();
}