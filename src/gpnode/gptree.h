#ifndef GPTREE_H
#define GPTREE_H

#include "gpnode.h"

int treeDepth(const GPNode* tree);

// count nodes in a tree
int treeNodeCount(const GPNode* tree);
int treeNodeCount(const GPNode* tree, GPType outType);

// find ith node in a tree
const GPNode* treeGetNodeNum(const GPNode* tree, int num);
GPNode* treeGetNodeNum(GPNode* tree, int num);

// substitute tree2 into tree1 at num, returning a new tree
GPNode* treeReplaceAt(const GPNode* tree1, int num, const GPNode* tree2);

// swap random subtrees into two given trees, returning new trees
std::pair<GPNode*, GPNode*> swapRandomSubtrees(const GPNode* tree1, const GPNode* tree2);

// swap a random subtrees from tree1 into tree2, returning a new tree
GPNode* swapRandomSubtree(const GPNode* tree1, const GPNode* tree2);

// delete all nodes in tree
void cleanupTree(const GPNode* n);

#endif
