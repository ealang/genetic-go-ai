#include <unordered_map>
#include <vector>

#include "gtest/gtest.h"
#include "evolution_algs.h"

using namespace std;

extern vector<int> populationDropN(const unordered_map<int, float>& scores, unsigned int numDrop);

TEST(TestEvolutionAlgs, CanDropNoNumers) {
    unordered_map<int, float> scores = {{0, 100}, {1, 50}, {2, 150}, {3, 75}};
    auto result = populationDropN(scores, 0);
    ASSERT_EQ((vector<int>{2, 0, 3, 1}), result);
}

TEST(TestEvolutionAlgs, CanDrop2SmallestNumbers) {
    unordered_map<int, float> scores = {{0, 100}, {1, 50}, {2, 150}, {3, 75}};
    auto result = populationDropN(scores, 2);

    ASSERT_EQ((vector<int>{2, 0}), result);
}
