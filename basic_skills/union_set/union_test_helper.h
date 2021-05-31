#pragma once

#include <sys/_types/_time_t.h>

#include <cstddef>
#include <cstdlib>
#include <ctime>
#include <iostream>

#include "union_find.h"

namespace helper {
class UnionTestHelper {
public:
    static void test_union(int n) {
        srand(time(NULL));
        un::UnionFind uf = un::UnionFind(n);
        unsigned long start_time = clock();

        for (int i = 0; i < n; i++) {
            uf.join_union(rand() % n, rand() % n);
        }
        for (int i = 0; i < n; i++) {
            uf.is_connected(rand() % n, rand() % n);
        }
        unsigned long end_time = clock();
        std::cout << "uf," << 2 * n << " ops, " << double(end_time - start_time) / CLOCKS_PER_SEC << std::endl;
    }
};
}  // namespace helper
