#include <algorithm>
#include <cassert>
#include "scoring_algs.h"
#include "aimatch.h"
#include "gpnode.h"
#include "gptree.h"

using namespace std;

BenchmarkScore::BenchmarkScore(const GPNode* benchmark): benchmark(benchmark) {}

unordered_map<int, float> BenchmarkScore::operator()(int boardSize, const vector<const GPNode*>& pop) {

    for (unsigned int ai = 0; ai < pop.size(); ai++) {
        queue.submit([ai, boardSize, &pop, this](){
            int maxTurnsPerGame = int(boardSize * boardSize * 0.8);
            int totalScore = 0;
            for (int game = 0; game < gamesPerEvaluation; game++) {
                auto result = playAIMatch(*benchmark, *pop[ai], (game % 2) ? BLACK : WHITE, boardSize, maxTurnsPerGame);
                totalScore += result.whiteScore - result.blackScore;
            }
            return Result{(int)ai, (float)totalScore / gamesPerEvaluation};
        });
    }

    unordered_map<int, float> scores;
    for (unsigned int i = 0; i < pop.size(); i++) {
        Result result = queue.get();
        scores.emplace(result.ai, result.score);
    }
    return scores;
}

std::unordered_map<int, float> CompetitiveScore::operator()(int boardSize, const std::vector<const GPNode*>& pop) {
    for (unsigned int ai = 0; ai < pop.size(); ai++) {
        queue.submit([ai, boardSize, &pop, this](){
            int maxTurnsPerGame = int(boardSize * boardSize * 0.8);

            int totalScore = 0;
            for (int game = 0; game < gamesPerEvaluation; game++) {
                auto result = playAIMatch(*pop[rand() % pop.size()], *pop[ai],
                                          (game % 2) ? BLACK : WHITE,
                                          boardSize, maxTurnsPerGame);
                totalScore += result.whiteScore - result.blackScore;
            }
            return (float)totalScore / gamesPerEvaluation;
        });
    }

    unordered_map<int, float> scores;
    for (unsigned int ai = 0; ai < pop.size(); ai++) {
        scores.emplace(ai, queue.get());
    }
    return scores;
}
