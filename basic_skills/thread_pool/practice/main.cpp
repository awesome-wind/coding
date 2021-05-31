#include <iostream>
#include <thread>

#include "thread_pool.h"

std::mutex g_mutex;

void func() {
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    for (int i = 1; i < 4; i++) {
        std::unique_lock<std::mutex> lock(g_mutex);
        std::cout << "thread:" << std::this_thread::get_id() << " execute func[" << i << "]." << std::endl;
    }
};

int main() {
    ThreadPool pool(3);

    for (int i = 0; i < 10; i++) {
        pool.add_task(func);
    }

    getchar();
    return 0;
}