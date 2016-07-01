#ifndef GENERATE_AI
#define GENERATE_AI

#include <functional>
#include <vector>
#include <unordered_map>

class GPNode;

struct TrainingData {
    int generationNum;
    int aiNum;
    const GPNode* ai;
    float score;
};

struct TrainingOptions {
    // number of ai in population
    int populationSize;
    // number of ai generations to run
    int numGenerations;
    // initialize a new ai
    std::function<GPNode*()> createNewAI;
    // evolution score
    std::function<std::unordered_map<int, float>(int, const std::vector<const GPNode*>&)> evScoreFunc;
    // cross validation score 
    std::function<std::unordered_map<int, float>(int, const std::vector<const GPNode*>&)> cvScoreFunc;
    // produce next generation
    std::function<std::vector<const GPNode*>
        (const std::vector<const GPNode*>&, const std::unordered_map<int, float>&)> evolveFunc;
};

GPNode* generateAI(int boardSize, const TrainingOptions& options, std::function<void(const TrainingData&)> logger);

#endif
