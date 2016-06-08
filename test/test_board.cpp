#include <stdexcept>
#include "gtest/gtest.h"
#include "board.h"
#include "color.h"

TEST(BoardTest, CanCreateBoard) {
    Board b(9);
    ASSERT_EQ(9, b.size);
}

TEST(BoardTest, CanGenerateAStringRepresentation) {
    Board b(2);
    b.set(0, 0, BLACK);
    b.set(1, 1, WHITE);
    ASSERT_EQ("|B| |\n-----\n| |W|\n-----\n", b.toString());
}

TEST(BoardTest, CanModifyAndReadBoardState) {
    Board b(9);
    ASSERT_TRUE(b.empty(0, 0));
    ASSERT_EQ(NONE, b.get(0, 0));
    b.set(0, 0, BLACK);
    b.set(0, 1, WHITE);
    ASSERT_EQ(BLACK, b.get(0, 0));
    ASSERT_EQ(WHITE, b.get(0, 1));
    ASSERT_FALSE(b.empty(0, 0));
    ASSERT_FALSE(b.empty(0, 1));
}

TEST(BoardTest, CalculatesMultiCellPlayerTerritory) {
    Board b(4);
    ASSERT_EQ(0, b.territoryCount(BLACK));
    ASSERT_EQ(0, b.territoryCount(WHITE));

    b.set(1, 0, WHITE);
    b.set(0, 1, WHITE);
    b.set(1, 1, WHITE);
    b.set(2, 1, WHITE);
    b.set(3, 1, WHITE);
    ASSERT_EQ(0, b.territoryCount(BLACK));
    ASSERT_EQ(11, b.territoryCount(WHITE));

    b.set(3, 3, BLACK);
    ASSERT_EQ(0, b.territoryCount(BLACK));
    ASSERT_EQ(3, b.territoryCount(WHITE));

    b.set(0, 2, BLACK);
    b.set(1, 2, BLACK);
    b.set(2, 2, BLACK);
    b.set(3, 2, BLACK);
    ASSERT_EQ(3, b.territoryCount(BLACK));
    ASSERT_EQ(3, b.territoryCount(WHITE));
}

TEST(BoardTest, CalculatesSingleCellPlayerTerritory) {
    Board b(5);
    b.set(0, 0, BLACK);
    b.set(0, 2, BLACK);
    b.set(2, 0, WHITE);
    b.set(4, 0, WHITE);
    b.set(3, 1, WHITE);
    b.set(2, 2, WHITE);
    b.set(4, 2, WHITE);
    b.set(1, 3, WHITE);
    b.set(3, 3, WHITE);
    b.set(0, 4, WHITE);
    b.set(2, 4, WHITE);
    b.set(4, 4, WHITE);
    ASSERT_EQ(0, b.territoryCount(BLACK));
    ASSERT_EQ(7, b.territoryCount(WHITE));
}

TEST(BoardTest, ThrowsExceptionWhenMakingInvalidMove) {
    Board b(9);
    b.set(0, 0, BLACK);
    ASSERT_THROW(b.set(0, 0, BLACK), std::runtime_error);
}

TEST(BoardTest, CapturesSingleOpponentPiece) {
    Board b(3);
    b.set(0, 1, BLACK);
    b.set(2, 1, BLACK);
    b.set(1, 0, BLACK);
    b.set(1, 1, WHITE);
    b.set(1, 2, BLACK);

    ASSERT_EQ(NONE, b.get(1, 1));
    ASSERT_EQ(1, b.captureCount(BLACK));
    ASSERT_EQ(0, b.captureCount(WHITE));
}

TEST(BoardTest, CapturesTerritoryAlongWall) {
    Board b(3);
    b.set(0, 0, BLACK);
    b.set(0, 1, BLACK);
    b.set(0, 2, BLACK);
    b.set(1, 0, WHITE);
    b.set(1, 1, WHITE);
    ASSERT_EQ(0, b.captureCount(BLACK));
    ASSERT_EQ(0, b.captureCount(WHITE));
    b.set(1, 2, WHITE);
    ASSERT_EQ(0, b.captureCount(BLACK));
    ASSERT_EQ(3, b.captureCount(WHITE));

    ASSERT_EQ(NONE, b.get(0, 0));
    ASSERT_EQ(NONE, b.get(0, 1));
    ASSERT_EQ(NONE, b.get(0, 2));
}

TEST(BoardTest, CalculatesCapturesFromLastPlacedStone) {
    Board b(3);
    b.set(1, 0, WHITE);
    b.set(0, 1, WHITE);

    b.set(1, 1, BLACK);
    b.set(0, 2, BLACK);
    b.set(0, 0, BLACK);

    EXPECT_EQ(1, b.captureCount(BLACK));
    EXPECT_EQ(0, b.captureCount(WHITE));
    EXPECT_EQ(NONE, b.get(0, 1));
}

TEST(BoardTest, CalculatesPlayerScoreAsTerritoryPlusCaptures) {
    Board b(4);
    b.set(0, 0, BLACK);
    b.set(3, 3, BLACK);
    b.set(1, 0, WHITE);
    b.set(1, 1, WHITE);
    b.set(0, 1, WHITE);

    ASSERT_EQ(0, b.captureCount(BLACK));
    ASSERT_EQ(1, b.captureCount(WHITE));
    ASSERT_EQ(0, b.territoryCount(BLACK));
    ASSERT_EQ(1, b.territoryCount(WHITE));
    ASSERT_EQ(2, b.score(WHITE));
    ASSERT_EQ(0, b.score(BLACK));
}
