#include "graph.h"

#include <cstdlib>
#include <iostream>

#include "dense_graph.h"
#include "path.h"
#include "read_graph.h"
#include "sparse_graph.h"

int graph() {
    // int n = 20;
    // int m = 100;
    // bool direct = false;

    // std::srand(time(NULL));

    // DenseGraph dense_graph(n, direct);
    // for (int i = 0; i < m; i++) {
    //     int v = rand() % n;
    //     int w = rand() % n;
    //     dense_graph.add_edge(v, w);
    // }

    // std::cout << "dense graph" << std::endl;
    // for (int i = 0; i < n; ++i) {
    //     DenseGraph::GraphIterator iter(dense_graph, i);
    //     std::cout << "vertex :" << i << " adjust:";
    //     for (int w = iter.begin(); !iter.end(); w = iter.next()) {
    //         std::cout << " " << w;
    //     }
    //     std::cout << std::endl;
    // }

    // SparseGraph sparse_graph(n, direct);
    // for (int i = 0; i < m; ++i) {
    //     int v = rand() % n;
    //     int w = rand() % n;
    //     sparse_graph.add_edge(v, w);
    // }

    // std::cout << "sparse graph:" << std::endl;
    // for (int i = 0; i < n; ++i) {
    //     SparseGraph::GraphIterator iter(sparse_graph, i);
    //     std::cout << "vertex :" << i << " adjust:";
    //     for (int w = iter.begin(); !iter.end(); w = iter.next()) {
    //         std::cout << " " << w;
    //     }
    //     std::cout << std::endl;
    // }

    DenseGraph dense_graph(11, false);
    std::string filename = "../graph/test.txt";
    ReadGraph<DenseGraph> graph1(dense_graph, filename);
    dense_graph.show();

    SparseGraph sparse_graph(11, false);
    ReadGraph<SparseGraph> graph2(sparse_graph, filename);
    sparse_graph.show();

    Path<DenseGraph> path(dense_graph, 3);
    path.show_path(10);

    return 0;
}