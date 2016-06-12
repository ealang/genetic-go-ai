#include <stdexcept>
#include "gtest/gtest.h"
#include "color.h"

TEST(ColorTest, GetsOtherColor) {
    EXPECT_EQ(BLACK, otherColor(WHITE));
    EXPECT_EQ(WHITE, otherColor(BLACK));
    ASSERT_THROW(otherColor(NONE), std::runtime_error);
}
