#include <vector>
#include "gtest/gtest.h"
#include "gpnode.h"
#include "gptree.h"
#include "gpnodes_experimental.h"
#include "board.h"

using namespace std;

class MyOperatorNode: public GPOperatorNode {
    int getImpl(const Board&) const override { return 1; }
    GPNode* cloneImpl() const { return nullptr; };
public:
    MyOperatorNode(GPNode* a, GPNode* b)
        : GPOperatorNode(a, b) { }
};

class MyTerminalNode: public GPTerminalNode {
    int getImpl(const Board& ) const override { return 1; }
    GPNode* cloneImpl() const { return new MyTerminalNode(); };
public:
    MyTerminalNode() {}
};

TEST(GPNodeTest, OperatorNodeReturnsChildren) {
    auto c1 = ConstNode(1), c2 = ConstNode(2);
    auto children = MyOperatorNode(&c1, &c2).children();
    ASSERT_EQ(2, children.size());
    ASSERT_EQ(&c1, children[0]);
    ASSERT_EQ(&c2, children[1]);
}

TEST(GPNodeTest, TerminalNodeHasNoChildren) {
    ASSERT_EQ(0, MyTerminalNode().children().size());
}

TEST(GPNodeTest, NodeValueHasAdjustableScale) {
    Board b(9);
    MyTerminalNode node;
    ASSERT_EQ(1, node.get(b));
    node.setScale(-2);
    ASSERT_EQ(-2, node.get(b));
    node.setScale(0);
    ASSERT_EQ(0, node.get(b));
}

TEST(GPNodeTest, ScaleIsCarriedToClonedNode) {
    Board b(9);
    MyTerminalNode node;
    node.setScale(-2);
    GPNode* clone = node.clone();
    ASSERT_EQ(-2, node.get(b));
    ASSERT_EQ(-2, clone->get(b));
    cleanupTree(clone);
}
