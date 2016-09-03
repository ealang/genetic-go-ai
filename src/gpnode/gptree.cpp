#include <algorithm>
#include <cstdlib>
#include <functional>
#include <utility>
#include "gptree.h"
#include "gpnodes_experimental.h"
#include "ts_rand.h"

using namespace std;

// walk nodes in the tree. callback: (node num, parent, childNum, node)
void iterateNodes(const GPNode* tree, function<bool(int, const GPNode*, int, const GPNode*)> callback) {
    int curNode = 0;
    function<void(const GPNode*, int, const GPNode*)> r = [&](const GPNode* parent, int childNum, const GPNode* cur) {
        if (!callback(curNode++, parent, childNum, cur)) {
            return;
        }
        for (int i = 0; i < cur->getNumInputs(); i++) {
            r(cur, i, cur->getInput(i));
        }
    };
    return r(nullptr, -1, tree);
}

// walk nodes in a mutable tree. callback: (node num, parent, childNum, node)
void iterateNodes(GPNode* tree, function<bool(int, GPNode*, int, GPNode*)> callback) {
    iterateNodes((const GPNode*)tree, [&callback](int i, const GPNode* parent, int childI, const GPNode* cur) {
        return callback(i, const_cast<GPNode*>(parent), childI, const_cast<GPNode*>(cur));
    });
}

// walk nodes in a tree. callback: (node num, node)
void iterateNodes(const GPNode* tree, function<bool(int, const GPNode*)> callback) {
    iterateNodes(tree, [&callback](int i, const GPNode*, int, const GPNode* cur) {
        return callback(i, cur);
    });
}

void cleanupTree(const GPNode* tree) {
    for (int i = 0; i < tree->getNumInputs(); i++) {
        cleanupTree(tree->getInput(i));
    }
    delete tree;
}

int treeDepth(const GPNode* tree) {
    int maxDepth = 0;
    for (int i = 0; i < tree->getNumInputs(); i++) {
        maxDepth = max(maxDepth, treeDepth(tree->getInput(i)));
    }
    return maxDepth + 1;
}

int treeNodeCount(const GPNode* tree) {
    int count = 1;
    for (int i = 0; i < tree->getNumInputs(); i++) {
        count += treeNodeCount(tree->getInput(i));
    }
    return count;
}

int treeNodeCount(const GPNode* tree, GPType outType) {
    int count = tree->getOutputType() == outType;
    for (int i = 0; i < tree->getNumInputs(); i++) {
        count += treeNodeCount(tree->getInput(i), outType);
    }
    return count;
}

const GPNode* treeGetNodeNum(const GPNode* tree, int num) {
    const GPNode* targetNode = nullptr;
    iterateNodes(tree, [&](int i, const GPNode* node) {
        if (i == num) {
            targetNode = node;
            return false;
        } else {
            return true;
        }
    });
    return targetNode;
}

GPNode* treeGetNodeNum(GPNode* tree, int num) {
    return const_cast<GPNode*>(treeGetNodeNum((const GPNode*)tree, num));
}

pair<const GPNode*, int> treeGetNodeNum(const GPNode* tree, int num, GPType type) {
    pair<const GPNode*, int> out = make_pair(nullptr, 0);
    int typeI = 0;
    iterateNodes(tree, [&](int i, const GPNode* node) {
        if (node->getOutputType() == type && typeI++ == num) {
            out.first = node;
            out.second = i;
            return false;
        } else {
            return true;
        }
    });
    return out;
}

GPNode* treeReplaceAt(const GPNode* tree1, int num, const GPNode* tree2) {
    if (num == 0) {
        return tree2->clone();
    } else {
        GPNode* newTree = tree1->clone();
        iterateNodes(newTree, [num, &tree2](int i, GPNode* parent, int childI, GPNode*) {
            if (i == num) {
                const GPNode* discarded = parent->replaceInput(childI, tree2->clone());
                cleanupTree(discarded);
                return false;
            } else {
                return true;
            }
        });
        return newTree;
    }
}

pair<GPNode*, GPNode*> swapRandomSubtrees(const GPNode* tree1, const GPNode* tree2) {
    int sub1 = ts_rand() % treeNodeCount(tree1);
    const GPNode *subTree1 = treeGetNodeNum(tree1, sub1);
    GPType type = subTree1->getOutputType();

    int compatibleCount = treeNodeCount(tree2, type);
    if (compatibleCount > 0) {
        auto p = treeGetNodeNum(tree2, ts_rand() % compatibleCount, type);
        const GPNode* subTree2 = p.first;
        int sub2 = p.second;

        return make_pair(treeReplaceAt(tree1, sub1, subTree2),
                         treeReplaceAt(tree2, sub2, subTree1));
    } else {
        return make_pair(tree1->clone(), tree2->clone());
    }
}

GPNode* swapRandomSubtree(const GPNode* tree1, const GPNode* tree2) {
    int sub1 = ts_rand() % treeNodeCount(tree1);
    const GPNode *subTree1 = treeGetNodeNum(tree1, sub1);
    GPType type = subTree1->getOutputType();

    int compatibleCount = treeNodeCount(tree2, type);
    if (compatibleCount > 0) {
        int sub2 = treeGetNodeNum(tree2, ts_rand() % compatibleCount, type).second;

        return treeReplaceAt(tree2, sub2, subTree1);
    } else {
        return tree2->clone();
    }
}
