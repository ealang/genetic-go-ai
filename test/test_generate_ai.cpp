#include "gtest/gtest.h"
#include "board.h"
#include "generate_ai.h"
#include "gptree.h"
#include "gpnodes_experimental.h"
#include "gpnode_context.h"

using namespace std;

TEST(TestGenerateAI, LogsTrainingInformation) {
    RandomIntNode benchmark(0, 1000);

    TrainingOptions options;
    options.populationSize = 1;
    options.numGenerations = 1;
    options.gamesPerEvaluation = 1;
    options.benchmarkAI = &benchmark;
    options.createNewAI = [](){ return new RandomIntNode(0, 10); };
    options.evolveNextGeneration = [](const vector<const GPNode*>& pop, const unordered_map<int, float>&) {
        vector<const GPNode*> newPop;
        for (auto ai: pop) {
            newPop.push_back(ai->clone());
        }
        return newPop;
    };

    int boardSize = 9;
    Board b(boardSize);

    bool loggerCalled = false;
    auto logger = [&loggerCalled, &b](const TrainingData& data){
        ASSERT_EQ(0, data.generationNum);
        ASSERT_EQ(0, data.aiNum);
        loggerCalled = true;
        data.ai->get(Context{0, 0, WHITE, b, b});
    };

    GPNode* ai = generateAI(boardSize, 1, options, logger);
    ASSERT_TRUE(loggerCalled);

    ai->get(Context{0, 0, WHITE, b, b});

    cleanupTree(ai);
}
