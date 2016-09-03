#include "scoring_algs.h"
#include "aimatch.h"
#include "gpnode.h"
#include "gptree.h"
#include "ts_rand.h"

using namespace std;

int maxTurnsForGame(int boardSize) {
    return int(boardSize * boardSize * 0.7);
}

BenchmarkScore::BenchmarkScore(int gamesPerEvaluation, const GPNode* benchmark): 
    gamesPerEvaluation(gamesPerEvaluation),
    benchmark(benchmark) {}

unordered_map<int, float> BenchmarkScore::operator()(int boardSize, const vector<const GPNode*>& pop) {
    for (unsigned int ai = 0; ai < pop.size(); ai++) {
        for (int game = 0; game < gamesPerEvaluation; game++) {
            queue.submit([ai, boardSize, game, &pop, this](){
                auto result = playAIMatch(*benchmark, *pop[ai],
                                          (game % 2) ? BLACK : WHITE,
                                          boardSize, maxTurnsForGame(boardSize));
                float score = result.whiteScore - result.blackScore;
                return make_pair((int)ai, (float)score);
            });
        }
    }

    unordered_map<int, float> scores;
    for (unsigned int i = 0; i < pop.size() * gamesPerEvaluation; i++) {
        auto result = queue.get();
        scores[result.first] += result.second / gamesPerEvaluation;
    }
    return scores;
}

CompetitiveScore::CompetitiveScore(int gamesPerEvaluation):
    gamesPerEvaluation(gamesPerEvaluation) {}

std::unordered_map<int, float> CompetitiveScore::operator()(int boardSize, const std::vector<const GPNode*>& pop) {
    for (unsigned int ai = 0; ai < pop.size(); ai++) {
        for (int game = 0; game < gamesPerEvaluation; game++) {
            queue.submit([ai, boardSize, game, &pop, this](){
                auto result = playAIMatch(*pop[ts_rand() % pop.size()], *pop[ai],
                                          (game % 2) ? BLACK : WHITE,
                                          boardSize, maxTurnsForGame(boardSize));
                float score = result.whiteScore - result.blackScore;
                return make_pair((int)ai, (float)score);
            });
        }
    }

    unordered_map<int, float> scores;
    for (unsigned int ai = 0; ai < pop.size() * gamesPerEvaluation; ai++) {
        auto result = queue.get();
        scores[result.first] += result.second / gamesPerEvaluation;
    }
    return scores;
}
