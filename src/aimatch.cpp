#include <stdexcept>
#include "aimatch.h"
#include "board.h"
#include "gpnode.h"
#include "gpnode_context.h"
#include "board_intel.h"

// thrown if no move is possible
struct NoMoveAvailable {};

bool Move::operator!=(const Move& other) const {
    return !(x == other.x && y == other.y);
}

Move getBestMove(const Board& board, const GPNode& aiLogic, Color color) {
    Move bestmove {0, 0};
    bool moved = false;
    int bestscore = 0;
    for (int x = 0; x < board.size(); x++) {
        for (int y = 0; y < board.size(); y++) {
            if (board.empty(x, y) && !isSuicideMove(board, x, y, color)) {
                auto newboard = board;
                try {
                    newboard.set(x, y, color);
                    int score = aiLogic.get(Context{x, y, color, board, newboard});
                    if (score >= bestscore || !moved) {
                        bestmove = Move{x, y};
                        bestscore = score;
                        moved = true;
                    }
                } catch (const KoRuleViolated&) {
                }
            }
        }
    }
    if (!moved) {
        throw NoMoveAvailable();
    }
    return bestmove;
}

MatchResult playAIMatch(const GPNode& black, const GPNode& white, Color turn, int boardSize, int turnLimit) {
    Board board(boardSize);

    int nturns = 0;
    while ((nturns < turnLimit || turnLimit == -1) && nturns < boardSize * boardSize) {
        try {
            auto move = getBestMove(board, turn == BLACK ? black : white, turn);
            board.set(move.x, move.y, turn);
        } catch (const NoMoveAvailable&) {
            break;
        }
        turn = otherColor(turn);
        ++nturns;
    }
    return MatchResult {board.score(BLACK),
                        board.score(WHITE),
                        nturns};
}
