#include <algorithm>
#include <stdexcept>
#include <cassert>
#include "board.h"
#include "board_intel.h"
#include "board_iter.h"

using namespace std;

bool isSuicideMove(const Board& board, int x, int y, Color color) {
    Color enemyColor = otherColor(color);
    Board possible = board;
    try {
        possible.set(x, y, color);
        return board.captureCount(enemyColor) < possible.captureCount(enemyColor);
    } catch (const KoRuleViolated&) {
        return false;
    }
}

int stoneNetworkStrength(const Board& board, Color color) {
    int len = 0;
    Bitset2D visited(board.size(), board.size());
    iterateBoard(board, [&](int x, int y) {
        if (board.get(x, y) == color && !visited.get(x, y)) {
            int count = 0;
            iterateConnectedStones(board, x, y, false, [&](int x, int y) {
                visited.set(x, y, true);
                ++count;
                return true;
            });
            if (count > 1) {
                len += count;
            }
        }
        return true;
    });
    return len;
}

int numLiberties(const Board& board, Color color) {
    Bitset2D visited(board.size(), board.size());
    int liberties = 0;
    iterateBoard(board, [&](int x, int y) {
        if (board.get(x, y) == color) {
            iterateAdjacentCells(board, x, y, [&](int x, int y) {
                if (board.get(x, y) == NONE && !visited.get(x, y)) {
                    visited.set(x, y);
                    ++liberties;
                }
            });
        }
        return true;
    });
    return liberties;
}

int numLiberties(const Board& board, int x, int y) {
    if (board.get(x, y) == NONE) {
        throw runtime_error("Cannot iterate over empty cluster");
    }
    Bitset2D visited(board.size(), board.size());
    int liberties = 0;
    bool inclDiag = true;
    iterateConnectedStones(board, x, y, !inclDiag, [&](int x, int y) {
        iterateAdjacentCells(board, x, y, [&](int x, int y) {
            if (board.get(x, y) == NONE && !visited.get(x, y)) {
                visited.set(x, y);
                ++liberties;
            }
        });
        return true;
    });
    return liberties;
}

int maxClusterSize(const Board& board, Color color) {
    int maxCluster = 0;
    Bitset2D visited(board.size(), board.size());

    iterateStones(board, color, [&](int x, int y) {
        if (!visited.get(x, y)) {
            int clusterSize = 0;
            bool inclDiag = true;
            iterateConnectedStones(board, x, y, inclDiag, [&](int x, int y) {
                if (!visited.get(x, y)) {
                    visited.set(x, y);
                    ++clusterSize;
                }
                return true;
            });
            maxCluster = std::max(clusterSize, maxCluster);
        }
        return true;
    });
    return maxCluster;
}

bool canBeCaptured(const Board& board, Color color) {
    bool canBe = false;
    iterateClusters(board, color, [&](int x, int y, int) {
        assert(board.get(x, y) == color);
        canBe = canBe || (numLiberties(board, x, y) == 1);
        return !canBe;
    });
    return canBe;
}

int numAdjacentStones(const Board& board, bool inclDiag, int x, int y, Color color) {
    int count = 0;
    auto func = inclDiag ? iterateAdjacentDiagCells
                         : iterateAdjacentCells;
    func(board, x, y, [&](int x, int y) {
        if (board.get(x, y) == color) {
            ++count;
        }
    });
    return count;
}

int maxStoneSpacing(const Board& board, Color color) {
    auto xyToI = [&board](int x, int y) { return x + y * board.size(); };
    int bs = board.size();
    vector<int> dist(bs * bs, bs * 2);
    iterateStones(board, color, [&](int x, int y) {
        dist[xyToI(x, y)] = 0;
        return true;
    });
    for (int i = 0; i < bs * 2; i++) {
        bool stillChanging = false;
        iterateBoard(board, [&](int x, int y) {
            int pval = dist[xyToI(x, y)] + 1;
            iterateAdjacentCells(board, x, y, [&](int x, int y) {
                int val = dist[xyToI(x, y)];
                if (pval < val) {
                    dist[xyToI(x, y)] = pval;
                    stillChanging = true;
                }
            });
            return true;
        });
        if (!stillChanging) {
            break;
        }
    }
    return *max_element(dist.begin(), dist.end());
}
