#ifndef GPTREE_H
#define GPTREE_H

class GPNode;

void cleanupTree(GPNode* n);
int treeDepth(const GPNode* tree);
int treeNodeCount(const GPNode* tree);

// return node number num in tree
const GPNode* treeGetNodeNum(const GPNode* tree, int num);

// substitute tree2 into tree1 at num, returning a new tree
GPNode* treeReplaceAt(const GPNode* tree1, int num, const GPNode* tree2);

// randomly substitute a part of one tree into the other, returning a new tree
GPNode* mutateCombineTrees(const GPNode* tree1, const GPNode* tree2);

// replace subtree rooted at 'num' with something else, returning a new tree
GPNode* mutateRandomReplaceSubTree(const GPNode* tree, int num);

// replace node 'num' in 'tree' with 'node', returning a new tree
GPNode* mutateReplaceNode(const GPNode* tree, int num, const GPNode* node);

#endif
