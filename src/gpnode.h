#ifndef GPNODE_H
#define GPNODE_H

#include <vector>
#include <string>

class Context;
class GPNode;

class GPNode {
    int scale;
    virtual int getImpl(const Context& context) const = 0;
    virtual GPNode* cloneImpl() const = 0;
    virtual std::string toStringImpl() const = 0;
public:
    GPNode();
    int get(const Context&) const;
    void setScale(int);
    std::string toString() const;

    GPNode* clone() const;
    virtual std::vector<GPNode*> children() const = 0;
    virtual GPNode* replaceChild(int num, GPNode* other) = 0;
    virtual ~GPNode();
};

// Node with children
class GPOperatorNode: public GPNode {
protected:
    std::vector<GPNode*> nodes;
public:
    GPOperatorNode(GPNode* a, GPNode* b);
    GPOperatorNode(GPNode* a, GPNode* b, GPNode* c, GPNode* d);
    std::vector<GPNode*> children() const override;
    GPNode* replaceChild(int num, GPNode* other) override;
};

// Node with no children
class GPTerminalNode: public GPNode {
public:
    std::vector<GPNode*> children() const override;
    GPNode* replaceChild(int num, GPNode* other) override;
};

#endif
