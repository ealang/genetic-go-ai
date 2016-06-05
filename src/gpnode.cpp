#include <stdexcept>
#include "gpnode.h"

using namespace std;

/* GPNode */

GPNode::~GPNode() { }

/* Operator Node */

GPOperatorNode::GPOperatorNode(GPNode* a, GPNode* b)
    : nodes(vector<GPNode*>{a, b}) { }

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
