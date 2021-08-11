#pragma once

#include <cassert>
#include <iostream>
#include <vector>

class DenseGraph {
public:
    DenseGraph(int n, int direct) {
        this->n_ = n;
        this->m_ = 0;
        this->directed_ = direct;

        for (int i = 0; i < n; ++i) {
            graph_.push_back(std::vector<bool>(n, false));
        }
    }
    ~DenseGraph() {
    }

    int vertices() {
        return n_;
    }

    int edges() {
        return m_;
    }

    void add_edge(int v, int w) {
        assert(v >= 0 && v < n_);
        assert(w >= 0 && w < n_);

        if (has_edge(v, w)) {
            return;
        }

        graph_[v][w] = true;
        if (!directed_) {
            graph_[w][v] = true;
        }
        m_++;
    }

    bool has_edge(int v, int w) {
        assert(0 <= v && v < n_);
        assert(0 <= w && w < n_);

        return graph_[v][w];
    }

    void show() {
        for (int i = 0; i < n_; ++i) {
            std::cout << "vertex: " << i << " adjust:";
            for (int j = 0; j < n_; ++j) {
                if (has_edge(i, j)) {
                    std::cout << " " << j;
                }
            }
            std::cout << std::endl;
        }
    }

    class GraphIterator {
    public:
        GraphIterator(DenseGraph& g, int v) : dense_graph_(g), v_(v), index_(0) {
        }

        int begin() {
            index_ = -1;
            return next();
        }
        int next() {
            int v = -1;
            for (index_ += 1; index_ < dense_graph_.vertices(); ++index_) {
                if (dense_graph_.graph_[v_][index_])
                    return index_;
            }
            return -1;
        }

        bool end() {
            return index_ >= dense_graph_.vertices();
        }

    private:
        DenseGraph& dense_graph_;
        int v_;
        int index_;
    };

private:
    int n_, m_;
    bool directed_ = false;
    std::vector<std::vector<bool>> graph_;
};