#ifndef GPNODES_EXPERIMENTAL
#define GPNODES_EXPERIMENTAL

#include <string>
#include "gpnode.h"
#include "color.h"

GPNode* createRandomTree(int depth);


class IntIfNode: public GPOperatorNode<INT, BOOL, INT, INT> {
    int getImpl(const Context&) const override;
    GPNode* cloneImpl() const override;
    std::string toStringImpl() const override;
public:
    IntIfNode(GPNode* i, GPNode* t, GPNode* e);
};

class IntEqualsNode: public GPOperatorNode<BOOL, INT, INT> {
    int getImpl(const Context&) const override;
    GPNode* cloneImpl() const override;
    std::string toStringImpl() const override;
public:
    IntEqualsNode(GPNode* l, GPNode* r);
};

class BoolEqualsNode: public GPOperatorNode<BOOL, BOOL, BOOL> {
    int getImpl(const Context&) const override;
    GPNode* cloneImpl() const override;
    std::string toStringImpl() const override;
public:
    BoolEqualsNode(GPNode* l, GPNode* r);
};

class AndNode: public GPOperatorNode<BOOL, BOOL, BOOL> {
    int getImpl(const Context&) const override;
    GPNode* cloneImpl() const override;
    std::string toStringImpl() const override;
public:
    AndNode(GPNode* l, GPNode* r);
};

class OrNode: public GPOperatorNode<BOOL, BOOL, BOOL> {
    int getImpl(const Context&) const override;
    GPNode* cloneImpl() const override;
    std::string toStringImpl() const override;
public:
    OrNode(GPNode* l, GPNode* r);
};

class NORNode: public GPOperatorNode<BOOL, BOOL, BOOL> {
    int getImpl(const Context&) const override;
    GPNode* cloneImpl() const override;
    std::string toStringImpl() const override;
public:
    NORNode(GPNode* l, GPNode* r);
};

class LessThanNode: public GPOperatorNode<BOOL, INT, INT> {
    int getImpl(const Context&) const override;
    GPNode* cloneImpl() const override;
    std::string toStringImpl() const override;
public:
    LessThanNode(GPNode* l, GPNode* r);
};

class PlusNode: public GPOperatorNode<INT, INT, INT> {
    int getImpl(const Context&) const override;
    GPNode* cloneImpl() const override;
    std::string toStringImpl() const override;
public:
    PlusNode(GPNode* l, GPNode* r);
};

class MultiplyNode: public GPOperatorNode<INT, INT, INT> {
    int getImpl(const Context&) const override;
    GPNode* cloneImpl() const override;
    std::string toStringImpl() const override;
public:
    MultiplyNode(GPNode* l, GPNode* r);
};

class BoolConstNode: public GPTerminalNode<BOOL> {
    const bool v;
    int getImpl(const Context&) const override;
    GPNode* cloneImpl() const override;
    std::string toStringImpl() const override;
public:
    BoolConstNode(bool v);
};

class IntConstNode: public GPTerminalNode<INT> {
    const int v;
    int getImpl(const Context&) const override;
    GPNode* cloneImpl() const override;
    std::string toStringImpl() const override;
public:
    IntConstNode(int v);
};

class RandomIntNode: public GPTerminalNode<INT> {
    const int min, max;
    int getImpl(const Context&) const override;
    GPNode* cloneImpl() const override;
    std::string toStringImpl() const override;
public:
    RandomIntNode(int min, int max);
};

class NetworkStrengthDeltaNode: public GPTerminalNode<INT> {
    int getImpl(const Context&) const override;
    GPNode* cloneImpl() const override;
    std::string toStringImpl() const override;
};

class NetworkStrengthNode: public GPTerminalNode<INT> {
    int getImpl(const Context&) const override;
    GPNode* cloneImpl() const override;
    std::string toStringImpl() const override;
};

class PlayerScoreDeltaNode: public GPTerminalNode<INT> {
    int getImpl(const Context&) const override;
    GPNode* cloneImpl() const override;
    std::string toStringImpl() const override;
};

class PlayerScoreNode: public GPTerminalNode<INT> {
    int getImpl(const Context&) const override;
    GPNode* cloneImpl() const override;
    std::string toStringImpl() const override;
};

class LibertiesDeltaNode: public GPTerminalNode<INT> {
    bool mine;
    int getImpl(const Context&) const override;
    GPNode* cloneImpl() const override;
    std::string toStringImpl() const override;
public:
    LibertiesDeltaNode(bool mine);
};

class LibertiesNode: public GPTerminalNode<INT> {
    bool mine;
    int getImpl(const Context&) const override;
    GPNode* cloneImpl() const override;
    std::string toStringImpl() const override;
public:
    LibertiesNode(bool mine);
};

class MaxClusterDeltaNode: public GPTerminalNode<INT> {
    int getImpl(const Context&) const override;
    GPNode* cloneImpl() const override;
    std::string toStringImpl() const override;
};

class CanBeCapturedNode: public GPTerminalNode<BOOL> {
    bool inclDiag;
    int getImpl(const Context&) const override;
    GPNode* cloneImpl() const override;
    std::string toStringImpl() const override;
};

class CanCaptureNode: public GPTerminalNode<BOOL> {
    bool inclDiag;
    int getImpl(const Context&) const override;
    GPNode* cloneImpl() const override;
    std::string toStringImpl() const override;
};

class IsWinningNode: public GPTerminalNode<BOOL> {
    bool inclDiag;
    int getImpl(const Context&) const override;
    GPNode* cloneImpl() const override;
    std::string toStringImpl() const override;
};

class AdjacentStonesNode: public GPTerminalNode<INT> {
    bool mine, inclDiag;
    int getImpl(const Context&) const override;
    GPNode* cloneImpl() const override;
    std::string toStringImpl() const override;
public:
    AdjacentStonesNode(bool mine, bool inclDiag);
};

class StoneSpacingDeltaNode: public GPTerminalNode<INT> {
    int getImpl(const Context&) const override;
    GPNode* cloneImpl() const override;
    std::string toStringImpl() const override;
};

#endif
