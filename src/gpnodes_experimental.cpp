#include <cstdlib>
#include <sstream>
#include <stdexcept>
#include "gpnodes_experimental.h"
#include "bitset2d.h"
#include "board.h"
#include "board_iter.h"

using namespace std;

GPNode* createRandomNode(int curDepth, int maxDepth);
GPNode* createRandomTree(int depth) {
    return createRandomNode(1, depth);
}

/* PlusNode */

PlusNode::PlusNode(GPNode* l, GPNode* r) : GPOperatorNode(l, r) { }

int PlusNode::getImpl(const Board& b) const { return nodes[0]->get(b) + nodes[1]->get(b); }

GPNode* PlusNode::cloneImpl() const {
    return new PlusNode(nodes[0]->clone(), nodes[1]->clone());
}

string PlusNode::toString() const {
    stringstream ss;
    ss << "PlusNode(" << nodes[0]->toString() << "," << nodes[1]->toString() << ")";
    return ss.str();
}

/* MultiplyNode */

MultiplyNode::MultiplyNode(GPNode* l, GPNode* r) : GPOperatorNode(l, r) { }

int MultiplyNode::getImpl(const Board& b) const { return nodes[0]->get(b) * nodes[1]->get(b); }

GPNode* MultiplyNode::cloneImpl() const {
    return new MultiplyNode(nodes[0]->clone(), nodes[1]->clone());
}

string MultiplyNode::toString() const {
    stringstream ss;
    ss << "MultiplyNode(" << nodes[0]->toString() << "," << nodes[1]->toString() << ")";
    return ss.str();
}

/* IfLessThanNode */

IfLessThanNode::IfLessThanNode(GPNode* cmpA, GPNode* cmpB, GPNode* branch1, GPNode* branch2)
    : GPOperatorNode(cmpA, cmpB, branch1, branch2) { }

int IfLessThanNode::getImpl(const Board& b) const {
    if (nodes[0]->get(b) < nodes[1]->get(b)) {
        return nodes[2]->get(b);
    } else {
        return nodes[3]->get(b);
    }
}

GPNode* IfLessThanNode::cloneImpl() const {
    return new IfLessThanNode(nodes[0]->clone(),
                              nodes[1]->clone(),
                              nodes[2]->clone(),
                              nodes[3]->clone());
}

string IfLessThanNode::toString() const {
    stringstream ss;
    ss << "IfLessThanNode(" << nodes[0]->toString() << ","
                            << nodes[1]->toString() << ","
                            << nodes[2]->toString() << ","
                            << nodes[3]->toString() << ")";
    return ss.str();
}

/* ConstNode */

ConstNode::ConstNode(int v): v(v) {}

int ConstNode::getImpl(const Board&) const { return v; }

GPNode* ConstNode::cloneImpl() const {
    return new ConstNode(v);
}

string ConstNode::toString() const {
    stringstream ss;
    ss << "ConstNode(" << v << ")";
    return ss.str();
}

/* RandomNode */

RandomNode::RandomNode(int min, int max): min(min), max(max) { }

int RandomNode::getImpl(const Board&) const {
    return min + rand() % (max - min + 1);
}

GPNode* RandomNode::cloneImpl() const {
    return new RandomNode(min, max);
}

string RandomNode::toString() const {
    stringstream ss;
    ss << "RandomNode(" << min << "," << max << ")";
    return ss.str();
}

/* TerritoryNode */

TerritoryNode::TerritoryNode(Color color): color(color) { }

int TerritoryNode::getImpl(const Board& board) const {
    return board.territoryCount(color);
}

GPNode* TerritoryNode::cloneImpl() const {
    return new TerritoryNode(color);
}

string TerritoryNode::toString() const {
    stringstream ss;
    ss << "TerritoryNode(" << (color == BLACK ? "black" : "white") << ")";
    return ss.str();
}

/* ChainLengthNode */

ChainLengthNode::ChainLengthNode(Color color): color(color) { }

int ChainLengthNode::getImpl(const Board& b) const {
    int len = 0;
    Bitset2D visited(b.size, b.size);
    iterateBoard(b, [&](int x, int y) {
        if (b.get(x, y) == color && !visited.get(x, y)) {
            int count = 0;
            iterateConnectedStones(b, x, y, false, [&](int x, int y) {
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

GPNode* ChainLengthNode::cloneImpl() const {
    return new ChainLengthNode(color);
}

string ChainLengthNode::toString() const {
    stringstream ss;
    ss << "ChainLengthNode(" << (color == BLACK ? "black" : "white") << ")";
    return ss.str();
}

/* PlayerScoreNode */
 
PlayerScoreNode::PlayerScoreNode(Color color): color(color) { }

int PlayerScoreNode::getImpl(const Board& board) const {
    return board.score(color);
}

GPNode* PlayerScoreNode::cloneImpl() const {
    return new PlayerScoreNode(color);
}

string PlayerScoreNode::toString() const {
    stringstream ss;
    ss << "PlayerScoreNode(" << (color == BLACK ? "black" : "white") << ")";
    return ss.str();
}

/* PlayerCaptureNode */
 
PlayerCaptureNode::PlayerCaptureNode(Color color): color(color) { }

int PlayerCaptureNode::getImpl(const Board& board) const {
    return board.captureCount(color);
}

GPNode* PlayerCaptureNode::cloneImpl() const {
    return new PlayerCaptureNode(color);
}

string PlayerCaptureNode::toString() const {
    stringstream ss;
    ss << "PlayerCaptureNode(" << (color == BLACK ? "black" : "white") << ")";
    return ss.str();
}

/* LibertiesNode */
 
LibertiesNode::LibertiesNode(Color color): color(color) { }

int LibertiesNode::getImpl(const Board& board) const {
    Bitset2D visited(board.size, board.size);
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

GPNode* LibertiesNode::cloneImpl() const {
    return new LibertiesNode(color);
}

string LibertiesNode::toString() const {
    stringstream ss;
    ss << "LibertiesNode(" << (color == BLACK ? "black" : "white") << ")";
    return ss.str();
}

/* ClustersNode */
 
ClustersNode::ClustersNode(Color color, bool inclDiag): color(color), inclDiag(inclDiag) { }

int ClustersNode::getImpl(const Board& board) const {
    Bitset2D visited(board.size, board.size);
    int clusters = 0;
    iterateBoard(board, [&](int x, int y) {
        if (board.get(x, y) == color && !visited.get(x, y)) {
            ++clusters;
            iterateConnectedStones(board, x, y, inclDiag, [&](int x, int y) {
                visited.set(x, y);
                return true;
            });
        }
        return true;
    });
    return clusters;
}

GPNode* ClustersNode::cloneImpl() const {
    return new ClustersNode(color, inclDiag);
}

string ClustersNode::toString() const {
    stringstream ss;
    ss << "ClustersNode(" << (color == BLACK ? "black" : "white") << ","
                          << (inclDiag ? "includeDiag" : "excludeDiag") << ")";
    return ss.str();
}

/* random tree */

GPNode* createRandomNode(int curDepth, int maxDepth) {
    static const int CONST_MIN = -10, CONST_MAX = 10;
    static auto randcolor = [](){ return rand() % 2 == 0 ? BLACK : WHITE; };
    static auto randint = [](int min, int max){ return min + rand() % (max - min); };
    static vector<function<GPNode*(void)>> terminalFac = {
        [](){ return new ConstNode(randint(CONST_MIN, CONST_MAX + 1)); },
        [](){ return new RandomNode(CONST_MIN, CONST_MAX); },
        [](){ return new TerritoryNode(randcolor()); },
        [](){ return new ChainLengthNode(randcolor()); },
        [](){ return new PlayerScoreNode(randcolor()); },
        [](){ return new PlayerCaptureNode(randcolor()); },
        [](){ return new LibertiesNode(randcolor()); },
        [](){ return new ClustersNode(randcolor(), (bool)randint(0, 2)); },
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
