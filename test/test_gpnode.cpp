#include <stdexcept>
#include <vector>
#include "gtest/gtest.h"
#include "board.h"
#include "gpnode.h"
#include "gpnode_context.h"
#include "gpnodes_experimental.h"
#include "gptree.h"

using namespace std;

class MyOperatorNode: public GPOperatorNode<INT, INT, INT> {
    int getImpl(const Context&) const override { return 1; }
    GPNode* cloneImpl() const { return nullptr; };
    string toStringImpl() const { return ""; };
public:
    MyOperatorNode(GPNode* a, GPNode* b)
        : GPOperatorNode({a, b}) { }
};

class MyTerminalNode: public GPTerminalNode<INT> {
    int getImpl(const Context&) const override { return 1; }
    GPNode* cloneImpl() const { return new MyTerminalNode(); };
    string toStringImpl() const { return ""; };
};

class GPNodeTest: public ::testing::Test {
public:
    Board board;
    Context context;
    GPNodeTest(): board(5), context{0, 0, BLACK, board, board} {}
};

TEST_F(GPNodeTest, OperatorNodeReturnsInputs) {
    auto c1 = IntConstNode(1), c2 = IntConstNode(2);
    auto node = MyOperatorNode(&c1, &c2);
    ASSERT_EQ(2, node.getNumInputs());
    ASSERT_EQ(&c1, node.getInput(0));
    ASSERT_EQ(&c2, node.getInput(1));
}

TEST_F(GPNodeTest, TerminalNodeHasNoInputs) {
    ASSERT_EQ(0, MyTerminalNode().getNumInputs());
}

TEST_F(GPNodeTest, NodeReportsDataType) {
    ASSERT_EQ(BOOL, BoolConstNode(true).getOutputType());
    ASSERT_EQ(INT, IntConstNode(1).getOutputType());
}

TEST_F(GPNodeTest, DisallowsIncompatibleTypes) {
    IntConstNode i(1);
    BoolConstNode b(true);
    ASSERT_THROW(PlusNode(&i, &b), runtime_error);
    ASSERT_THROW(OrNode(&i, &b), runtime_error);
}

TEST_F(GPNodeTest, NodeValueHasAdjustableScale) {
    Board b(9);
    MyTerminalNode node;
    ASSERT_EQ(1, node.get(context));
    node.setScale(-2);
    ASSERT_EQ(-2, node.get(context));
    node.setScale(0);
    ASSERT_EQ(0, node.get(context));
}

TEST_F(GPNodeTest, ScaleIsCarriedToClonedNode) {
    Board b(9);
    MyTerminalNode node;
    node.setScale(-2);
    GPNode* clone = node.clone();
    ASSERT_EQ(-2, node.get(context));
    ASSERT_EQ(-2, clone->get(context));
    cleanupTree(clone);
}
