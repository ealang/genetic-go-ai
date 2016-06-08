#ifndef BOARD_H
#define BOARD_H

#include <string>
#include <unordered_map>
#include <functional>
#include "color.h"

class Board {
public:
    Board(int size);

    const int size;
    bool empty(int x, int y) const;
    Color get(int x, int y) const;
    void set(int x, int y, Color c);

    int territoryCount(Color) const;
    int captureCount(Color) const;
    int score(Color) const;

    std::string toString() const;

    void iterateConnectedStones(int x, int y, const std::function<bool(int, int)>&) const;
    void iterateAdjacentCells(int x, int y, const std::function<void(int, int)>&) const;
    void iterateBoard(const std::function<bool(int, int)>&) const;
private:
    class BoardStorage {
        int size;
        std::unordered_map<int, Color> cells;
    public:
        BoardStorage(int);
        Color get(int x, int y) const;
        void set(int x, int y, Color color);
    };
    BoardStorage board;

    int blackCaptures, whiteCaptures;
    bool isOutOfBounds(int x, int y) const;
    void removeCapturedStones(int x, int y, int& blackRemoved, int& whiteRemoved);
    int countTerritoryFor(Color color) const;
};

#endif
