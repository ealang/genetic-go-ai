#include <iostream>
#include <map>
#include "aimatch.h"
#include "generate_ai.h"
#include "gpnode.h"
#include "gpnodes_experimental.h"
#include "gptree.h"

using namespace std;

GPNode* createRandomAI() {
    return createRandomTree(1 + rand() % 5);
}

void cleanupAIArray(GPNode** ais, int numAI) {
    for (int i = 0; i < numAI; i++) {
        cleanupTree(ais[i]);
    }
    delete[] ais;
}

int randomExcluding(int max, int exclude) {
    int n;
    do {
        n = rand() % max;
    } while (n != exclude);
    return n;
}

GPNode** initialzeAI(int numAI) {
    GPNode** ais = new GPNode*[numAI];
    for (auto i = 0; i < numAI; i++) {
        ais[i] = createRandomAI();
    }
    return ais;
}

GPNode** updateAI(GPNode** origAIs, multimap<int, int> scores) {
    const int numAI = scores.size(), numUnmodified = (int)(numAI * 0.1), numKeep = (int)(numAI * 0.8);

    int survivaAiList[numKeep];
    auto it = scores.rbegin();
    for (auto i = 0; i < numKeep; i++) {
        int ai = it->second;
        survivaAiList[i] = ai;
        ++it;
    }

    GPNode **newAIs = new GPNode*[numAI];
    for (auto i = 0; i < numUnmodified; i++) {
        newAIs[i] = origAIs[survivaAiList[i]]->clone();
    }
    for (auto i = numUnmodified; i < numKeep; i++) {
        newAIs[i] = mutateCombineTrees(origAIs[survivaAiList[i]], 
                                       origAIs[survivaAiList[randomExcluding(numKeep, i)]]);
    }
    for (auto i = numKeep; i < numAI; i++) {
        newAIs[i] = createRandomAI();
    }
    return newAIs;
}

GPNode* generateAI(int boardSize, int numRounds, int numAI) {
    const int MAX_TURNS = boardSize * boardSize / 2;

    RandomNode benchmark = RandomNode(0, boardSize * boardSize);
    GPNode** ais = initialzeAI(numAI);

    for (auto round = 0; round < numRounds; round++) {
        multimap<int, int> scores;
        for (auto ai = 0; ai < numAI; ai++) {
            int randScore = 0, aiScore = 0;
            for (auto game = 0; game < 50; game++) {
                auto result = playAIMatch(benchmark, *ais[ai], boardSize, MAX_TURNS);
                randScore += result.blackScore;
                aiScore += result.whiteScore;
            }
            scores.insert(pair<int, int>(aiScore, ai));
            cout << "round=" << round 
                 << " ai=" << ai << " (size=" << treeNodeCount(ais[ai]) << " depth=" << treeDepth(ais[ai]) << ")"
                 << " ai score=" << aiScore
                 << " rand score=" << randScore << endl;
        }
        GPNode** newAIs = updateAI(ais, scores);
        cleanupAIArray(ais, numAI);
        ais = newAIs;
    }

    GPNode* best = ais[0]->clone();
    cleanupAIArray(ais, numAI);
    return best;
}

