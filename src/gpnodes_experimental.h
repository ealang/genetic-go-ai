#ifndef GPNODES_EXPERIMENTAL
#define GPNODES_EXPERIMENTAL

#include <string>
#include "gpnode.h"
#include "color.h"

GPNode* createRandomTree(int depth);

class PlusNode: public GPOperatorNode {
    int getImpl(const Board& b) const override;
public:
    PlusNode(GPNode* l, GPNode* r);
    GPNode* cloneImpl() const override;
    std::string toString() const override;
};

class MultiplyNode: public GPOperatorNode {
    int getImpl(const Board& b) const override;
public:
    MultiplyNode(GPNode* l, GPNode* r);
    GPNode* cloneImpl() const override;
    std::string toString() const override;
};

class IfLessThanNode: public GPOperatorNode {
    int getImpl(const Board& b) const override;
public:
    IfLessThanNode(GPNode* cmpA, GPNode* cmpB, GPNode* branch1, GPNode* branch2);
    GPNode* cloneImpl() const override;
    std::string toString() const override;
};

class ConstNode: public GPTerminalNode {
    const int v;
    int getImpl(const Board&) const override;
public:
    ConstNode(int v);
    GPNode* cloneImpl() const override;
    std::string toString() const override;
};

class RandomNode: public GPTerminalNode {
    const int min, max;
    int getImpl(const Board&) const override;
public:
    RandomNode(int min, int max);
    GPNode* cloneImpl() const override;
    std::string toString() const override;
};

class TerritoryNode: public GPTerminalNode {
    Color color;
    int getImpl(const Board&) const override;
public:
    TerritoryNode(Color color);
    GPNode* cloneImpl() const override;
    std::string toString() const override;
};

class ChainLengthNode: public GPTerminalNode {
    Color color;
    int getImpl(const Board&) const override;
public:
    ChainLengthNode(Color color);
    GPNode* cloneImpl() const override;
    std::string toString() const override;
};

class PlayerScoreNode: public GPTerminalNode {
    Color color;
    int getImpl(const Board&) const override;
public:
    PlayerScoreNode(Color color);
    GPNode* cloneImpl() const override;
    std::string toString() const override;
};

class PlayerCaptureNode: public GPTerminalNode {
    Color color;
    int getImpl(const Board&) const override;
public:
    PlayerCaptureNode(Color color);
    GPNode* cloneImpl() const override;
    std::string toString() const override;
};

class LibertiesNode: public GPTerminalNode {
    Color color;
    int getImpl(const Board&) const override;
public:
    LibertiesNode(Color color);
    GPNode* cloneImpl() const override;
    std::string toString() const override;
};

class ClustersNode: public GPTerminalNode {
    Color color;
    bool inclDiag;
    int getImpl(const Board&) const override;
public:
    ClustersNode(Color color, bool inclDiag);
    GPNode* cloneImpl() const override;
    std::string toString() const override;
};

#endif
