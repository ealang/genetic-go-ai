#include <cstring>
#include <sstream>
#include <unordered_set>
#include <stack>
#include "color.h"
#include "board.h"

using namespace std;

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
    std::unordered_set<int> visited;
    auto explorePoint = [this, &visited, &myColor](int startPoint) {
        auto isOOB = [this](int i) {
            int x = i % size;
            int y = i / size;
            return x < 0 || y < 0 || x >= size || y >= size;
        };

        const int directions[] = {-1, 1, -size, size};

        int count = 0;
        bool touchMyTerritory = false, touchEnemyTerritory = false;

        std::stack<int> toVisit; toVisit.push(startPoint);

        while (toVisit.size() > 0) {
            int i = toVisit.top(); toVisit.pop();
            if (!visited.count(i)) {
                visited.insert(i);
                ++count;
                for (int dir: directions) {
                    int ii = i + dir;
                    if (!isOOB(ii)) {
                        Color c = get(ii);
                        if (c == NONE) 
                            toVisit.push(ii);
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
            if (!visited.count(i) && empty(x, y)) {
                territoryCount += explorePoint(i);
            }
        }
    }
    return territoryCount;
}

