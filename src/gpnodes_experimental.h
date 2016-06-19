#ifndef GPNODES_EXPERIMENTAL
#define GPNODES_EXPERIMENTAL

#include <string>
#include "gpnode.h"
#include "color.h"

GPNode* createRandomTree(int depth);

class PlusNode: public GPOperatorNode {
    int getImpl(const Context&) const override;
    GPNode* cloneImpl() const override;
    std::string toStringImpl() const override;
public:
    PlusNode(GPNode* l, GPNode* r);
};

class MultiplyNode: public GPOperatorNode {
    int getImpl(const Context&) const override;
    GPNode* cloneImpl() const override;
    std::string toStringImpl() const override;
public:
    MultiplyNode(GPNode* l, GPNode* r);
};

class IfLessThanNode: public GPOperatorNode {
    int getImpl(const Context&) const override;
    GPNode* cloneImpl() const override;
    std::string toStringImpl() const override;
public:
    IfLessThanNode(GPNode* cmpA, GPNode* cmpB, GPNode* branch1, GPNode* branch2);
};

class ConstNode: public GPTerminalNode {
    const int v;
    int getImpl(const Context&) const override;
    GPNode* cloneImpl() const override;
    std::string toStringImpl() const override;
public:
    ConstNode(int v);
};

class RandomNode: public GPTerminalNode {
    const int min, max;
    int getImpl(const Context&) const override;
    GPNode* cloneImpl() const override;
    std::string toStringImpl() const override;
public:
    RandomNode(int min, int max);
};

class ChainLengthDeltaNode: public GPTerminalNode {
    int getImpl(const Context&) const override;
    GPNode* cloneImpl() const override;
    std::string toStringImpl() const override;
public:
    ChainLengthDeltaNode();
};

class PlayerScoreDeltaNode: public GPTerminalNode {
    int getImpl(const Context&) const override;
    GPNode* cloneImpl() const override;
    std::string toStringImpl() const override;
public:
    PlayerScoreDeltaNode();
};

class LibertiesDeltaNode: public GPTerminalNode {
    bool mine;
    int getImpl(const Context&) const override;
    GPNode* cloneImpl() const override;
    std::string toStringImpl() const override;
public:
    LibertiesDeltaNode(bool mine);
};

class MaxClusterDeltaNode: public GPTerminalNode {
    int getImpl(const Context&) const override;
    GPNode* cloneImpl() const override;
    std::string toStringImpl() const override;
public:
    MaxClusterDeltaNode();
};

class CanBeCapturedNode: public GPTerminalNode {
    bool inclDiag;
    int getImpl(const Context&) const override;
    GPNode* cloneImpl() const override;
    std::string toStringImpl() const override;
public:
    CanBeCapturedNode();
};

class CanCaptureNode: public GPTerminalNode {
    bool inclDiag;
    int getImpl(const Context&) const override;
    GPNode* cloneImpl() const override;
    std::string toStringImpl() const override;
public:
    CanCaptureNode();
};

class AdjacentStonesNode: public GPTerminalNode {
    bool mine, inclDiag;
    int getImpl(const Context&) const override;
    GPNode* cloneImpl() const override;
    std::string toStringImpl() const override;
public:
    AdjacentStonesNode(bool mine, bool inclDiag);
};

#endif
