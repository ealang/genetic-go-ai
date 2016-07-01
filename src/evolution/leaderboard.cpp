#include <utility>
#include <algorithm>
#include "leaderboard.h"
#include "gptree.h"


LeaderBoard::LeaderBoard() {}

LeaderBoard::~LeaderBoard() {
    if (leader) {
        cleanupTree(leader);
    }
    for (auto ai: roundLeaders) {
        cleanupTree(ai);
    }
}

void LeaderBoard::update(const std::vector<const GPNode*>& pop, const std::unordered_map<int, float>& scores) {
    auto best = max_element(scores.begin(), scores.end(), 
                    [](std::pair<int, float> a, std::pair<int, float> b) { return a.second < b.second; });

    int i = best->first,
        score = best->second;
    if (score > leaderScore || leader == nullptr) {
        leaderScore = score;
        if (leader) {
            cleanupTree(leader);
        }
        leader = pop[i]->clone();
    }
    roundLeaders.emplace_back(pop[i]->clone());
}

const GPNode* LeaderBoard::alltime() const {
    return leader;
}

const GPNode* LeaderBoard::lastround() const {
    return roundLeaders.back();
}

const GPNode* LeaderBoard::round(int i) const {
    return roundLeaders.at(i);
}
