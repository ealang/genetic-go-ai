#include <cstring>
#include <sstream>
#include <stack>
#include "color.h"
#include "bitset.h"
#include "board.h"

using namespace std;

struct Direction {
    const int x, y;
};
const Direction DIRECTIONS[] = {Direction{-1, 0},
                                Direction{1, 0},
                                Direction{0, -1},
                                Direction{0, 1}};

Board::Board(int size): size(size), nused(0)  {
}

bool Board::empty(int x, int y) const {
    int i = toBoardNum(x, y);
    return board.count(i) == 0;
}

Color Board::get(int x, int y) const {
    return get(toBoardNum(x, y));
}

Color Board::get(int i) const {
    if (board.count(i)) {
        return board.find(i)->second;
    } else {
        return NONE;
    }
}

bool Board::full() const {
    return nused == size * size;
}

int Board::blackTerritory() const {
    return countTerritoryFor(BLACK);
}

int Board::whiteTerritory() const {
    return countTerritoryFor(WHITE);
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

void Board::set(int x, int y, Color c) {
    int i = toBoardNum(x, y);
    if (board.count(i)) {
        throw std::runtime_error("Cell already occupied");
    }
    ++nused;
    board[i] = c;
}

int Board::toBoardNum(int x, int y) const {
    return x + y * size;
}

int Board::countTerritoryFor(Color myColor) const {
    Bitset visited(size * size);
    auto explorePoint = [this, &visited, &myColor](int startPoint) {
        auto isOOB = [this](int x, int y) {
            return x < 0 || y < 0 || x >= size || y >= size;
        };
        auto iToX = [this](int i) { return i % size; };
        auto iToY = [this](int i) { return i / size; };
        auto xyToI = [this](int x, int y) { return x + y * size; };

        int count = 0;
        bool touchMyTerritory = false, touchEnemyTerritory = false;

        std::stack<int> toVisit; toVisit.push(startPoint);

        while (toVisit.size() > 0) {
            int i = toVisit.top(); toVisit.pop();
            if (!visited.get(i)) {
                visited.set(i, true);
                ++count;

                for (auto dir: DIRECTIONS) {
                    int x = iToX(i) + dir.x, 
                        y = iToY(i) + dir.y;
                    if (!isOOB(x, y)) {
                        Color c = get(x, y);
                        if (c == NONE) 
                            toVisit.push(xyToI(x, y));
                        else if (c == myColor) {
                            touchMyTerritory = true;
                        } else {
                            touchEnemyTerritory = true;
                        }
                    }
                }        
            }
        }
        if (touchMyTerritory && !touchEnemyTerritory) {
            return count;
        } else {
            return 0;
        }
    };

    int territoryCount = 0;
    for (int x = 0; x < size; x++) {
        for (int y = 0; y < size; y++) {
            int i = toBoardNum(x, y);
            if (!visited.get(i) && empty(x, y)) {
                territoryCount += explorePoint(i);
            }
        }
    }
    return territoryCount;
}

