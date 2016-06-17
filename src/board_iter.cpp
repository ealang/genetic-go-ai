#include "board.h"
#include "board_iter.h"

using namespace std;

struct Direction {
    const int x, y;
};
static const Direction ADJ_DIRECTIONS[] = {Direction{-1, 0},
                                           Direction{1, 0},
                                           Direction{0, -1},
                                           Direction{0, 1}};
static const Direction ADJ_DIAG_DIRECTIONS[] = {Direction{-1, 0},
                                                Direction{1, 0},
                                                Direction{0, -1},
                                                Direction{0, 1},
                                                Direction{-1, -1},
                                                Direction{1, 1},
                                                Direction{-1, 1},
                                                Direction{1, -1}};

void iterateBoard(const Board& board, std::function<bool(int, int)> callback) {
    for (int x = 0; x < board.size; x++) {
        for (int y = 0; y < board.size; y++) {
            if (!callback(x, y)) {
                return;
            }
        }
    }
}

void iterateLiberties(const Board& board, int x, int y, std::function<bool(int, int)> callback) {
    if (board.get(x, y) != NONE) {
        Bitset2D visited(board.size, board.size);
        iterateConnectedStones(board, x, y, false, [&](int x, int y) {
            bool continueIterating = true;
            iterateAdjacentCells(board, x, y, [&](int x, int y) {
                if (continueIterating && board.get(x, y) == NONE && !visited.get(x, y)) {
                    visited.set(x, y);
                    continueIterating = callback(x, y) && continueIterating;
                }
            });
            return continueIterating;
        });
    }
}

void iterateAdjacentCells(const Board& board, int x, int y, std::function<void(int, int)> callback) {
    for (auto dir: ADJ_DIRECTIONS) {
        int xx = x + dir.x,
            yy = y + dir.y;
        if (xx >= 0 && yy >= 0 && xx < board.size && yy < board.size) {
            callback(xx, yy);
        }
    }
}

void iterateAdjacentDiagCells(const Board& board, int x, int y, std::function<void(int, int)> callback) {
    for (auto dir: ADJ_DIAG_DIRECTIONS) {
        int xx = x + dir.x,
            yy = y + dir.y;
        if (xx >= 0 && yy >= 0 && xx < board.size && yy < board.size) {
            callback(xx, yy);
        }
    }
}

void iterateConnectedStones(const Board& board, int x, int y, bool includeDiag, std::function<bool(int, int)> callback) {
    return iterateConnectedStonesWithExtraStoneAt(board, x, y, -1, -1, NONE, includeDiag, callback);
}

void iterateConnectedStonesWithExtraStoneAt(const Board& board, int startX, int startY, int extraX, int extraY, Color extraColor, bool includeDiag, std::function<bool(int, int)> callback) {
    Bitset2D visited(board.size, board.size);
    auto getWithExtra = [&](int x, int y) {
        if (x == extraX && y == extraY) {
            return extraColor;
        } else {
            return board.get(x, y);
        }
    };
    Color color = getWithExtra(startX, startY);
    function<void(int, int)> r = [&](int x, int y) {
        visited.set(x, y);
        if (!callback(x, y)) {
            return;
        }
        auto countfunc = [&](int x, int y) {
            if (getWithExtra(x, y) == color && !visited.get(x, y)) {
                r(x, y);
            }
        };
        if (includeDiag) {
            iterateAdjacentDiagCells(board, x, y, countfunc);
        } else {
            iterateAdjacentCells(board, x, y, countfunc);
        }
    };
    r(startX, startY);
}
