#include <stdexcept>
#include <cstring>
#include <functional>
#include <sstream>
#include "color.h"
#include "bitset2d.h"
#include "board.h"
#include "board_iter.h"

using namespace std;

Board::Board(int boardSize):
    boardSize(boardSize),
    blackCaptures(0),
    whiteCaptures(0),
    board(BoardStorage(boardSize)),
    lastMove({false, 0, 0, NONE, Bitset2D(boardSize, boardSize)}) {
}

int Board::size() const {
    return boardSize;
}

bool Board::empty(int x, int y) const {
    return board.get(x, y) == NONE;
}

Color Board::get(int x, int y) const {
    return board.get(x, y);
}

int Board::territoryCount(Color color) const {
    return countTerritoryFor(color);
}

int Board::captureCount(Color c) const {
    return c == BLACK ? blackCaptures : whiteCaptures;
}

int Board::score(Color c) const {
    return territoryCount(c) + captureCount(c);
}

bool Board::isSuicideMove(int x, int y, Color forColor) const {
    Color enemyColor = otherColor(forColor);
    Board possible = *this;
    try {
        possible.set(x, y, forColor);
        return captureCount(enemyColor) < possible.captureCount(enemyColor);
    } catch (const KoRuleViolated&) {
        return false;
    }
}

string Board::toString() const {
    stringstream ss;
    char chars[] = {' ', 'B', 'W'};
    char sep[boardSize * 2 + 2];
    sep[boardSize * 2 + 1] = 0;
    memset(sep, '-', boardSize * 2 + 1);
    for (int y = 0; y < boardSize; y++) {
        ss << '|';
        for (int x = 0; x < boardSize; x++) {
            ss << chars[get(x, y)] << '|';
        }
        ss << endl << sep << endl;
    }
    return ss.str();
}

void Board::set(int x, int y, Color color) {
    if (isOutOfBounds(x, y)) {
        throw runtime_error("Cell is out of bounds");
    } else if (!empty(x, y) && color != NONE) {
        throw runtime_error("Cell is already occupied");
    } else {
        Bitset2D captureMask = calculateCaptureMask(x, y, color);
        MoveInfo thisMove = {true, x, y, color, captureMask};
        if (!moveViolatesKoRule(lastMove, thisMove)) {
            board.set(x, y, color);
            int blackRemoved = (board.blackMask() & captureMask).count(),
                whiteRemoved = (board.whiteMask() & captureMask).count();
            blackCaptures += whiteRemoved;
            whiteCaptures += blackRemoved;
            board.clearCells(captureMask);
            lastMove = thisMove;
        } else {
            throw KoRuleViolated();
        }
    }
}

bool Board::moveViolatesKoRule(const MoveInfo& lastMove, const MoveInfo& thisMove) const {
    return lastMove.valid &&
           thisMove.capture.get(lastMove.x, lastMove.y) &&
           lastMove.capture.get(thisMove.x, thisMove.y) &&
           lastMove.color != thisMove.color &&
           lastMove.capture.count() == 1 &&
           thisMove.capture.count() == 1;
}

bool Board::isOutOfBounds(int x, int y) const {
    return x < 0 || y < 0 || x >= boardSize || y >= boardSize;
}

Bitset2D Board::calculateCaptureMask(int moveX, int moveY, Color moveColor) const {
    Bitset2D visited(boardSize, boardSize);
    Bitset2D captureMask(boardSize, boardSize);

    auto getColorWithNewMove = [&](int x, int y) {
        if (x == moveX && y == moveY) {
            return moveColor;
        } else if (captureMask.get(x, y)) {
            return NONE; // stone will have been removed
        } else {
            return get(x, y);
        }
    };

    auto processCapturesAt = [&](int x, int y) {
        Color color = getColorWithNewMove(x, y);
        if (color != NONE && !visited.get(x, y)) {
            bool isSurrounded = true;
            iterateConnectedStonesWithExtraStoneAt(*this, x, y, moveX, moveY, moveColor, false, [&](int x, int y) {
                visited.set(x, y);
                iterateAdjacentCells(*this, x, y, [&](int x, int y) {
                    if (getColorWithNewMove(x, y) == NONE) {
                        isSurrounded = false;
                    }
                });
                return isSurrounded;
            });

            if (isSurrounded) {
                iterateConnectedStonesWithExtraStoneAt(*this, x, y, moveX, moveY, moveColor, false, [&](int x, int y) {
                    captureMask.set(x, y);
                    return true;
                });
            }
        }
    };

    iterateAdjacentCells(*this, moveX, moveY, processCapturesAt);
    processCapturesAt(moveX, moveY);
    return captureMask;
}

int Board::countTerritoryFor(Color myColor) const {
    Bitset2D visited(boardSize, boardSize);
    auto explorePoint = [&](int x, int y) {
        int count = 0;
        bool touchMyTerritory = false, touchEnemyTerritory = false;

        iterateConnectedStones(*this, x, y, false, [&](int x, int y) {
            visited.set(x, y, true);
            ++count;
            iterateAdjacentCells(*this, x, y, [&](int x, int y) {
                Color c = get(x, y);
                if (c == myColor) {
                    touchMyTerritory = true;
                } else if (c != NONE) {
                    touchEnemyTerritory = true;
                }
            });
            return true;
        });

        if (touchMyTerritory && !touchEnemyTerritory) {
            return count;
        } else {
            return 0;
        }
    };

    int territoryCount = 0;
    iterateBoard(*this, [&](int x, int y) {
        if (!visited.get(x, y) && empty(x, y)) {
            territoryCount += explorePoint(x, y);
        }
        return true;
    });

    return territoryCount;
}

Board::BoardStorage::BoardStorage(int size): 
    size(size),
    black(Bitset2D(size, size)),
    white(Bitset2D(size, size)) { 
}

Color Board::BoardStorage::get(int x, int y) const {
    if (black.get(x, y)) {
        return BLACK;
    } else if (white.get(x, y)) {
        return WHITE;
    } else {
        return NONE;
    }
}

void Board::BoardStorage::set(int x, int y, Color color) {
    black.set(x, y, color == BLACK);
    white.set(x, y, color == WHITE);
}

void Board::BoardStorage::clearCells(const Bitset2D& mask) {
    black &= ~mask;
    white &= ~mask;
}

const Bitset2D& Board::BoardStorage::blackMask() const {
    return black;
}

const Bitset2D& Board::BoardStorage::whiteMask() const {
    return white;
}
