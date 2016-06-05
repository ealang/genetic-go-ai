#ifndef BOARD_H
#define BOARD_H

#include <string>
#include <unordered_map>
#include "color.h"

class Board {
public:
    Board(int size);

    const int size;
    bool empty(int x, int y) const;
    Color get(int x, int y) const;
    bool full() const;
    int blackTerritory() const;
    int whiteTerritory() const;
    std::string toString() const;

    void set(int x, int y, Color c);

private:
    int nused;
    std::unordered_map<int, Color> board;
    Color get(int i) const;
    int toBoardNum(int x, int y) const;
    int countTerritoryFor(Color color) const;
};

#endif
