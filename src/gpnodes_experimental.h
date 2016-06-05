#ifndef GPNODES_EXPERIMENTAL
#define GPNODES_EXPERIMENTAL

#include <string>
#include "gpnode.h"
#include "color.h"

GPNode* createRandomTree(int depth);

class PlusNode: public GPOperatorNode {
public:
    PlusNode(GPNode* l, GPNode* r);
    int get(const Board& b) const override;
    GPNode* clone() const override;
    std::string toString() const override;
};

class MultiplyNode: public GPOperatorNode {
public:
    MultiplyNode(GPNode* l, GPNode* r);
    int get(const Board& b) const override;
    GPNode* clone() const override;
    std::string toString() const override;
};

class ConstNode: public GPTerminalNode {
    const int v;
public:
    ConstNode(int v);
    int get(const Board&) const override;
    GPNode* clone() const override;
    std::string toString() const override;
};

class RandomNode: public GPTerminalNode {
    const int min, max;
public:
    RandomNode(int min, int max);
    int get(const Board&) const override;
    GPNode* clone() const override;
    std::string toString() const override;
};

class TerritoryNode: public GPTerminalNode {
    Color color;
public:
    TerritoryNode(Color color);
    int get(const Board&) const override;
    GPNode* clone() const override;
    std::string toString() const override;
};

class ChainLengthNode: public GPTerminalNode {
    Color color;
public:
    ChainLengthNode(Color color);
    int get(const Board&) const override;
    GPNode* clone() const override;
    std::string toString() const override;
};

#endif
