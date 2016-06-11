#ifndef GPTREE_H
#define GPTREE_H

#include <utility>

class GPNode;

void cleanupTree(const GPNode* n);
int treeDepth(const GPNode* tree);
int treeNodeCount(const GPNode* tree);

// return node number num in tree
const GPNode* treeGetNodeNum(const GPNode* tree, int num);

// substitute tree2 into tree1 at num, returning a new tree
GPNode* treeReplaceAt(const GPNode* tree1, int num, const GPNode* tree2);

// swap random subtrees into two given trees, returning new trees
std::pair<GPNode*, GPNode*> swapRandomSubtrees(const GPNode* tree1, const GPNode* tree2);

// swap a random subtrees from tree1 into tree2, returning a new tree
GPNode* swapRandomSubtree(const GPNode* tree1, const GPNode* tree2);

#endif
