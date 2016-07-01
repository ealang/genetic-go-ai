#include "gtest/gtest.h"
#include "board.h"
#include "generate_ai.h"
#include "gptree.h"
#include "gpnodes_experimental.h"
#include "gpnode_context.h"

using namespace std;

TEST(TestGenerateAI, LogsTrainingInformation) {
    bool calledEvolveScore = false,
         calledCrossValidateScore = false,
         calledEvolveFunc = false,
         calledLogger = false;

    TrainingOptions options;
    options.populationSize = 1;
    options.numGenerations = 1;
    options.createNewAI = [](){ return new RandomIntNode(0, 10); };
    options.evScoreFunc = [&calledEvolveScore](int, const vector<const GPNode*>&){
        calledEvolveScore = true;
        return unordered_map<int, float>{{0, 10}};
    };
    options.cvScoreFunc = [&calledCrossValidateScore](int, const vector<const GPNode*>&){
        calledCrossValidateScore = true;
        return unordered_map<int, float>{{0, 20}};
    };
    options.evolveFunc = [&calledEvolveFunc](const vector<const GPNode*>& pop, const unordered_map<int, float>& scores) {
        calledEvolveFunc = true;
        assert(10 == scores.find(0)->second);

        vector<const GPNode*> newPop;
        for (auto ai: pop) {
            newPop.push_back(ai->clone());
        }
        return newPop;
    };

    int boardSize = 9;
    Board b(boardSize);
    auto logger = [&calledLogger, &b](const TrainingData& data){
        ASSERT_EQ(0, data.generationNum);
        ASSERT_EQ(0, data.aiNum);
        ASSERT_EQ(20, data.score);
        calledLogger = true;
        data.ai->get(Context{0, 0, WHITE, b, b});
    };

    GPNode* ai = generateAI(boardSize, options, logger);
    ASSERT_TRUE(calledEvolveFunc);
    ASSERT_TRUE(calledCrossValidateScore);
    ASSERT_TRUE(calledEvolveScore);
    ASSERT_TRUE(calledLogger);

    ai->get(Context{0, 0, WHITE, b, b});

    cleanupTree(ai);
}
