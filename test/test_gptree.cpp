#include "gtest/gtest.h"
#include "gpnodes_experimental.h"
#include "gptree.h"
#include "board.h"

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
    ASSERT_EQ(9, treeGetNodeNum(&n0, 0)->get(board));
    ASSERT_EQ(5, treeGetNodeNum(&n0, 1)->get(board));
    ASSERT_EQ(2, treeGetNodeNum(&n0, 2)->get(board));
    ASSERT_EQ(3, treeGetNodeNum(&n0, 3)->get(board));
    ASSERT_EQ(4, treeGetNodeNum(&n0, 4)->get(board));
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

TEST_F(GPTreeTest, CanRandomlyCombineTrees) {
    GPNode* tree1 = createRandomTree(4);
    GPNode* tree2 = createRandomTree(4);
    GPNode* tree3 = mutateCombineTrees(tree1, tree2);
    tree3->get(board);

    ASSERT_LT(treeDepth(tree3), 9);

    cleanupTree(tree1);
    cleanupTree(tree2);
    cleanupTree(tree3);
}
