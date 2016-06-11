#include <vector>
#include "gtest/gtest.h"
#include "gpnode.h"
#include "gptree.h"
#include "gpnodes_experimental.h"
#include "board.h"

using namespace std;

class MyOperatorNode: public GPOperatorNode {
    int getUnscaled(const Board&) const override { return 1; }
public:
    MyOperatorNode(GPNode* a, GPNode* b)
        : GPOperatorNode(a, b) { }
    GPNode* clone() const { return nullptr; };
};

class MyTerminalNode: public GPTerminalNode {
    int getUnscaled(const Board& ) const override { return 1; }
public:
    MyTerminalNode() {}
    GPNode* clone() const { return nullptr; };
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
