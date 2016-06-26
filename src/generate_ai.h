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
    // num games in a row to play when evaluating an ai
    int gamesPerEvaluation; 
    // standard compeditor ai
    const GPNode* benchmarkAI;
    // initialize a new ai
    std::function<GPNode*()> createNewAI;
    // produce next generation
    std::function<
        std::vector<const GPNode*>
        (const std::vector<const GPNode*>&, const std::unordered_map<int, float>&)>
            evolveNextGeneration;
};

GPNode* generateAI(int boardSize, int maxTurnsPerGame, const TrainingOptions& options, std::function<void(const TrainingData&)> logger);

#endif
