#include "aimatch.h"
#include "generate_ai.h"
#include "gpnode.h"
#include "gptree.h"

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

GPNode* generateAI(int boardSize, int maxTurnsPerGame, const TrainingOptions& options, std::function<void(const TrainingData&)> logger) {
    auto pop = createInitialPopulation(options);

    for (int generation = 0; generation < options.numGenerations; generation++) {
        unordered_map<int, int> scores;
        for (int ai = 0; ai < options.populationSize; ai++) {
            int benchmarkScore = 0, aiScore = 0;
            for (int game = 0; game < options.gamesPerEvaluation; game++) {
                auto result = playAIMatch(*options.benchmarkAI, *pop[ai], boardSize, maxTurnsPerGame);
                benchmarkScore += result.blackScore;
                aiScore += result.whiteScore;
            }
            scores.insert(pair<int, int>(ai, aiScore));
            logger(TrainingData{generation, ai, pop[ai], aiScore, benchmarkScore});
        }
        vector<const GPNode*> nextGen = options.evolveNextGeneration(pop, scores);
        cleanupPopulation(pop);
        pop = nextGen;
    }

    GPNode* winner = pop[0]->clone();
    cleanupPopulation(pop);
    return winner;
}
