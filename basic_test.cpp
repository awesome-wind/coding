#include <cmath>
#include <thread>

#include "basic/base/base.h"
#include "binary_tree/binary_tree.h"
#include "graph/graph.h"
#include "gtest/gtest.h"
#include "leetcode/leetcode.h"
#include "thread_pool/basic/thread_pool.h"

std::mutex g_mutex;

void test_func() {
    for (int i = 1; i < 4; i++) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        std::lock_guard<std::mutex> lock(g_mutex);
        std::cout << "test_func() [" << i << "] at thread [" << std::this_thread::get_id() << "] executed."
                  << std::endl;
    }
}

TEST(TreeTest, binaryTree) {
    EXPECT_EQ(run_binary_tree(), 0);
}

TEST(GraphTest, graph) {
    EXPECT_EQ(graph(), 0);
}

TEST(ThreadPoolTest, thread_pool) {
    thread_pool::ThreadPool thread_pool;

    for (int i = 0; i < 5; i++) {
        thread_pool.add_task(test_func);
    }

    // getchar();
    std::this_thread::sleep_for(std::chrono::seconds(3));
}

TEST(BaseTest, async_access) {
    std::unique_ptr<Base> base = std::make_unique<Base>();

    int k = 6;
    EXPECT_EQ(base->promise_task(k), std::pow(2, k));
    EXPECT_EQ(base->packaged_task(k), std::pow(2, k));
    EXPECT_EQ(base->async_task(k), std::pow(2, k));
}
