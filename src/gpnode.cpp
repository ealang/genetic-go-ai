#include <stdexcept>
#include "gpnode.h"

using namespace std;

/* GPNode */

GPNode::GPNode(): scale(1) { }

GPNode::~GPNode() { }

int GPNode::get(const Board& context) const {
    return getImpl(context) * scale;
}

void GPNode::setScale(int val) {
    scale = val;
}

GPNode* GPNode::clone() const {
    GPNode* copy = cloneImpl();
    copy->setScale(scale);
    return copy;
}

/* Operator Node */

GPOperatorNode::GPOperatorNode(GPNode* a, GPNode* b)
    : nodes(vector<GPNode*>{a, b}) { }

GPOperatorNode::GPOperatorNode(GPNode* a, GPNode* b, GPNode* c, GPNode* d)
    : nodes(vector<GPNode*>{a, b, c, d}) { }

std::vector<GPNode*> GPOperatorNode::children() const {
    return nodes;
}

GPNode* GPOperatorNode::replaceChild(int num, GPNode* other) {
    GPNode* old = nodes[num];
    nodes[num] = other;
    return old;
}

std::string GPOperatorNode::toString() const {
    return "GPOperatorNode()";
}

/* Terminal Node */

std::vector<GPNode*> GPTerminalNode::children() const {
    return vector<GPNode*>();
}

GPNode* GPTerminalNode::replaceChild(int, GPNode*) {
    throw runtime_error("Terminal has no children to replace");
    return nullptr;
}

std::string GPTerminalNode::toString() const {
    return string("GPTerminalNode()");
}
