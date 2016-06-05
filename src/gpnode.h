#ifndef GPNODE_H
#define GPNODE_H

#include <vector>
#include <string>

class Board;
class GPNode;

class GPNode {
public:
    virtual int get(const Board& context) const = 0;
    virtual std::vector<GPNode*> children() const = 0;
    virtual GPNode* replaceChild(int num, GPNode* other) = 0;
    virtual GPNode* clone() const = 0;
    virtual std::string toString() const = 0;
    virtual ~GPNode();
};

// Node with children
class GPOperatorNode: public GPNode {
protected:
    std::vector<GPNode*> nodes;
public:
    GPOperatorNode(GPNode* a, GPNode* b);
    std::vector<GPNode*> children() const override;
    GPNode* replaceChild(int num, GPNode* other) override;
    std::string toString() const override;
};

// Node with no children
class GPTerminalNode: public GPNode {
protected:
public:
    std::vector<GPNode*> children() const override;
    GPNode* replaceChild(int num, GPNode* other) override;
    std::string toString() const override;
};

#endif