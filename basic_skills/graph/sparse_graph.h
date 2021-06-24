#pragma once

#include <iostream>
#include <vector>

class SparseGraph {
public:
    SparseGraph(int n, bool is_directed) : n_(n), m_(0), directed_(is_directed) {
        for (int i = 0; i < n_; ++i) {
            graph_.push_back(std::vector<int>());
        }
    }
    ~SparseGraph() {
    }

    int vertices() {
        return n_;
    }
    int edges() {
        return m_;
    }

    void add_edge(int v, int w) {
        assert(0 <= v && v < n_);
        assert(0 <= w && w < n_);

        graph_[v].push_back(w);
        if (!directed_) {
            graph_[w].push_back(v);
        }
        ++m_;
    }

    bool has_edge(int v, int w) {
        assert(0 <= v && v < n_);
        assert(0 <= w && w < n_);

        for (int i = 0; i < graph_[v].size(); ++i) {
            if (graph_[v][i] == w) {
                return true;
            }
        }
        return false;
    }

    void show() {
        for (int i = 0; i < n_; ++i) {
            std::cout << "vectex: " << i << " adjust:";
            for (int p = 0; p < graph_[i].size(); ++p) {
                std::cout << " " << graph_[i][p];
            }
            std::cout << std::endl;
        }
    }

    class GraphIterator {
    public:
        GraphIterator(SparseGraph& g, int v) : sparse_graph_(g), v_(v), index_(0) {
        }

        int begin() {
            index_ = 0;
            if (sparse_graph_.graph_[v_].size())
                return sparse_graph_.graph_[v_][index_];
            return -1;
        }

        int next() {
            ++index_;
            if (index_ < sparse_graph_.graph_[v_].size()) {
                return sparse_graph_.graph_[v_][index_];
            }
            return -1;
        }

        bool end() {
            return index_ >= sparse_graph_.graph_[v_].size();
        }

    private:
        SparseGraph& sparse_graph_;
        int v_;
        int index_;
    };

private:
    int n_, m_;
    bool directed_ = false;
    std::vector<std::vector<int>> graph_;
};