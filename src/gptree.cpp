#include <cstdlib>
#include <functional>
#include "gptree.h"
#include "gpnodes_experimental.h"

using namespace std;

int max(int a, int b) {
    return (a > b) ? a : b;
}

void cleanupTree(const GPNode* tree) {
    for (auto child: tree->children()) {
        cleanupTree(child);
    }
    delete tree;
}

int treeDepth(const GPNode* tree) {
    int maxDepth = 0;
    for (auto child: tree->children()) {
        maxDepth = max(maxDepth, treeDepth(child));
    }
    return maxDepth + 1;
}

int treeNodeCount(const GPNode* tree) {
    int count = 1;
    for (auto child: tree->children()) {
        count += treeNodeCount(child);
    }
    return count;
}

const GPNode* treeGetNodeNum(const GPNode* tree, int num) {
    int i = 0;
    function<const GPNode*(const GPNode*)> r = [&i, num, &r](const GPNode* node) {
        if (i++ == num) {
            return node;
        } else {
            for (auto child: node->children()) {
                const GPNode* searchNode = r(child);
                if (searchNode != nullptr) {
                    return searchNode;
                }
            }
            return (const GPNode*)nullptr;
        }
    };
    return r(tree);
}

GPNode* treeGetNodeNum(GPNode* tree, int num) {
    return const_cast<GPNode*>(treeGetNodeNum((const GPNode*)tree, num));
}

GPNode* treeReplaceAt(const GPNode* tree1, int num, const GPNode* tree2) {
    int curNode = 0;
    function<GPNode*(GPNode*, GPNode*, int)> r = [&](GPNode* node, GPNode* parent, int parentIndex) {
        if (curNode == num) {
            return parent->replaceChild(parentIndex, tree2->clone());
        } else {
            int childI = 0;
            for (auto child: node->children()) {
                ++curNode;
                GPNode* replaced = r(child, node, childI);
                if (replaced != nullptr) {
                    return replaced;
                }
                ++childI;
            }
            return (GPNode*)nullptr;
        }
    };

    if (num == 0) {
        return tree2->clone();
    } else {
        GPNode* newTree = tree1->clone();
        GPNode* discardedTree = r(newTree, nullptr, -1);
        cleanupTree(discardedTree);
        return newTree;
    }
}

pair<GPNode*, GPNode*> swapRandomSubtrees(const GPNode* tree1, const GPNode* tree2) {
    int count1 = treeNodeCount(tree1),
        count2 = treeNodeCount(tree2),
        sub1 = rand() % count1,
        sub2 = rand() % count2;
    const GPNode* subtree1 = treeGetNodeNum(tree1, sub1),
                * subtree2 = treeGetNodeNum(tree2, sub2);

    return make_pair(treeReplaceAt(tree1, sub1, subtree2),
                     treeReplaceAt(tree2, sub2, subtree1));
}

GPNode* swapRandomSubtree(const GPNode* tree1, const GPNode* tree2) {
    int count1 = treeNodeCount(tree1),
        count2 = treeNodeCount(tree2);
    const GPNode* subtree1 = treeGetNodeNum(tree1, rand() % count1);
    return treeReplaceAt(tree2, rand() % count2, subtree1);
}
