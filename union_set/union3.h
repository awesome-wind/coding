#pragma once

#include <assert.h>

namespace un {
class Union3 {
public:
    Union3(int count) : count_(count) {
        parent_ = new int[count_];
        s_nums_ = new int[count_];
        for (int i = 0; i < count_; ++i) {
            parent_[i] = i;
            s_nums_[i] = 1;
        }
    }
    ~Union3() {
        delete[] parent_;
        delete[] s_nums_;
    }

    int find(int p) {
        assert(p >= 0 && p < count_);
        while (p != parent_[p])
            p = parent_[p];
        return p;
    }

    bool is_connected(int p, int q) {
        return find(p) == find(q);
    }

    void join_union(int p, int q) {
        int p_root = find(p);
        int q_root = find(q);

        if (p_root == q_root) {
            return;
        }

        if (s_nums_[p_root] < s_nums_[q_root]) {
            parent_[p_root] = q_root;
            s_nums_[q_root] += s_nums_[p_root];
        } else {
            parent_[q_root] = p_root;
            s_nums_[p_root] += s_nums_[q_root];
        }
    }

private:
    int* parent_;
    //某个节点的父节点所拥有的nodes数量
    int* s_nums_;
    int count_;
};
}  // namespace un