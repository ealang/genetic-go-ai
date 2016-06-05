#ifndef AIMATCH_H
#define AIMATCH_H

#include "color.h"

class Board;
class GPNode;

struct Move {
    int x, y;
};

Move getBestMove(const Board& board, const GPNode& aiLogic, Color color);

struct MatchResult {
    int blackScore, whiteScore;
    int numTurns;
};

MatchResult playAIMatch(const GPNode& black, const GPNode& white, int boardSize, int turnLimit);

#endif