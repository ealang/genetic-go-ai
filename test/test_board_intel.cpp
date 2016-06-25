#include "gtest/gtest.h"
#include "board.h"
#include "board_intel.h"

using namespace std;

TEST(StoneIntelTest, CanDetectSuicideMovesForSingleStone) {
    Board b(2);
    b.set(0, 1, BLACK);
    b.set(1, 0, BLACK);
    ASSERT_TRUE(isSuicideMove(b, 0, 0, WHITE));
    ASSERT_FALSE(isSuicideMove(b, 0, 0, BLACK));
}

TEST(StoneIntelTest, CanDetectSuicideMovesForMultipleStones) {
    Board b(3);
    b.set(0, 1, BLACK);
    b.set(1, 1, BLACK);
    b.set(2, 0, BLACK);
    b.set(0, 0, WHITE);
    ASSERT_TRUE(isSuicideMove(b, 1, 0, WHITE));
    ASSERT_FALSE(isSuicideMove(b, 1, 0, BLACK));
}

TEST(StoneIntelTest, CanDetectSuicideMovesWhenWillCaptureOpponent) {
    Board b(3);
    b.set(0, 1, WHITE);
    b.set(1, 0, WHITE);
    b.set(1, 1, BLACK);
    b.set(0, 2, BLACK);
    ASSERT_FALSE(isSuicideMove(b, 0, 0, BLACK));
    ASSERT_FALSE(isSuicideMove(b, 0, 0, WHITE));
}

TEST(StoneIntelTest, CalculatesNetworkStrength) {
    Board b(3);
    b.set(0, 0, BLACK);
    b.set(1, 0, BLACK);
    b.set(0, 1, WHITE);
    b.set(2, 2, BLACK);
    ASSERT_EQ(2, stoneNetworkStrength(b, BLACK));
    ASSERT_EQ(0, stoneNetworkStrength(b, WHITE));
}

TEST(StoneIntelTest, DetectsIfCanBeCaptured) {
    Board b(8);
    b.set(0, 0, BLACK);
    b.set(1, 0, BLACK);

    b.set(0, 1, WHITE);
    b.set(1, 1, WHITE);

    EXPECT_TRUE(canBeCaptured(b, BLACK));
    EXPECT_FALSE(canBeCaptured(b, WHITE));
}

TEST(StoneIntelTest, CalculatesNumberOfLibertiesTotal) {
    Board b(4);
    b.set(0, 0, BLACK);
    b.set(1, 0, BLACK);
    b.set(0, 1, WHITE);
    b.set(3, 3, WHITE);
    ASSERT_EQ(2, numLiberties(b, BLACK));
    ASSERT_EQ(4, numLiberties(b, WHITE));
}

TEST(StoneIntelTest, CalculatesNumberOfLibertiesInCluster) {
    Board b(4);
    b.set(0, 0, BLACK);
    b.set(1, 0, BLACK);
    b.set(0, 1, WHITE);
    b.set(3, 3, WHITE);
    ASSERT_EQ(2, numLiberties(b, 0, 0));
    ASSERT_EQ(2, numLiberties(b, 1, 0));
    ASSERT_EQ(2, numLiberties(b, 0, 1));
    ASSERT_EQ(2, numLiberties(b, 3, 3));
    ASSERT_THROW(numLiberties(b, 2, 2), std::runtime_error);
}

TEST(StoneIntelTest, CalculatesMaxClusterSize) {
    Board b(8);
    b.set(0, 0, BLACK);
    b.set(2, 0, BLACK);
    b.set(2, 1, BLACK);
    b.set(4, 0, BLACK);
    b.set(4, 1, BLACK);
    b.set(5, 0, BLACK);

    b.set(7, 7, WHITE);
    ASSERT_EQ(3, maxClusterSize(b, BLACK));
    ASSERT_EQ(1, maxClusterSize(b, WHITE));
}

TEST(StoneIntelTest, CanCountNumberOfAdjacentStones) {
    Board b(7);
    b.set(0, 0, BLACK);
    b.set(1, 0, BLACK);
    b.set(2, 0, BLACK);
    b.set(3, 1, BLACK);
    b.set(0, 1, WHITE);

    bool inclDiag = true;
    EXPECT_EQ(1, numAdjacentStones(b, !inclDiag, 0, 0, BLACK));
    EXPECT_EQ(2, numAdjacentStones(b, !inclDiag, 1, 0, BLACK));
    EXPECT_EQ(1, numAdjacentStones(b, !inclDiag, 2, 0, BLACK));
    EXPECT_EQ(0, numAdjacentStones(b, !inclDiag, 3, 1, BLACK));
    EXPECT_EQ(1, numAdjacentStones(b, inclDiag, 3, 1, BLACK));
    EXPECT_EQ(0, numAdjacentStones(b, inclDiag, 0, 1, WHITE));
    EXPECT_EQ(2, numAdjacentStones(b, inclDiag, 0, 1, BLACK));
}

TEST(StoneIntelTest, CanCalcStoneSpacing) {
    Board b(5);
    ASSERT_EQ(10, maxStoneSpacing(b, BLACK));

    b.set(0, 0, BLACK);
    ASSERT_EQ(8, maxStoneSpacing(b, BLACK));

    b.set(4, 0, BLACK);
    ASSERT_EQ(6, maxStoneSpacing(b, BLACK));

    b.set(0, 4, BLACK);
    ASSERT_EQ(4, maxStoneSpacing(b, BLACK));

    b.set(2, 2, BLACK);
    ASSERT_EQ(4, maxStoneSpacing(b, BLACK));
}
