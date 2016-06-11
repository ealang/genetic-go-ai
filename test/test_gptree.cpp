#include <utility>
#include "gtest/gtest.h"
#include "gpnodes_experimental.h"
#include "gptree.h"
#include "board.h"

using namespace std;

class GPTreeTest: public ::testing::Test {
public:
    Board board;
    GPTreeTest(): board(Board(9)) {}
};

TEST_F(GPTreeTest, MeasuresCorrectDepthOfTerminalNode) {
    ConstNode n = ConstNode(1);
    ASSERT_EQ(1, treeDepth(&n));
}

TEST_F(GPTreeTest, MeasuresCorrectCountOfTerminalNode) {
    auto n = ConstNode(1);
    ASSERT_EQ(1, treeNodeCount(&n));
}

TEST_F(GPTreeTest, MeasuresCorrectDepthOfTree) {
    auto c1 = ConstNode(1);
    auto n2 = PlusNode(&c1, &c1);
    auto n1 = PlusNode(&n2, &c1);
    ASSERT_EQ(3, treeDepth(&n1));
}

TEST_F(GPTreeTest, MeasuresCorrectNodeCountInTree) {
    auto c1 = ConstNode(1);
    auto n2 = PlusNode(&c1, &c1);
    auto n1 = PlusNode(&n2, &c1);
    ASSERT_EQ(5, treeNodeCount(&n1));
}

TEST_F(GPTreeTest, CanFindNodeByNumber) {
    auto c2 = ConstNode(2), c3 = ConstNode(3), c4 = ConstNode(4);
    auto n1 = PlusNode(&c2, &c3);
    auto n0 = PlusNode(&n1, &c4);
    ASSERT_EQ(&n0, treeGetNodeNum(&n0, 0));
    ASSERT_EQ(&n1, treeGetNodeNum(&n0, 1));
    ASSERT_EQ(&c2, treeGetNodeNum(&n0, 2));
    ASSERT_EQ(&c3, treeGetNodeNum(&n0, 3));
    ASSERT_EQ(&c4, treeGetNodeNum(&n0, 4));
    ASSERT_EQ(nullptr, treeGetNodeNum(&n0, 5));
}

TEST_F(GPTreeTest, CanSubstituteTrees) {
    auto c2 = ConstNode(2), c3 = ConstNode(3), c4 = ConstNode(4);
    auto n1 = PlusNode(&c2, &c3);
    auto n0 = PlusNode(&n1, &c4);
    auto sub = ConstNode(10);
    int expectedSum[] = {10, 14, 17, 16, 15};
    for (int i = 0; i < 5; i++) {
        GPNode* newtree = treeReplaceAt(&n0, i, &sub);
        ASSERT_EQ(expectedSum[i], newtree->get(board)) << "replace at " << i;
        cleanupTree(newtree);
    }
}

TEST_F(GPTreeTest, CanRandomlySwapSubtreesWithNoNodeLoss) {
    ConstNode c1 = ConstNode(1),
              c2 = ConstNode(2),
              c3 = ConstNode(4),
              c4 = ConstNode(8),
              c5 = ConstNode(16),
              c6 = ConstNode(32);
    PlusNode n1 = PlusNode(&c1, &c2);
    PlusNode n2 = PlusNode(&n1, &c3);
    PlusNode n3 = PlusNode(&c4, &c5);
    PlusNode n4 = PlusNode(&n3, &c6);

    ASSERT_EQ(1 + 2 + 4, n2.get(board));
    ASSERT_EQ(32 + 16 + 8, n4.get(board));

    auto trees = swapRandomSubtrees(&n2, &n4);

    ASSERT_NE(trees.first->get(board), n2.get(board));
    ASSERT_NE(trees.second->get(board), n4.get(board));
    ASSERT_EQ(63, trees.first->get(board) + trees.second->get(board));

    cleanupTree(trees.first);
    cleanupTree(trees.second);
}
