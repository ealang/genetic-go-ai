#include <stdexcept>
#include "aimatch.h"
#include "board.h"
#include "gpnode.h"

Color nextTurn(Color c) { return c == BLACK ? WHITE : BLACK; }

Move getBestMove(const Board& board, const GPNode& aiLogic, Color color) {
    Move bestmove {0, 0};
    bool moved = false;
    int bestscore = 0;
    for (int x = 0; x < board.size; x++) {
        for (int y = 0; y < board.size; y++) {
            if (board.empty(x, y)) {
                auto newboard = board;
                newboard.set(x, y, color);
                int score = aiLogic.get(newboard);
                if (score >= bestscore || !moved) {
                    bestmove = Move{x, y};
                    bestscore = score;
                    moved = true;
                }
            }
        }
    }
    if (!moved) {
        throw std::runtime_error("No spaces available to play");
    }
    return bestmove;
}

MatchResult playAIMatch(const GPNode& black, const GPNode& white, int boardSize, int turnLimit) {
    Board board(boardSize);

    int nturns = 0;
    Color turn = BLACK;
    while (!board.full() && (nturns < turnLimit || turnLimit == -1)) {
        auto move = getBestMove(board, turn == BLACK ? black : white, turn);
        board.set(move.x, move.y, turn);
        turn = nextTurn(turn);
        ++nturns;
    }
    return MatchResult {board.blackTerritory(), board.whiteTerritory(), nturns};
}
