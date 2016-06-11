#include <cstring>
#include <functional>
#include <sstream>
#include <stack>
#include "color.h"
#include "bitset2d.h"
#include "board.h"

using namespace std;

struct Direction {
    const int x, y;
};
static const Direction DIRECTIONS[] = {Direction{-1, 0},
                                       Direction{1, 0},
                                       Direction{0, -1},
                                       Direction{0, 1}};

Board::Board(int size): size(size), board(BoardStorage(size)),
    blackCaptures(0),
    whiteCaptures(0) {
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

string Board::toString() const {
    stringstream ss;
    char chars[] = {' ', 'B', 'W'};
    char sep[size * 2 + 2];
    sep[size * 2 + 1] = 0;
    memset(sep, '-', size * 2 + 1);
    for (int y = 0; y < size; y++) {
        ss << '|';
        for (int x = 0; x < size; x++) {
            ss << chars[get(x, y)] << '|';
        }
        ss << endl << sep << endl;
    }
    return ss.str();
}

void Board::set(int x, int y, Color color) {
    if (!empty(x, y) && color != NONE) {
        throw runtime_error("Cell already occupied");
    } else {
        board.set(x, y, color);
        int blackRemoved, whiteRemoved;
        removeCapturedStones(x, y, blackRemoved, whiteRemoved);
        blackCaptures += whiteRemoved;
        whiteCaptures += blackRemoved;
    }
}

bool Board::isOutOfBounds(int x, int y) const {
    return x < 0 || y < 0 || x >= size || y >= size;
}

Board::BoardStorage::BoardStorage(int size): size(size) { }

Color Board::BoardStorage::get(int x, int y) const {
    int i = x + y * size;
    auto it = cells.find(i);
    if (it != cells.end()) {
        return it->second;
    } else {
        return NONE;
    }
}

void Board::BoardStorage::set(int x, int y, Color color) {
    cells[x + y * size] = color;
}

void Board::removeCapturedStones(int x, int y, int& blackRemoved, int& whiteRemoved) {
    blackRemoved = 0;
    whiteRemoved = 0;
    Bitset2D visited(size, size);

    auto processCapturesAt = [&](int x, int y) {
        Color color = get(x, y);
        if (color != NONE && !visited.get(x, y)) {
            bool isSurrounded = true;
            iterateConnectedStones(x, y, [&](int x, int y) {
                visited.set(x, y, true);
                iterateAdjacentCells(x, y, [&](int xx, int yy) {
                    if (get(xx, yy) == NONE) {
                        isSurrounded = false;
                    }
                });
                return isSurrounded;
            });

            if (isSurrounded) {
                iterateConnectedStones(x, y, [&](int x, int y) {
                    if (color == BLACK)
                        ++blackRemoved;
                    else 
                        ++whiteRemoved;
                    set(x, y, NONE);
                    return true;
                });
            }
        }
    };

    iterateAdjacentCells(x, y, processCapturesAt);
    processCapturesAt(x, y);
}

int Board::countTerritoryFor(Color myColor) const {
    Bitset2D visited(size, size);
    auto explorePoint = [&](int x, int y) {
        int count = 0;
        bool touchMyTerritory = false, touchEnemyTerritory = false;

        iterateConnectedStones(x, y, [&](int x, int y) {
            visited.set(x, y, true);
            ++count;
            iterateAdjacentCells(x, y, [&](int x, int y) {
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
    iterateBoard([&](int x, int y) {
        if (!visited.get(x, y) && empty(x, y)) {
            territoryCount += explorePoint(x, y);
        }
        return true;
    });

    return territoryCount;
}

void Board::iterateConnectedStones(int x, int y, const std::function<bool(int, int)>& callback) const {
    Bitset2D visited(size, size);
    Color color = get(x, y);
    function<void(int, int)> r = [&](int x, int y) {
        visited.set(x, y, true);
        if (!callback(x, y)) {
            return;
        }
        iterateAdjacentCells(x, y, [&](int x, int y) {
            if (get(x, y) == color && !visited.get(x, y)) {
                r(x, y);
            }
        });
    };
    r(x, y);
}

void Board::iterateAdjacentCells(int x, int y, const std::function<void(int, int)>& callback) const {
    for (auto dir: DIRECTIONS) {
        int xx = x + dir.x,
            yy = y + dir.y;
        if (xx >= 0 && yy >= 0 && xx < size && yy < size) {
            callback(xx, yy);
        }
    }
}

void Board::iterateBoard(const std::function<bool(int, int)>& callback) const {
    for (int x = 0; x < size; x++) {
        for (int y = 0; y < size; y++) {
            if (!callback(x, y)) {
                return;
            }
        }
    }
}
