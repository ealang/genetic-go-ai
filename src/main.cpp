#include <cstring>
#include <iostream>
#include "evolution_algs.h"
#include "generate_ai.h"
#include "gpnodes_experimental.h"
#include "gptree.h"
#include "scoring_algs.h"
#include "ui.h"

using namespace std;

void logTrainingData(const TrainingData& data) {
    cout << "training_data " <<
        "{" << 
            "\"ai\": " << data.aiNum << ", " <<
            "\"gen\": " << data.generationNum << ", " <<
            "\"size\": " << treeNodeCount(data.ai) << ", " <<
            "\"depth\": " << treeDepth(data.ai) << ", " <<
            "\"cv_score\": " << data.score << ", " <<
            "\"program\": \"" << data.ai->toString() << "\"" <<
        "}" << endl;
}

GPNode* trainAI(int boardSize) {
    CompetitiveScore evScoring(30);

    RandomIntNode benchmarkAI(0, 10000);
    BenchmarkScore cvScoring(4, &benchmarkAI);
    
    TrainingOptions options;
    options.populationSize = 100;
    options.numGenerations = 30;
    options.createNewAI = createRandomAI;
    options.evolveFunc = evolvePopulationCrossover;
    options.evScoreFunc = [&evScoring](int boardSize, const vector<const GPNode*>& pop) {
        return evScoring(boardSize, pop);
    };
    options.cvScoreFunc = [&cvScoring](int boardSize, const vector<const GPNode*>& pop) {
        return cvScoring(boardSize, pop);
    };

    return generateAI(boardSize, options, logTrainingData);
}

int main(int arglen, char **argv) {
    const int boardSize = 7;
    GPNode* ai = trainAI(boardSize);

    cout << ai->toString() << endl;

    if (arglen == 2 && strcmp(argv[1], "--play") == 0)
        playInteractiveVsAI(boardSize, ai);

    cleanupTree(ai);
    return 0;
}
