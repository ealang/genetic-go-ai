#include <cstdlib>
#include "gptree.h"
#include "gpnodes_experimental.h"

int max(int a, int b) {
    return (a > b) ? a : b;
}

void cleanupTree(GPNode* tree) {
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

const GPNode* treeGetNodeNum_(const GPNode* tree, const int num, int& i);
const GPNode* treeGetNodeNum(const GPNode* tree, int num) {
    int i = 0;
    return treeGetNodeNum_(tree, num, i);
}
const GPNode* treeGetNodeNum_(const GPNode* tree, const int num, int& i) {
    if (i == num) {
        return tree;
    } else {
        for (auto child: tree->children()) {
            const GPNode* searchNode = treeGetNodeNum_(child, num, ++i);
            if (searchNode != nullptr) {
                return searchNode;
            }
        }
        return nullptr;
    }
}

// substitute tree2 into tree1 at num, returning a new tree
GPNode* treeReplaceAt_(GPNode* tree1, const int num, GPNode* tree2, int& i, GPNode* parent, int parentIndex);
GPNode* treeReplaceAt(const GPNode* tree1, int num, const GPNode* tree2) {
    if (num == 0) {
        return tree2->clone();
    } else {
        int i = 0;
        GPNode* newTree = tree1->clone();
        cleanupTree(treeReplaceAt_(newTree, num, tree2->clone(), i, nullptr, -1));
        return newTree;
    }
}
GPNode* treeReplaceAt_(GPNode* tree1, const int num, GPNode* tree2, int& i, GPNode* parent, int parentIndex) {
    if (i == num) {
        return parent->replaceChild(parentIndex, tree2);
    } else {
        int childI = 0;
        for (auto child: tree1->children()) {
            GPNode* replaced = treeReplaceAt_(child, num, tree2, ++i, tree1, childI);
            if (replaced != nullptr) {
                return replaced;
            }
            ++childI;
        }
        return nullptr;
    }
}

GPNode* mutateCombineTrees(const GPNode* tree1, const GPNode* tree2) {
    if (rand() % 2 == 0) {
        std::swap(tree1, tree2);
    }
    int count1 = treeNodeCount(tree1),
        count2 = treeNodeCount(tree2);
    const GPNode* subtree2 = treeGetNodeNum(tree2, rand() % count2);

    return treeReplaceAt(tree1, rand() % count1, subtree2);
}
