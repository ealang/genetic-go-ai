#include <algorithm>
#include <cassert>
#include <map>
#include "gpnode.h"
#include "gpnodes_experimental.h"
#include "gptree.h"
#include "evolution_algs.h"

using namespace std;

float rand01() {
    return rand() / (RAND_MAX + 1.0);
}

int randExcluding(int n, int exclude) {
    assert(n > 1);
    int v;
    do {
        v = rand() % n;
        if (v != exclude) {
            break;
        }
    } while (v == exclude);
    return v;
}

GPNode* createRandomAI() {
    static const int NEW_AI_MIN_DEPTH = 1,
                     NEW_AI_MAX_DEPTH = 5;
    return createRandomTree(NEW_AI_MIN_DEPTH + rand() % (NEW_AI_MAX_DEPTH - NEW_AI_MIN_DEPTH));
}

vector<int> populationDropN(const unordered_map<int, int>& scores, unsigned int numDrop) {
    assert(scores.size() >= numDrop);
    multimap<int, int> sort;
    for (auto pair: scores) {
        sort.insert(make_pair(pair.second, pair.first));
    }
    vector<int> result;
    auto it = sort.rbegin();
    for (unsigned int i = 0; i < scores.size() - numDrop; i++) {
        result.push_back(it->second);
        ++it;
    }
    return result;
}

// change the scale factor of a random node in the tree
GPNode* mutateAI(const GPNode* ai) {
    static const int MUTATION_SCALE_MIN = -5, MUTATION_SCALE_MAX = 5;
    int size = treeNodeCount(ai),
        n = rand() % size;
    GPNode* newAi = treeGetNodeNum(ai, n)->clone();
    int scale = MUTATION_SCALE_MIN + rand() % (MUTATION_SCALE_MAX - MUTATION_SCALE_MIN + 1);
    newAi->setScale(scale);
    return newAi;
}

// randomly mutate entire population
void randomMutation(vector<const GPNode*>& pop) {
    static const float MUTATE_CHANCE = 0.05;

    for (unsigned int i = 0; i < pop.size(); i++) {
        if (rand01() < MUTATE_CHANCE) {
            const GPNode *oldAi = pop[i],
                         *newAi = mutateAI(oldAi);
            pop[i] = newAi;
            cleanupTree(oldAi);    
        }
    }
}

// Use 2 way crossover with num offspring proportional to score
// Allow some to survive unmodified
vector<const GPNode*> evolvePopulationCrossover(const vector<const GPNode*>& oldPop, const unordered_map<int, int>& scores) {
    const float GENERATION_PERCENT_DROP = 0.05,
                POPULATION_SIZE = oldPop.size();

    const int numDropped = int(POPULATION_SIZE * GENERATION_PERCENT_DROP);
    vector<int> survivors = populationDropN(scores, numDropped);

    int totScore = accumulate(survivors.begin(), survivors.end(), 0,
                              [&scores](int sum, int ai){ return sum + scores.find(ai)->second; });

    vector<const GPNode*> newPop;
    for (auto ai: survivors) {
        if (newPop.size() >= oldPop.size()) {
            break;
        }
        int score = scores.find(ai)->second;
        int numOffspring = max((int)(((float)score / totScore) * survivors.size()), 1);
        if (numOffspring > 1) {
            newPop.push_back(oldPop[ai]->clone());
            --numOffspring;
        }
        for (int i = 0; i < numOffspring && newPop.size() < POPULATION_SIZE; i++) {
            newPop.push_back(swapRandomSubtree(oldPop[ai], 
                                               oldPop[randExcluding(POPULATION_SIZE, ai)]));
        }
    }
    while (newPop.size() < POPULATION_SIZE) {
        newPop.push_back(createRandomAI());
    }

    randomMutation(newPop);

    return newPop;
}

// Use 1 and 2 way crossover plus mutation
// Allow some to survive unmodified
vector<const GPNode*> evolvePopulationOperationsMix(const vector<const GPNode*>& oldPop, const unordered_map<int, int>& scores) {
    static const float MUTATE_CHANCE_1_WAY_CROSSOVER = 1.0,
                       GENERATION_PERCENT_DROP = 0.05;
    const unsigned int POPULATION_SIZE = oldPop.size(),
                       numDropped = int(POPULATION_SIZE * GENERATION_PERCENT_DROP);

    auto pop = [](vector<int>& vec) { int v = vec.back(); vec.pop_back(); return v; };

    vector<int> survivors = populationDropN(scores, numDropped);
    random_shuffle(survivors.begin(), survivors.end());

    vector<const GPNode*> newPop;
    while (survivors.size() >= 2) {
        int p1 = pop(survivors),
            p2 = pop(survivors);

        if (rand01() < MUTATE_CHANCE_1_WAY_CROSSOVER) {
            // 1 way crossover
            auto a = scores.find(p1)->second > scores.find(p2)->second ? p1 : p2,
                 b = a == p1 ? p2 : p1;
            newPop.push_back(oldPop[a]->clone());
            newPop.push_back(swapRandomSubtree(oldPop[a], oldPop[b]));
        } else {
            // 2 way crossover
            auto pair = swapRandomSubtrees(oldPop[p1],
                                           oldPop[p2]);
            newPop.push_back(pair.first);
            newPop.push_back(pair.second);
        }
    }

    while (survivors.size() > 0) {
        newPop.push_back(oldPop[pop(survivors)]->clone());
    }

    while (newPop.size() < POPULATION_SIZE) {
        newPop.push_back(createRandomAI());
    }

    randomMutation(newPop);

    return newPop;
}
