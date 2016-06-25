#ifndef BOARD_H
#define BOARD_H

#include <string>
#include "color.h"
#include "bitset2d.h"

// thrown if invalid move made due to KO rule
struct KoRuleViolated { };

class Board {
public:
    Board(int boardSize);

    int size() const;
    bool empty(int x, int y) const;
    Color get(int x, int y) const;
    void set(int x, int y, Color c);

    int territoryCount(Color) const;
    int captureCount(Color) const;
    int score(Color) const;

    std::string toString() const;

private:
    int boardSize;
    int blackCaptures, whiteCaptures;
    class BoardStorage {
        int size;
        Bitset2D black, white;
    public:
        BoardStorage(int size);
        Color get(int x, int y) const;
        const Bitset2D& blackMask() const;
        const Bitset2D& whiteMask() const;

        void set(int x, int y, Color color);
        void clearCells(const Bitset2D& mask);
    };
    BoardStorage board;

    struct MoveInfo {
        bool valid;
        int x, y;
        Color color;
        Bitset2D capture;
    };
    MoveInfo lastMove;

    bool isOutOfBounds(int x, int y) const;
    Bitset2D calculateCaptureMask(int x, int y, Color color) const;
    int countTerritoryFor(Color color) const;
    bool moveViolatesKoRule(const MoveInfo& lastMove, const MoveInfo& thisMove) const;
};

#endif
