#include <stdexcept>
#include "gtest/gtest.h"
#include "bitset2d.h"

TEST(Bitset2DTest, IsInitializedToFalse) {
    int width = 21, height = 13;
    Bitset2D b(width, height);
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            ASSERT_FALSE(b.get(x, y));
        }
    }
}

TEST(Bitset2DTest, CanSetBitsOnOrOff) {
    Bitset2D b(1, 1);
    ASSERT_FALSE(b.get(0, 0));
    b.set(0, 0, true);
    ASSERT_TRUE(b.get(0, 0));
    b.set(0, 0, false);
    ASSERT_FALSE(b.get(0, 0));
}

TEST(Bitset2DTest, CanSetWithoutAffectingOtherBits) {
    int width = 11, height = 5;
    Bitset2D b(width, height);
    b.set(1, 3, true);
    ASSERT_TRUE(b.get(1, 3));
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            if (!(x == 1 && y == 3))
                ASSERT_FALSE(b.get(x, y));
        }
    }
}

TEST(Bitset2DTest, ThrowsExceptionWhenOOB) {
    Bitset2D b(1, 5);
    ASSERT_THROW(b.get(-1, 3), std::runtime_error);
    ASSERT_THROW(b.get(1, 3), std::runtime_error);
    ASSERT_THROW(b.get(0, 5), std::runtime_error);
    ASSERT_THROW(b.get(0, -1), std::runtime_error);
    ASSERT_THROW(b.set(1, 3, true), std::runtime_error);
    ASSERT_THROW(b.set(-1, 3, true), std::runtime_error);
    ASSERT_THROW(b.set(0, 5, true), std::runtime_error);
    ASSERT_THROW(b.set(0, -1, true), std::runtime_error);
}
