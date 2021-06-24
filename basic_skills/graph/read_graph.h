#pragma once

#include <cstdio>
#include <fstream>
#include <sstream>
#include <string>

template <typename Graph>
class ReadGraph {
public:
    ReadGraph(Graph& graph, std::string& filename) {
        std::ifstream file(filename);
        std::string line;
        int V, E;

        assert(file.is_open());
        assert(getline(file, line));
        std::stringstream ss(line);
        ss >> V >> E;

        assert(V == graph.vertices());

        int a, b;
        for (int i = 0; i < E; ++i) {
            assert(getline(file, line));
            std::stringstream ss(line);
            ss >> a >> b;
            graph.add_edge(a, b);
        }
    }
};