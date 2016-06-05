#include <stdexcept>
#include "gtest/gtest.h"
#include "aimatch.h"
#include "board.h"
#include "gptree.h"
#include "gpnodes_experimental.h"

TEST(AIMatchTest, CanPlayGame) {
    ConstNode ai(1);
    auto result = playAIMatch(ai, ai, 3, -1);
    ASSERT_EQ(9, result.numTurns);
}

TEST(AIMatchTest, CanGetMove) {
    auto board = Board(9);
    auto node = ConstNode(1);
    auto move = getBestMove(board, node, BLACK);
    EXPECT_EQ(8, move.x);
    EXPECT_EQ(8, move.y);
}

TEST(AIMatchTest, ThrowsExceptionWhenNoMovesPossible) {
    auto board = Board(2);
    board.set(0, 0, WHITE);
    board.set(0, 1, WHITE);
    board.set(1, 0, WHITE);
    board.set(1, 1, WHITE);
    auto node = ConstNode(1);

    ASSERT_THROW(getBestMove(board, node, BLACK), std::runtime_error);
}
