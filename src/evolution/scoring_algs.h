#ifndef SCORING_ALGS_H
#define SCORING_ALGS_H

#include "task_queue.h"
#include <unordered_map>
#include <utility>
#include <vector>

class GPNode;

// Score by playing against a benchmark ai
class BenchmarkScore {
    static const int gamesPerEvaluation = 30;
    TaskQueue<std::pair<int, float>> queue;
    const GPNode* benchmark;
public:
    BenchmarkScore(const GPNode* benchmark);
    std::unordered_map<int, float> operator()(int boardSize, const std::vector<const GPNode*>& pop);
};

// Score by playing ai against each other
class CompetitiveScore {
    static const int gamesPerEvaluation = 30;
    TaskQueue<std::pair<int, float>> queue;
public:
    std::unordered_map<int, float> operator()(int boardSize, const std::vector<const GPNode*>& pop);
};

#endif
