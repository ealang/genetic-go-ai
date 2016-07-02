#include "gtest/gtest.h"
#include "board.h"
#include "gptree.h"
#include "gpnode_context.h"
#include "gpnodes_experimental.h"

class GPNodesExperimentalTest: public ::testing::Test {
public:
    Board prevBoard, board;
    Context blackContext, whiteContext;
    GPNodesExperimentalTest():
        prevBoard(5),
        board(5),
        blackContext{0, 0, BLACK, board, board},
        whiteContext{0, 0, WHITE, board, board} {
    }
    Context set(int x, int y, Color c) {
        prevBoard = board;
        board.set(x, y, c);
        return Context{x, y, c, prevBoard, board};
    }
};

TEST_F(GPNodesExperimentalTest, CanAddNumbers) {
    auto n1 = IntConstNode(1), n2 = IntConstNode(2);
    ASSERT_EQ(3, PlusNode(&n1, &n2).get(blackContext));
}

TEST_F(GPNodesExperimentalTest, CanGenerateRandomNumbers) {
    auto node = RandomIntNode(0, 10);
    for (int i = 0; i < 10000; i++) {
        int n = node.get(blackContext);
        ASSERT_TRUE(n >= 0 && n <= 10);
    }
}

TEST_F(GPNodesExperimentalTest, RandomTreeHasCorrectDepth) {
    GPNode* n = createRandomTree(3);
    n->get(blackContext);
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
    auto chain = NetworkStrengthDeltaNode();

    ASSERT_EQ(0, chain.get(set(0, 0, BLACK)));
    ASSERT_EQ(2, chain.get(set(1, 0, BLACK)));
    ASSERT_EQ(1, chain.get(set(2, 0, BLACK)));

    set(0, 2, WHITE);
    set(2, 2, WHITE);
    ASSERT_EQ(3, chain.get(set(1, 2, WHITE)));
}

TEST_F(GPNodesExperimentalTest, CalulatesNumberOfConnectedStonesAndDoesntWrap) {
    auto chain = NetworkStrengthDeltaNode();
    board.set(0, 1, BLACK);
    ASSERT_EQ(0, chain.get(set(4, 0, BLACK)));
}

TEST_F(GPNodesExperimentalTest, CalculatesNumberOfLiberties) {
    bool mine = true;
    board.set(0, 0, BLACK);
    board.set(0, 1, BLACK);
    {
        Context context = set(0, 2, BLACK);
        ASSERT_EQ(1, LibertiesDeltaNode(mine).get(context));
        ASSERT_EQ(0, LibertiesDeltaNode(!mine).get(context));
    }
    board.set(1, 0, WHITE);
    {
        Context context = set(1, 1, WHITE);
        ASSERT_EQ(1, LibertiesDeltaNode(mine).get(context));
        ASSERT_EQ(-1, LibertiesDeltaNode(!mine).get(context));
    }
}

TEST_F(GPNodesExperimentalTest, CanDetectChangesInClusterSize) {
    EXPECT_EQ(1, MaxClusterDeltaNode().get(set(0, 0, BLACK)));
    EXPECT_EQ(1, MaxClusterDeltaNode().get(set(1, 0, BLACK)));
    EXPECT_EQ(0, MaxClusterDeltaNode().get(set(3, 0, BLACK)));
    EXPECT_EQ(2, MaxClusterDeltaNode().get(set(2, 0, BLACK)));

    EXPECT_EQ(1, MaxClusterDeltaNode().get(set(1, 1, WHITE)));
    EXPECT_EQ(0, MaxClusterDeltaNode().get(set(3, 3, WHITE)));
    EXPECT_EQ(1, MaxClusterDeltaNode().get(set(4, 3, WHITE)));
    EXPECT_EQ(0, MaxClusterDeltaNode().get(set(1, 3, WHITE)));
    EXPECT_EQ(0, MaxClusterDeltaNode().get(set(3, 1, WHITE)));
    EXPECT_EQ(4, MaxClusterDeltaNode().get(set(2, 2, WHITE)));
}

TEST_F(GPNodesExperimentalTest, CalculatesNearbyStones) {
    board.set(0, 1, BLACK);
    board.set(0, 2, BLACK);
    board.set(1, 0, BLACK);
    board.set(1, 1, WHITE);
    board.set(2, 2, WHITE);

    bool incladj = true;
    {
        bool black = true;
        Context bcontext{0, 1, BLACK, board, board};
        EXPECT_EQ(1, AdjacentStonesNode(black, !incladj).get(bcontext));
        EXPECT_EQ(2, AdjacentStonesNode(black, incladj).get(bcontext));
        EXPECT_EQ(1, AdjacentStonesNode(!black, !incladj).get(bcontext));
    }

    {
        bool white = true;
        Context wcontext{1, 1, WHITE, board, board};
        EXPECT_EQ(0, AdjacentStonesNode(white, !incladj).get(wcontext));
        EXPECT_EQ(1, AdjacentStonesNode(white, incladj).get(wcontext));
        EXPECT_EQ(3, AdjacentStonesNode(!white, incladj).get(wcontext));
    }
}

TEST_F(GPNodesExperimentalTest, DetectsIfCanBeCaptured) {
    board.set(0, 1, BLACK);

    {
        Context c = set(0, 0, WHITE);
        EXPECT_TRUE(CanBeCapturedNode().get(c));
    }
    {
        Context c = set(1, 0, WHITE);
        EXPECT_FALSE(CanBeCapturedNode().get(c));
    }
}

TEST_F(GPNodesExperimentalTest, DetectsIfCanCapture) {
    board.set(0, 0, WHITE);
    {
        Context c = set(0, 1, BLACK);
        EXPECT_TRUE(CanCaptureNode().get(c));
    }
}

TEST_F(GPNodesExperimentalTest, DetectScoreChanges) {
    board.set(0, 1, BLACK);
    board.set(0, 0, WHITE);
    board.set(4, 4, WHITE);
    EXPECT_EQ(1, PlayerScoreDeltaNode().get(set(1, 0, BLACK)));
}
