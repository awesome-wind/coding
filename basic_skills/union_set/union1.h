#pragma once

#include <cassert>

namespace un {
class Union1 {
public:
    Union1(int n) : count_(n) {
        id_ = new int[n];
        for (int i = 0; i < n; i++) {
            id_[i] = i;
        }
    }
    ~Union1() {
        delete[] id_;
    }

    int find(int x) {
        assert(x >= 0 && x < count_);
        return id_[x];
    }

    bool is_connected(int x, int y) {
        return id_[x] == id_[y];
    }

    void join_union(int x, int y) {
        int xid = id_[x];
        int yid = id_[y];

        if (xid == yid) {
            return;
        } else {
            for (int i = 0; i < count_; i++) {
                if (id_[i] == yid) {
                    id_[i] = xid;
                }
            }
        }
    }

private:
    int* id_;
    int count_;
};
}  // namespace un