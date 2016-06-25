#ifndef GPNODE_H
#define GPNODE_H

#include <vector>
#include <string>

class Context;
class GPNode;

enum GPType { INT, BOOL };

template <GPType T> int compute(int val);
template <> int compute<BOOL>(int val);
template <> int compute<INT>(int val);

// Base node type for a genetic program tree
class GPNode {
    virtual GPNode* cloneImpl() const = 0;
    virtual std::string toStringImpl() const = 0;
protected:
    int scale;
    virtual int getImpl(const Context& context) const = 0;
public:
    GPNode();
    virtual ~GPNode();

    virtual int get(const Context&) const = 0;
    void setScale(int);
    std::string toString() const;

    GPNode* clone() const;

    virtual int getNumInputs() const = 0;
    virtual GPType getOutputType() const = 0;
    virtual GPNode* getInput(int num) const = 0;
    virtual GPNode* replaceInput(int num, GPNode* other) = 0;
};

// A node with inputs
template <GPType OutT, GPType... InputT>
class GPOperatorNode: public GPNode {
    const std::vector<GPType> inputTypes;
protected:
    std::vector<GPNode*> inputs;
public:
    GPOperatorNode(std::initializer_list<GPNode*> in)
    : inputTypes(std::vector<GPType>{InputT...}),
      inputs(std::vector<GPNode*>(in))
    {
        if (inputs.size() != inputTypes.size()) {
            throw std::runtime_error("Incorrect num arguments");
        }
        for (unsigned int i = 0; i < inputs.size(); i++) {
            if (inputs[i]->getOutputType() != inputTypes[i]) {
                throw std::runtime_error("Type mismatch");
            }
        }
    }

    int get(const Context& context) const override {
        return compute<OutT>(getImpl(context) * scale);
    }

    int getNumInputs() const override {
        return inputs.size();
    }

    GPType getOutputType() const override {
        return OutT;
    }

    GPNode* getInput(int num) const override {
        return inputs[num];
    }

    GPNode* replaceInput(int num, GPNode* other) override {
        if (inputTypes[num] != other->getOutputType()) {
            throw std::runtime_error("Tried to replace with incompatible type");
        }
        GPNode* old = inputs[num];
        inputs[num] = other;
        return old;
    }
};

// A node with no inputs
template <GPType OutT>
class GPTerminalNode: public GPNode {
public:
    int getNumInputs() const override {
        return 0;
    }

    int get(const Context& context) const override {
        return compute<OutT>(getImpl(context) * scale);
    }

    GPType getOutputType() const override {
        return OutT;
    }

    GPNode* getInput(int) const {
        throw std::runtime_error("Terminal has no inputs");
        return nullptr;
    }

    GPNode* replaceInput(int, GPNode*) override {
        throw std::runtime_error("Terminal has no inputs");
        return nullptr;
    }
};

#endif
