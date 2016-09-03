#include <utility>
#include "gtest/gtest.h"
#include "gpnode_context.h"
#include "gpnodes_experimental.h"
#include "gptree.h"
#include "board.h"

using namespace std;

extern pair<const GPNode*, int> treeGetNodeNum(const GPNode* tree, int num, GPType type);

class GPTreeTest: public ::testing::Test {
public:
    Board board;
    Context context;
    GPTreeTest(): board(9), context{0, 0, BLACK, board, board} {}
};

TEST_F(GPTreeTest, MeasuresCorrectDepthOfTerminalNode) {
    IntConstNode n = IntConstNode(1);
    ASSERT_EQ(1, treeDepth(&n));
}

TEST_F(GPTreeTest, MeasuresCorrectCountOfTerminalNode) {
    auto n = IntConstNode(1);
    ASSERT_EQ(1, treeNodeCount(&n));
}

TEST_F(GPTreeTest, MeasuresCorrectDepthOfTree) {
    auto c1 = IntConstNode(1);
    auto n2 = PlusNode(&c1, &c1);
    auto n1 = PlusNode(&n2, &c1);
    ASSERT_EQ(3, treeDepth(&n1));
}

TEST_F(GPTreeTest, MeasuresCorrectNodeCountInTree) {
    auto c1 = IntConstNode(1);
    auto n2 = PlusNode(&c1, &c1);
    auto n1 = PlusNode(&n2, &c1);
    ASSERT_EQ(5, treeNodeCount(&n1));
}

TEST_F(GPTreeTest, CanFindNodeByNumber) {
    auto c2 = IntConstNode(2), c3 = IntConstNode(3), c4 = IntConstNode(4);
    auto n1 = PlusNode(&c2, &c3);
    auto n0 = PlusNode(&n1, &c4);
    ASSERT_EQ(&n0, treeGetNodeNum(&n0, 0));
    ASSERT_EQ(&n1, treeGetNodeNum(&n0, 1));
    ASSERT_EQ(&c2, treeGetNodeNum(&n0, 2));
    ASSERT_EQ(&c3, treeGetNodeNum(&n0, 3));
    ASSERT_EQ(&c4, treeGetNodeNum(&n0, 4));
    ASSERT_EQ(nullptr, treeGetNodeNum(&n0, 5));
}

TEST_F(GPTreeTest, CanSubstituteTrees) {
    auto c2 = IntConstNode(2), c3 = IntConstNode(3), c4 = IntConstNode(4);
    auto n1 = PlusNode(&c2, &c3);
    auto n0 = PlusNode(&n1, &c4);
    auto sub = IntConstNode(10);
    int expectedSum[] = {10, 14, 17, 16, 15};
    for (int i = 0; i < 5; i++) {
        GPNode* newtree = treeReplaceAt(&n0, i, &sub);
        ASSERT_EQ(expectedSum[i], newtree->get(context)) << "replace at " << i;
        cleanupTree(newtree);
    }
}

TEST_F(GPTreeTest, CanRandomlySwapSubtreesWithNoNodeLoss) {
    IntConstNode c1(1), c2(2), c3(4), c4(8), c5(16), c6(32);
    PlusNode n1(&c1, &c2);
    PlusNode n2(&n1, &c3);
    PlusNode n3(&c4, &c5);
    PlusNode n4(&n3, &c6);

    ASSERT_EQ(1 + 2 + 4, n2.get(context));
    ASSERT_EQ(32 + 16 + 8, n4.get(context));

    auto trees = swapRandomSubtrees(&n2, &n4);

    ASSERT_NE(trees.first->get(context), n2.get(context));
    ASSERT_NE(trees.second->get(context), n4.get(context));
    ASSERT_EQ(63, trees.first->get(context) + trees.second->get(context));

    cleanupTree(trees.first);
    cleanupTree(trees.second);
}

TEST_F(GPTreeTest, ReturnsCloneIfSwapTreesIsIncompatible) {
    IntConstNode inode(3);
    BoolConstNode bnode(true);
    {
        auto trees = swapRandomSubtrees(&inode, &bnode);
        ASSERT_EQ(3, trees.first->get(context));
        ASSERT_EQ(1, trees.second->get(context));
        cleanupTree(trees.first);
        cleanupTree(trees.second);
    }
    {
        auto tree = swapRandomSubtree(&inode, &bnode);
        ASSERT_EQ(1, tree->get(context));
        cleanupTree(tree);
    }
}

TEST_F(GPTreeTest, SwapsCompatibleSubtrees) {
    class AddThings: public GPOperatorNode<INT, BOOL, INT> {
        int getImpl(const Context& c) const override {
            return inputs[0]->get(c) + inputs[1]->get(c);
        }
        GPNode* cloneImpl() const override { return new AddThings(inputs[0]->clone(), inputs[1]->clone()); }
        std::string toStringImpl() const override { return "AddThings"; }
    public:
        AddThings(GPNode* a, GPNode* b): GPOperatorNode({a, b}) {}
    };
    GPNode* tree1 = new AddThings(new BoolConstNode(false), new IntConstNode(2));
    GPNode* tree2 = new AddThings(new BoolConstNode(true), new IntConstNode(4));

    EXPECT_EQ(2, tree1->get(context));
    EXPECT_EQ(5, tree2->get(context));

    {
        auto trees = swapRandomSubtrees(tree1, tree2);
        EXPECT_NE(2, trees.first->get(context));
        EXPECT_NE(5, trees.second->get(context));
        cleanupTree(trees.first);
        cleanupTree(trees.second);
    }
    {
        auto tree = swapRandomSubtree(tree1, tree2);
        EXPECT_NE(5, tree->get(context));
        cleanupTree(tree);
    }
    cleanupTree(tree1);
    cleanupTree(tree2);
}

TEST_F(GPTreeTest, CanRetreiveNodeOfType) {
    IntConstNode n1(1), n2(2), n4(4);
    PlusNode np(&n1, &n2);
    BoolConstNode ntrue(true);
    BoolConstNode nfalse(false);
    OrNode nor(&ntrue, &nfalse);
    IntIfNode ni(&nor, &np, &n4);

    ASSERT_EQ(3, ni.get(context));
    EXPECT_EQ(5, treeNodeCount(&ni, INT));
    EXPECT_EQ(3, treeNodeCount(&ni, BOOL));

    GPNode* expBool[] = {&nor, &ntrue, &nfalse, nullptr};
    for (int i = 0; i < 4; i++) {
        EXPECT_EQ(expBool[i], treeGetNodeNum(&ni, i, BOOL).first) << i;
    }

    GPNode* expInt[] = {&ni, &np, &n1, &n2, &n4, nullptr};
    for (int i = 0; i < 6; i++) {
        EXPECT_EQ(expInt[i], treeGetNodeNum(&ni, i, INT).first) << i;
    }
}
