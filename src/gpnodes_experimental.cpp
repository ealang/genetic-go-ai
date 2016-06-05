#include <cstdlib>
#include <sstream>
#include <stdexcept>
#include "gpnodes_experimental.h"
#include "bitset.h"
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
    if (color == BLACK) return board.blackTerritory();
    else return board.whiteTerritory();
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

int countConnectedChains(int x, int y, Color color, const Board& b, Bitset& visited) {
    const int directions[][2] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
    auto isOOB = [&b](int x, int y) {
        return x < 0 || y < 0 || x >= b.size || y >= b.size;
    };

    int count = 1;
    visited.set(x + y * b.size, true);
    for (auto dir: directions) {
        int xx = x + dir[0],
            yy = y + dir[1];
        int ii = xx + yy * b.size;
        if (!isOOB(xx, yy) && !visited.get(ii) && b.get(xx, yy) == color) {
            count += countConnectedChains(xx, yy, color, b, visited);
        }
    }
    return count;

}

int ChainLengthNode::get(const Board& b) const {
    int len = 0;
    Bitset visited(b.size * b.size);
    for (int x = 0; x < b.size; x++) {
        for (int y = 0; y < b.size; y++) {
            if (b.get(x, y) == color && !visited.get(x + y * b.size)) {
                int chainlen = countConnectedChains(x, y, color, b, visited);
                if (chainlen > 1) {
                    len += chainlen;
                }
            }
        }
    }
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
