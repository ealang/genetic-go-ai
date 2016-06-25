#include <stdexcept>
#include <sstream>
#include "gpnode.h"

using namespace std;

template <>
int compute<BOOL>(int val) {
    return val > 0 ? 1 : 0;
}

template <>
int compute<INT>(int val) {
    return val;
}

/* GPNode */

GPNode::GPNode(): scale(1) { }

GPNode::~GPNode() { }

void GPNode::setScale(int val) {
    scale = val;
}

GPNode* GPNode::clone() const {
    GPNode* copy = cloneImpl();
    copy->setScale(scale);
    return copy;
}

std::string GPNode::toString() const {
    if (scale != 1) {
        std::stringstream ss;
        ss << scale << "*" << toStringImpl();
        return ss.str();
    } else {
        return toStringImpl();
    }
}
