#include <cstdlib>
#include <sstream>
#include <stdexcept>
#include "gpnodes_experimental.h"
#include "bitset2d.h"
#include "board.h"
#include "board_iter.h"
#include "gpnode_context.h"

using namespace std;

GPNode* createRandomNode(int curDepth, int maxDepth);
GPNode* createRandomTree(int depth) {
    return createRandomNode(1, depth);
}

/* PlusNode */

PlusNode::PlusNode(GPNode* l, GPNode* r) : GPOperatorNode(l, r) { }

int PlusNode::getImpl(const Context& c) const { return nodes[0]->get(c) + nodes[1]->get(c); }

GPNode* PlusNode::cloneImpl() const {
    return new PlusNode(nodes[0]->clone(), nodes[1]->clone());
}

string PlusNode::toStringImpl() const {
    stringstream ss;
    ss << "PlusNode(" << nodes[0]->toString() << "," << nodes[1]->toString() << ")";
    return ss.str();
}

/* MultiplyNode */

MultiplyNode::MultiplyNode(GPNode* l, GPNode* r) : GPOperatorNode(l, r) { }

int MultiplyNode::getImpl(const Context& c) const { return nodes[0]->get(c) * nodes[1]->get(c); }

GPNode* MultiplyNode::cloneImpl() const {
    return new MultiplyNode(nodes[0]->clone(), nodes[1]->clone());
}

string MultiplyNode::toStringImpl() const {
    stringstream ss;
    ss << "MultiplyNode(" << nodes[0]->toString() << "," << nodes[1]->toString() << ")";
    return ss.str();
}

/* IfLessThanNode */

IfLessThanNode::IfLessThanNode(GPNode* cmpA, GPNode* cmpB, GPNode* branch1, GPNode* branch2)
    : GPOperatorNode(cmpA, cmpB, branch1, branch2) { }

int IfLessThanNode::getImpl(const Context& c) const {
    if (nodes[0]->get(c) < nodes[1]->get(c)) {
        return nodes[2]->get(c);
    } else {
        return nodes[3]->get(c);
    }
}

GPNode* IfLessThanNode::cloneImpl() const {
    return new IfLessThanNode(nodes[0]->clone(),
                              nodes[1]->clone(),
                              nodes[2]->clone(),
                              nodes[3]->clone());
}

string IfLessThanNode::toStringImpl() const {
    stringstream ss;
    ss << "IfLessThanNode(" << nodes[0]->toString() << ","
                            << nodes[1]->toString() << ","
                            << nodes[2]->toString() << ","
                            << nodes[3]->toString() << ")";
    return ss.str();
}

/* ConstNode */

ConstNode::ConstNode(int v): v(v) {}

int ConstNode::getImpl(const Context&) const { return v; }

GPNode* ConstNode::cloneImpl() const {
    return new ConstNode(v);
}

string ConstNode::toStringImpl() const {
    stringstream ss;
    ss << "ConstNode(" << v << ")";
    return ss.str();
}

/* RandomNode */

RandomNode::RandomNode(int min, int max): min(min), max(max) { }

int RandomNode::getImpl(const Context&) const {
    return min + rand() % (max - min + 1);
}

GPNode* RandomNode::cloneImpl() const {
    return new RandomNode(min, max);
}

string RandomNode::toStringImpl() const {
    stringstream ss;
    ss << "RandomNode(" << min << "," << max << ")";
    return ss.str();
}

/* ChainLengthDeltaNode */

ChainLengthDeltaNode::ChainLengthDeltaNode() { }

int ChainLengthDeltaNode::getImpl(const Context& c) const {
    auto chainLengthForBoard = [](const Board& board, Color color) {
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
    };
    return chainLengthForBoard(c.board, c.color) -
           chainLengthForBoard(c.prevBoard, c.color);
}

GPNode* ChainLengthDeltaNode::cloneImpl() const {
    return new ChainLengthDeltaNode();
}

string ChainLengthDeltaNode::toStringImpl() const {
    return "ChainLengthDeltaNode()";
}

/* PlayerScoreDeltaNode */
 
PlayerScoreDeltaNode::PlayerScoreDeltaNode() { }

int PlayerScoreDeltaNode::getImpl(const Context& c) const {
    return c.board.score(c.color) - c.prevBoard.score(c.color);
}

GPNode* PlayerScoreDeltaNode::cloneImpl() const {
    return new PlayerScoreDeltaNode();
}

string PlayerScoreDeltaNode::toStringImpl() const {
    return "PlayerScoreDeltaNode()";
}

/* LibertiesDeltaNode */
 
LibertiesDeltaNode::LibertiesDeltaNode(bool mine): mine(mine) { }

int LibertiesDeltaNode::getImpl(const Context& c) const {
    auto libertiesForBoard = [](const Board& board, Color color) {
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
    };

    Color color = mine ? c.color : otherColor(c.color);
    return (mine ? 1 : -1) *
           (libertiesForBoard(c.board, color) -
            libertiesForBoard(c.prevBoard, color));
}

GPNode* LibertiesDeltaNode::cloneImpl() const {
    return new LibertiesDeltaNode(mine);
}

string LibertiesDeltaNode::toStringImpl() const {
    stringstream ss;
    ss << "LibertiesDeltaNode(" << (mine ? "friendly" : "opponent") << ")";
    return ss.str();
}

/* MaxClusterDeltaNode */

MaxClusterDeltaNode::MaxClusterDeltaNode() { }

int MaxClusterDeltaNode::getImpl(const Context& c) const {
    auto countClusterAround = [](int x, int y, const Board& board, Color color) {
        int maxCluster = 0;
        Bitset2D visited(board.size(), board.size());
        iterateAdjacentDiagCells(board, x, y, [&](int x, int y) {
            if (!visited.get(x, y) && board.get(x, y) == color) {
                bool inclDiag = true;
                int clusterSize = 0;
                iterateConnectedStones(board, x, y, inclDiag, [&](int x, int y) {
                    if (!visited.get(x, y)) {
                        visited.set(x, y);
                        ++clusterSize;
                    }
                    return true;
                });
                maxCluster = max(clusterSize, maxCluster);
            }
        });
        return maxCluster;
    };
    return countClusterAround(c.x, c.y, c.board, c.color) -
           countClusterAround(c.x, c.y, c.prevBoard, c.color);
}

GPNode* MaxClusterDeltaNode::cloneImpl() const {
    return new MaxClusterDeltaNode();
}

string MaxClusterDeltaNode::toStringImpl() const {
    return "MaxClusterDeltaNode()";
}
/* CanBeCapturedNode */

CanBeCapturedNode::CanBeCapturedNode() { }

int CanBeCapturedNode::getImpl(const Context& c) const {
    bool wasCaptured = false;
    Color enemyColor = otherColor(c.color);
    iterateLiberties(c.board, c.x, c.y, [&](int x, int y){
        Board altBoard = c.board;
        try {
            altBoard.set(x, y, enemyColor);
        } catch (const KoRuleViolated&){
        }
        if (altBoard.captureCount(enemyColor) > c.board.captureCount(enemyColor)) {
            wasCaptured = true;
            return false;
        } else {
            return true;
        }
    });
    return wasCaptured ? -1 : 1;
}

GPNode* CanBeCapturedNode::cloneImpl() const {
    return new CanBeCapturedNode();
}

string CanBeCapturedNode::toStringImpl() const {
    return "CanBeCapturedNode(x, y)";
}

/* CanCaptureNode */

CanCaptureNode::CanCaptureNode() { }

int CanCaptureNode::getImpl(const Context& c) const {
    return c.prevBoard.captureCount(c.color) < c.board.captureCount(c.color);
}

GPNode* CanCaptureNode::cloneImpl() const {
    return new CanCaptureNode();
}

string CanCaptureNode::toStringImpl() const {
    return "CanCaptureNode(x, y)";
}

/* AdjacentStonesNode */

AdjacentStonesNode::AdjacentStonesNode(bool mine, bool inclDiag): mine(mine), inclDiag(inclDiag) { }

int AdjacentStonesNode::getImpl(const Context& c) const {
    int count = 0;
    Color color = mine ? c.color : otherColor(c.color);
    auto func = inclDiag ? iterateAdjacentDiagCells
                         : iterateAdjacentCells;
    func(c.board, c.x, c.y, [&](int x, int y) {
        if (c.board.get(x, y) == color) {
            ++count;
        }
    });
    return count;
}

GPNode* AdjacentStonesNode::cloneImpl() const {
    return new AdjacentStonesNode(mine, inclDiag);
}

string AdjacentStonesNode::toStringImpl() const {
    stringstream ss;
    ss << "AdjacentStonesNode(x, y, " << (mine ? "friendly" : "opponent") << ")";
    return ss.str();
}

/* random tree */

GPNode* createRandomNode(int curDepth, int maxDepth) {
    static const int CONST_MIN = -5, CONST_MAX = 5;
    static auto randint = [](int min, int max){ return min + rand() % (max - min); };
    static auto randbool = [](){ return (bool)(rand() % 2); };
    static vector<function<GPNode*(void)>> terminalFac = {
        [](){ return new ConstNode(randint(CONST_MIN, CONST_MAX + 1)); },
        [](){ return new RandomNode(CONST_MIN, CONST_MAX); },
        [](){ return new ChainLengthDeltaNode(); },
        [](){ return new PlayerScoreDeltaNode(); },
        [](){ return new LibertiesDeltaNode(randbool()); },
        [](){ return new MaxClusterDeltaNode(); },
        [](){ return new CanBeCapturedNode(); },
        [](){ return new CanCaptureNode(); },
        [](){ return new AdjacentStonesNode(randbool(), randbool()); },
    };
    static vector<function<GPNode*(int, int)>> internalFac = {
        [](int curDepth, int maxDepth) {
            return new PlusNode(createRandomNode(curDepth + 1, maxDepth),
                                createRandomNode(curDepth + 1, maxDepth));
        },
        [](int curDepth, int maxDepth){
            return new MultiplyNode(createRandomNode(curDepth + 1, maxDepth),
                                    createRandomNode(curDepth + 1, maxDepth));
        },
        [](int curDepth, int maxDepth){
            return new IfLessThanNode(createRandomNode(curDepth + 1, maxDepth),
                                      createRandomNode(curDepth + 1, maxDepth),
                                      createRandomNode(curDepth + 1, maxDepth),
                                      createRandomNode(curDepth + 1, maxDepth));
        },
    };

    if (curDepth == maxDepth) {
        return terminalFac[randint(0, terminalFac.size())]();
    } else {
        return internalFac[randint(0, internalFac.size())](curDepth, maxDepth);
    }
}
