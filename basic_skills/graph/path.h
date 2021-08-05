#pragma once

#include <cassert>
#include <iostream>
#include <stack>
#include <vector>

template <typename Graph>
class Path {
public:
    Path(Graph& graph, int v) : g_(graph), v_(v) {
        visited_ = new bool[g_.vertices()];
        from_ = new int[g_.vertices()];

        for (int i = 0; i < g_.vertices(); ++i) {
            visited_[i] = false;
            from_[i] = -1;
        }

        for (int i = 0; i < g_.vertices(); ++i) {
            if (!visited_[i]) {
                dfs(i);
            }
        }
    }

    ~Path() {
        delete[] visited_;
        delete[] from_;
    }

    bool has_path(int p) {
        assert(0 <= p && p < g_.vertices());
        return visited_[p];
    }

    void show_path(int p) {
        std::stack<int> vec;
        while (p != -1) {
            vec.push(p);
            std::cout << " push" << p;
            p = from_[p];
        }
        std::cout << std::endl;
        if (vec.size() == 0 || vec.top() != v_) {
            std::cout << "there is no way form: " << v_ << " to: " << p << std::endl;
        } else {
            for (int i = 0; i < vec.size() - 1; ++i) {
                std::cout << vec.top() << " - > ";
                vec.pop();
            }
            std::cout << vec.top() << std::endl;
            vec.pop();
        }
    }

private:
    void dfs(int p) {
        visited_[p] = true;
        typename Graph::GraphIterator iter(g_, p);
        std::cout << "current:" << p << " adjust:";
        for (int w = iter.begin(); !iter.end(); w = iter.next()) {
            std::cout << " " << w;
        }
        std::cout << std::endl;

        for (int w = iter.begin(); !iter.end(); w = iter.next()) {
            if (!visited_[w]) {
                from_[w] = p;
                dfs(w);
            }
        }
    }

    Graph& g_;
    bool* visited_;
    int* from_;
    int v_;
};