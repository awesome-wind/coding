#include "binary_tree/binary_tree.h"
#include "graph/graph.h"
#include "gtest/gtest.h"

TEST(TreeTest, binaryTree) {
    EXPECT_EQ(run_binary_tree(), 0);
}

TEST(GraphTest, graph) {
    EXPECT_EQ(graph(), 0);
}