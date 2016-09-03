#ifndef LEADERBOARD_H
#define LEADERBOARD_H

#include <vector>
#include <unordered_map>

class GPNode;

class LeaderBoard {
    LeaderBoard(const LeaderBoard&) = delete;
    LeaderBoard& operator=(const LeaderBoard&) = delete;

    float leaderScore = 0;
    GPNode* leader = nullptr;
    std::vector<const GPNode*> roundLeaders;
public:
    LeaderBoard();
    virtual ~LeaderBoard();
    void update(const std::vector<const GPNode*>& pop, const std::unordered_map<int, float>& scores);
    const GPNode* alltime() const;
    const GPNode* lastround() const;
    const GPNode* round(int) const;
};

#endif
