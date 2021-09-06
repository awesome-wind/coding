#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#define MAXN 5005

int fa[MAXN], rank[MAXN];

inline void init(int n) {
    for (int i = 1; i <= n; ++i) {
        fa[i] = i;
        rank[i] = 1;
    }
}

int find(int x) {
    return x == fa[x] ? x : (fa[x] = find(fa[x]));
}

inline void merge(int i, int j) {
    int x = find(i), y = find(j);
    if (rank[x] <= rank[y]) {
        fa[x] = y;
    } else {
        fa[y] = x;
    }
    if (rank[x] == rank[y] && x != y) {
        rank[y]++;
    }
}

int main() {
    int n, m, p, x, y;
    std::ifstream in("releate.txt", std::ios::in);
    std::string line;
    std::getline(in, line);
    std::stringstream ss(line);
    ss >> n >> m >> p;
    std::cout << "n=" << n << " m=" << m << " p=" << p << std::endl;

    init(n);

    for (int i = 0; i < m; i++) {
        std::getline(in, line);
        std::stringstream ss(line);
        ss >> x >> y;
        std::cout << "x=" << x << " y=" << y << std::endl;
        merge(x, y);
    }

    for (int i = 0; i < p; i++) {
        std::getline(in, line);
        std::stringstream ss(line);
        ss >> x >> y;
        std::cout << "x=" << x << " y=" << y << " are releated:" << (find(x) == find(y)) << std::endl;
    }
    return 0;
}