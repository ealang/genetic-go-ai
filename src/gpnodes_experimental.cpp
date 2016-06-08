#include <cstdlib>
#include <sstream>
#include <stdexcept>
#include "gpnodes_experimental.h"
#include "bitset2d.h"
#include "board.h"

using namespace std;

GPNode* createRandomNode(int curDepth, int maxDepth);
GPNode* createRandomTree(int depth) {
    return createRandomNode(1, depth);
}

/* PlusNode */

PlusNode::PlusNode(GPNode* l, GPNode* r) : GPOperatorNode(l, r) { }

int PlusNode::get(const Board& b) const { return nodes[0]->get(b) + nodes[1]->get(b); }

GPNode* PlusNode::clone() const {
    return new PlusNode(nodes[0]->clone(), nodes[1]->clone());
}

string PlusNode::toString() const {
    stringstream ss;
    ss << "PlusNode(" << nodes[0]->toString() << "," << nodes[1]->toString() << ")";
    return ss.str();
}

/* MultiplyNode */

MultiplyNode::MultiplyNode(GPNode* l, GPNode* r) : GPOperatorNode(l, r) { }

int MultiplyNode::get(const Board& b) const { return nodes[0]->get(b) * nodes[1]->get(b); }

GPNode* MultiplyNode::clone() const {
    return new MultiplyNode(nodes[0]->clone(), nodes[1]->clone());
}

string MultiplyNode::toString() const {
    stringstream ss;
    ss << "MultiplyNode(" << nodes[0]->toString() << "," << nodes[1]->toString() << ")";
    return ss.str();
}

/* ConstNode */

ConstNode::ConstNode(int v): v(v) {}

int ConstNode::get(const Board&) const { return v; }

GPNode* ConstNode::clone() const {
    return new ConstNode(v);
}

string ConstNode::toString() const {
    stringstream ss;
    ss << "ConstNode(" << v << ")";
    return ss.str();
}

/* RandomNode */

RandomNode::RandomNode(int min, int max): min(min), max(max) { }

int RandomNode::get(const Board&) const {
    return min + rand() % (max - min);
}

GPNode* RandomNode::clone() const {
    return new RandomNode(min, max);
}

string RandomNode::toString() const {
    stringstream ss;
    ss << "RandomNode(" << min << "," << max << ")";
    return ss.str();
}

/* TerritoryNode */

TerritoryNode::TerritoryNode(Color color): color(color) { }

int TerritoryNode::get(const Board& board) const {
    return board.territoryCount(color);
}

GPNode* TerritoryNode::clone() const {
    return new TerritoryNode(color);
}

string TerritoryNode::toString() const {
    stringstream ss;
    ss << "TerritoryNode(" << (color == BLACK ? "black" : "white") << ")";
    return ss.str();
}

/* ChainLengthNode */

ChainLengthNode::ChainLengthNode(Color color): color(color) { }

int ChainLengthNode::get(const Board& b) const {
    int len = 0;
    Bitset2D visited(b.size, b.size);
    b.iterateBoard([&](int x, int y) {
        if (b.get(x, y) == color && !visited.get(x, y)) {
            int count = 0;
            b.iterateConnectedStones(x, y, [&](int x, int y) {
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

GPNode* ChainLengthNode::clone() const {
    return new ChainLengthNode(color);
}

string ChainLengthNode::toString() const {
    stringstream ss;
    ss << "ChainLengthNode(" << (color == BLACK ? "black" : "white") << ")";
    return ss.str();
}

/* random tree */

GPNode* createRandomNode(int curDepth, int maxDepth) {
    static const int CONST_MIN = -10, CONST_MAX = 10;
    if (curDepth == maxDepth) {
        switch(rand() % 4) {
            case 0:
                return new ConstNode(CONST_MIN + rand() % (CONST_MAX - CONST_MIN));
            case 1:
                return new RandomNode(CONST_MIN, CONST_MAX);
            case 2:
                return new TerritoryNode(rand() % 2 == 0 ? BLACK : WHITE);
            case 3:
                return new ChainLengthNode(rand() % 2 == 0 ? BLACK : WHITE);
        }
    } else {
        switch(rand() % 2) {
            case 0:
                return new PlusNode(createRandomNode(curDepth + 1, maxDepth),
                                    createRandomNode(curDepth + 1, maxDepth));
            case 1:
                return new MultiplyNode(createRandomNode(curDepth + 1, maxDepth),
                                        createRandomNode(curDepth + 1, maxDepth));

        }
    }
    throw runtime_error("No node created");
    return nullptr;
}
