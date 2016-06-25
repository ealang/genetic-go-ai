#ifndef BOARD_ITER_H
#define BOARD_ITER_H

#include <functional>

class Board;

void iterateBoard(const Board&, std::function<bool(int, int)>);

void iterateStones(const Board&, Color, std::function<bool(int, int)>);

void iterateLiberties(const Board&, int x, int y, std::function<bool(int, int)>);

void iterateAdjacentCells(const Board&, int x, int y, std::function<void(int, int)>);
void iterateAdjacentDiagCells(const Board&, int x, int y, std::function<void(int, int)>);

void iterateConnectedStones(const Board&, int x, int y, bool includeDiag, std::function<bool(int, int)>);
void iterateConnectedStonesWithExtraStoneAt(const Board&, int x, int y,
                                            int extraX, int extraY, Color extraColor,
                                            bool includeDiag, std::function<bool(int, int)> callback);

// call back a cell in each connected cluster (x, y, clusterNumber)
void iterateClusters(const Board&, Color c, std::function<bool(int, int, int)>);

#endif
