#include <cassert>
#include "aimatch.h"
#include "generate_ai.h"
#include "gpnode.h"
#include "gptree.h"
#include "task_queue.h"

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

struct LeaderBoard {
    int score = 0;
    GPNode* leader = nullptr;

    void update(const vector<const GPNode*>& pop, const unordered_map<int, int>& scores) {
        auto best = max_element(scores.begin(), scores.end(), 
                        [](pair<int, int> a, pair<int, int> b) { return a.second < b.second; });

        int i = best->first,
            iscore = best->second;
        if (iscore > score || leader == nullptr) {
            score = iscore;
            if (leader) {
                cleanupTree(leader);
            }
            leader = pop[i]->clone();
        }
    };
};

GPNode* generateAI(int boardSize, int maxTurnsPerGame, const TrainingOptions& options, std::function<void(const TrainingData&)> logger) {
    struct Result {
        int ai, aiScore, benchmarkScore;
    };
    LeaderBoard leader;

    TaskQueue<Result> queue;
    auto pop = createInitialPopulation(options);
    for (int generation = 0; generation < options.numGenerations; generation++) {
        unordered_map<int, int> scores;
        for (int ai = 0; ai < options.populationSize; ai++) {
            queue.submit([ai, boardSize, maxTurnsPerGame, &pop, &options](){
                int benchmarkScore = 0, aiScore = 0;
                for (int game = 0; game < options.gamesPerEvaluation; game++) {
                    auto result = playAIMatch(*options.benchmarkAI, *pop[ai], boardSize, maxTurnsPerGame);
                    benchmarkScore += result.blackScore;
                    aiScore += result.whiteScore;
                }
                return Result{ai, aiScore, benchmarkScore};
            });
        }
        queue.forEach([&pop, &scores, generation, &logger](Result result) {
            scores.insert(pair<int, int>(result.ai, result.aiScore));
            logger(TrainingData{generation, result.ai, pop[result.ai], result.aiScore, result.benchmarkScore});
        });
        assert(scores.size() == pop.size());
        leader.update(pop, scores);

        vector<const GPNode*> nextGen = options.evolveNextGeneration(pop, scores);
        cleanupPopulation(pop);
        pop = nextGen;
    }
    cleanupPopulation(pop);
    return leader.leader;
}
