#include <vector>
#include "gtest/gtest.h"
#include "gpnode.h"
#include "gpnode_context.h"
#include "gptree.h"
#include "gpnodes_experimental.h"
#include "board.h"

using namespace std;

class MyOperatorNode: public GPOperatorNode {
    int getImpl(const Context&) const override { return 1; }
    GPNode* cloneImpl() const { return nullptr; };
    string toStringImpl() const { return ""; };
public:
    MyOperatorNode(GPNode* a, GPNode* b)
        : GPOperatorNode(a, b) { }
};

class MyTerminalNode: public GPTerminalNode {
    int getImpl(const Context&) const override { return 1; }
    GPNode* cloneImpl() const { return new MyTerminalNode(); };
    string toStringImpl() const { return ""; };
public:
    MyTerminalNode() {}
};

class GPNodeTest: public ::testing::Test {
public:
    Board board;
    Context context;
    GPNodeTest(): board(5), context{0, 0, BLACK, board, board} {}
};

TEST_F(GPNodeTest, OperatorNodeReturnsChildren) {
    auto c1 = ConstNode(1), c2 = ConstNode(2);
    auto children = MyOperatorNode(&c1, &c2).children();
    ASSERT_EQ(2, children.size());
    ASSERT_EQ(&c1, children[0]);
    ASSERT_EQ(&c2, children[1]);
}

TEST_F(GPNodeTest, TerminalNodeHasNoChildren) {
    ASSERT_EQ(0, MyTerminalNode().children().size());
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
