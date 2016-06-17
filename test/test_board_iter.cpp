#include "gtest/gtest.h"
#include "board.h"
#include "board_iter.h"

using namespace std;

TEST(BoardIterTest, WillIterateLiberties) {
    Board b(4);
    b.set(0, 0, BLACK);
    b.set(1, 0, BLACK);
    b.set(2, 0, WHITE);

    vector<int> xlib, ylib;
    iterateLiberties(b, 0, 0, [&](int x, int y) {
        xlib.push_back(x);
        ylib.push_back(y);
        return true;
    });
    ASSERT_EQ((vector<int>{0, 1}), xlib);
    ASSERT_EQ((vector<int>{1, 1}), ylib);
}
