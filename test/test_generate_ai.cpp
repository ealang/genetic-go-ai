#include "gtest/gtest.h"
#include "generate_ai.h"

TEST(TestGenerateAI, CanRunGenerator) {
    int boardSize = 4, numRounds = 1, numAI = 1;
    generateAI(boardSize, numRounds, numAI);
}
