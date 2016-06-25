#ifndef BOARD_INTEL_H
#define BOARD_INTEL_H

#include "color.h"

class Board;

bool isSuicideMove(const Board&, int x, int y, Color);
bool canBeCaptured(const Board&, Color);
int stoneNetworkStrength(const Board&, Color);
int numLiberties(const Board&, Color);
int numLiberties(const Board&, int x, int y);
int maxClusterSize(const Board&, Color);
int numAdjacentStones(const Board&, bool inclDiag, int x, int y, Color);
int maxStoneSpacing(const Board&, Color);

#endif
