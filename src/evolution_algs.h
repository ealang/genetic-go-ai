#ifndef EVOLUTION_ALGS
#define EVOLUTION_ALGS

#include <vector>
#include <unordered_map>

class GPNode;

GPNode* createRandomAI();
std::vector<const GPNode*> evolvePopulationCrossover(const std::vector<const GPNode*>& oldPop, const std::unordered_map<int, int>& scores);
std::vector<const GPNode*> evolvePopulationOperationsMix(const std::vector<const GPNode*>& oldPop, const std::unordered_map<int, int>& scores);

#endif
