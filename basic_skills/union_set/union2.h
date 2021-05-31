#pragma once

#include <cassert>
namespace un {

class Union2 {
public:
    Union2(int count) : count_(count) {
        parent_ = new int(count_);
        for (int i = 0; i < count_; ++i) {
            parent_[i] = i;
        }
    }
    ~Union2() {
        delete[] parent_;
    }

    int find(int p) {
        assert(p >= 0 && p < count_);
        while (p != parent_[p]) {
            p = parent_[p];
        }
        return p;
    }

    bool is_connected(int p, int q) {
        return find(p) == find(q);
    }

    void join_union(int p, int q) {
        int p_root = find(p);
        int q_root = find(q);

        if (p_root == q_root)
            return;
        else
            parent_[p_root] = q_root;
    }

private:
    int* parent_;
    int count_;
};

}  // namespace un