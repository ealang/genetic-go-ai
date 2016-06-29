#ifndef LEADERBOARD_H
#define LEADERBOARD_H

#include <vector>
#include <unordered_map>
#include <utility>
#include <algorithm>

struct LeaderBoard {
    float score = 0;
    GPNode* leader = nullptr;

    void update(const std::vector<const GPNode*>& pop, const std::unordered_map<int, float>& scores) {
        auto best = max_element(scores.begin(), scores.end(), 
                        [](std::pair<int, float> a, std::pair<int, float> b) { return a.second < b.second; });

        int i = best->first,
            iscore = best->second;
        if (iscore > score || leader == nullptr) {
            score = iscore;
            if (leader) {
                cleanupTree(leader);
            }
            leader = pop[i]->clone();
        }
    };
};

#endif
