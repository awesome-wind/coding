#include <iostream>
#include <random>
#include <thread>

#include "thread_pool.h"

std::random_device rd;
std::mt19937 mt(rd);
std::uniform_int_distribution<int> dist(-1000, 1000);
auto rnd = std::bind(dist, mt);

void simulate_hard_computation() {
    std::this_thread::sleep_for(std::chrono::milliseconds(2000 + rnd()));
}

void multify(const int a, const int b) {
    simulate_hard_computation();
    const int res = a * b;
    std::cout << a << "*" << b << "=" << res << std::endl;
}

void multify_output(int& out, const int a, const int b) {
    simulate_hard_computation();
    out = a * b;
    std::cout << a << "*" << b << "=" << out << std::endl;
}

int multify_return(const int a, const int b) {
    simulate_hard_computation();
    const int res = a * b;
    std::cout << a << "*" << b << "=" << res << std::endl;
    return res;
}

int main() {
    ThreadPool pool(3);
    pool.init();

    for (int i = 1; i < 3; ++i) {
        for (int j = 1; j < 10; ++j) {
            pool.submit(multify, i, j);
        }
    }

    int output_ref;
    auto future1 = pool.submit(multify_output, std::ref(output_ref), 5, 6);
    future1.get();
    std::cout << "multify output is equal to:" << output_ref << std::endl;

    auto future2 = pool.submit(multify_return, 5, 3);
    int res = future2.get();
    std::cout << "multify result is equal to:" << res << std::endl;
}