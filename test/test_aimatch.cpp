#include <stdexcept>
#include "gtest/gtest.h"
#include "aimatch.h"
#include "board.h"
#include "gptree.h"
#include "gpnodes_experimental.h"

TEST(AIMatchTest, CanPlayGame) {
    IntConstNode ai(1);
    auto result = playAIMatch(ai, ai, BLACK, 3, 9);
    ASSERT_EQ(9, result.blackScore);
    ASSERT_EQ(1, result.whiteScore);
}

TEST(AIMatchTest, CanGetMove) {
    auto board = Board(9);
    auto node = IntConstNode(1);
    auto move = getBestMove(board, node, BLACK);
    EXPECT_EQ(8, move.x);
    EXPECT_EQ(8, move.y);
}

TEST(AIMatchTest, DoesntMakeSuicideMoves) {
    auto board = Board(9);
    board.set(0, 1, BLACK);
    board.set(1, 1, BLACK);
    board.set(2, 0, BLACK);
    board.set(0, 0, WHITE);

    auto ai = PlayerScoreDeltaNode(); 
    ai.setScale(-1); // really wants to give other player points

    auto move = getBestMove(board, ai, WHITE);
    ASSERT_NE((Move{1, 0}), move);
}
