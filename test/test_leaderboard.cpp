#include <stdexcept>
#include "gtest/gtest.h"
#include "board.h"
#include "leaderboard.h"
#include "gpnode_context.h"
#include "gpnodes_experimental.h"

using namespace std;

class LeaderboardTest: public ::testing::Test {
public:
    Board board;
    Context context;
    LeaderboardTest():
        board(5),
        context{0, 0, BLACK, board, board} {}
};

TEST_F(LeaderboardTest, TracksBestAndLatest) {
    LeaderBoard lb;

    IntConstNode ai1(1), ai2(2);
    vector<const GPNode*> ai = {&ai1, &ai2};

    lb.update(ai, unordered_map<int, float> {{0, 2.0}, {1, -2}});
    ASSERT_EQ(1, lb.alltime()->get(context));
    ASSERT_EQ(1, lb.lastround()->get(context));

    lb.update(ai, unordered_map<int, float> {{0, -11.0}, {1, 1}});
    ASSERT_EQ(1, lb.alltime()->get(context));
    ASSERT_EQ(2, lb.lastround()->get(context));
}

TEST_F(LeaderboardTest, TracksBestPerRound) {
    LeaderBoard lb;

    IntConstNode ai1(1), ai2(2);
    vector<const GPNode*> ai = {&ai1, &ai2};

    lb.update(ai, unordered_map<int, float> {{0, 2.0}, {1, -2}});
    lb.update(ai, unordered_map<int, float> {{0, 2.0}, {1, 3}});
    lb.update(ai, unordered_map<int, float> {{0, 22.0}, {1, 3}});
    ASSERT_EQ(1, lb.round(0)->get(context));
    ASSERT_EQ(2, lb.round(1)->get(context));
    ASSERT_EQ(1, lb.round(2)->get(context));
}
