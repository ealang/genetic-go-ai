#ifndef GPNODES_EXPERIMENTAL
#define GPNODES_EXPERIMENTAL

#include <string>
#include "gpnode.h"
#include "color.h"

GPNode* createRandomTree(int depth);

class PlusNode: public GPOperatorNode {
    int getUnscaled(const Board& b) const override;
public:
    PlusNode(GPNode* l, GPNode* r);
    GPNode* clone() const override;
    std::string toString() const override;
};

class MultiplyNode: public GPOperatorNode {
    int getUnscaled(const Board& b) const override;
public:
    MultiplyNode(GPNode* l, GPNode* r);
    GPNode* clone() const override;
    std::string toString() const override;
};

class ConstNode: public GPTerminalNode {
    const int v;
    int getUnscaled(const Board&) const override;
public:
    ConstNode(int v);
    GPNode* clone() const override;
    std::string toString() const override;
};

class RandomNode: public GPTerminalNode {
    const int min, max;
    int getUnscaled(const Board&) const override;
public:
    RandomNode(int min, int max);
    GPNode* clone() const override;
    std::string toString() const override;
};

class TerritoryNode: public GPTerminalNode {
    Color color;
    int getUnscaled(const Board&) const override;
public:
    TerritoryNode(Color color);
    GPNode* clone() const override;
    std::string toString() const override;
};

class ChainLengthNode: public GPTerminalNode {
    Color color;
    int getUnscaled(const Board&) const override;
public:
    ChainLengthNode(Color color);
    GPNode* clone() const override;
    std::string toString() const override;
};

class PlayerScoreNode: public GPTerminalNode {
    Color color;
    int getUnscaled(const Board&) const override;
public:
    PlayerScoreNode(Color color);
    GPNode* clone() const override;
    std::string toString() const override;
};

class PlayerCaptureNode: public GPTerminalNode {
    Color color;
    int getUnscaled(const Board&) const override;
public:
    PlayerCaptureNode(Color color);
    GPNode* clone() const override;
    std::string toString() const override;
};

#endif
