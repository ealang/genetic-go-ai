#include "gtest/gtest.h"
#include "board.h"
#include "gptree.h"
#include "gpnodes_experimental.h"

class GPNodesExperimentalTest: public ::testing::Test {
public:
    Board board;
    GPNodesExperimentalTest(): board(Board(5)) {}
};

TEST_F(GPNodesExperimentalTest, CanAddNumbers) {
    auto n1 = ConstNode(1), n2 = ConstNode(2);
    ASSERT_EQ(3, PlusNode(&n1, &n2).get(board));
}

TEST_F(GPNodesExperimentalTest, CanGenerateRandomNumbers) {
    auto node = RandomNode(0, 10);
    for (int i = 0; i < 10000; i++) {
        int n = node.get(board);
        ASSERT_TRUE(n >= 0 && n <= 10);
    }
}

TEST_F(GPNodesExperimentalTest, RandomTreeHasCorrectDepth) {
    GPNode* n = createRandomTree(3);
    n->get(board);
    ASSERT_EQ(3, treeDepth(n));
    cleanupTree(n);
}

TEST_F(GPNodesExperimentalTest, CanStringPrintRandomTree) {
    for (int i = 0; i < 10; i++) {
        GPNode* n = createRandomTree(3);
        n->toString();
        cleanupTree(n);
    }
}

TEST_F(GPNodesExperimentalTest, CalculatesNumberOfConnectedStones) {
    auto black = ChainLengthNode(BLACK),
         white = ChainLengthNode(WHITE);
    ASSERT_EQ(0, black.get(board));
    ASSERT_EQ(0, white.get(board));
    board.set(0, 0, BLACK);
    ASSERT_EQ(0, black.get(board));
    ASSERT_EQ(0, white.get(board));
    board.set(1, 0, BLACK);
    ASSERT_EQ(2, black.get(board));
    ASSERT_EQ(0, white.get(board));
    board.set(1, 1, BLACK);
    ASSERT_EQ(3, black.get(board));
    ASSERT_EQ(0, white.get(board));

    board.set(2, 2, WHITE);
    board.set(3, 2, WHITE);
    board.set(4, 2, WHITE);
    board.set(4, 3, WHITE);
    ASSERT_EQ(3, black.get(board));
    ASSERT_EQ(4, white.get(board));
}

TEST_F(GPNodesExperimentalTest, CalulatesNumberOfConnectedStonesAndDoesntWrap) {
    auto black = ChainLengthNode(BLACK);
    board.set(0, 1, BLACK);
    board.set(4, 0, BLACK);
    ASSERT_EQ(0, black.get(board));
}

TEST_F(GPNodesExperimentalTest, CalculatesNumberOfLiberties) {
    board.set(0, 0, BLACK);
    board.set(0, 1, BLACK);
    board.set(1, 0, BLACK);
    board.set(1, 1, WHITE);
    board.set(2, 2, WHITE);
    ASSERT_EQ(2, LibertiesNode(BLACK).get(board));
    ASSERT_EQ(4, LibertiesNode(WHITE).get(board));
}

TEST_F(GPNodesExperimentalTest, CalculatesNumberOfClustersOfStones) {
    board.set(0, 0, BLACK);
    board.set(0, 1, BLACK);
    board.set(1, 0, BLACK);
    board.set(2, 1, BLACK);
    board.set(1, 1, WHITE);
    board.set(2, 2, WHITE);
    board.set(4, 4, WHITE);
    EXPECT_EQ(2, ClustersNode(BLACK, false).get(board));
    EXPECT_EQ(3, ClustersNode(WHITE, false).get(board));
    EXPECT_EQ(1, ClustersNode(BLACK, true).get(board));
    EXPECT_EQ(2, ClustersNode(WHITE, true).get(board));
}

TEST_F(GPNodesExperimentalTest, CalculatesLessThan) {
    ConstNode c5(5);
    ConstNode c10(10);
    ConstNode c1(1);
    ConstNode c2(2);
    EXPECT_EQ(5, IfLessThanNode(&c1, &c2, &c5, &c10).get(board));
}
