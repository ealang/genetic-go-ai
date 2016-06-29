#include <unordered_map>
#include "generate_ai.h"
#include "gptree.h"
#include "leaderboard.h"

using namespace std;

void cleanupPopulation(const vector<const GPNode*>& pop) {
    for (auto ai: pop) {
        cleanupTree(ai);
    }
}

vector<const GPNode*> createInitialPopulation(const TrainingOptions& options) {
    vector<const GPNode*> pop;
    for (int i = 0; i < options.populationSize; i++) {
        pop.push_back(options.createNewAI());
    }
    return pop;
}

GPNode* generateAI(int boardSize, const TrainingOptions& options, std::function<void(const TrainingData&)> logger) {
    LeaderBoard leader;

    auto pop = createInitialPopulation(options);
    for (int generation = 0; generation < options.numGenerations; generation++) {
        auto scores = options.scoreFunc(boardSize, pop);
        for (unsigned int i = 0; i < pop.size(); i++) {
            logger(TrainingData{generation, (int)i, pop[i], scores[i]});
        }
        auto nextGen = options.evolveFunc(pop, scores);

        leader.update(pop, scores);
        cleanupPopulation(pop);
        pop = nextGen;
    }
    cleanupPopulation(pop);
    return leader.leader;
}
