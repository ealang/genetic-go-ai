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

TEST(Bitset2DTest, CanConstructFromCopy) {
    int width = 2, height = 1;
    Bitset2D a(width, height);
    a.set(0, 0);
    Bitset2D b(a);
    ASSERT_TRUE(b.get(0, 0));
    ASSERT_EQ(1, b.count());
}

TEST(Bitset2DTest, CanSetBitsOnOrOff) {
    Bitset2D b(1, 1);
    ASSERT_FALSE(b.get(0, 0));
    b.set(0, 0, true);
    ASSERT_TRUE(b.get(0, 0));
    b.set(0, 0, false);
    ASSERT_FALSE(b.get(0, 0));
}

TEST(Bitset2DTest, CanCountSetBits) {
    int width = 2, height = 2;
    Bitset2D a(width, height);
    ASSERT_EQ(0, a.count());
    a.set(0, 0, true);
    ASSERT_EQ(1, a.count());
    a.set(1, 0, true);
    ASSERT_EQ(2, a.count());
    a.set(1, 0, true);
    ASSERT_EQ(2, a.count());
    a.set(1, 0, false);
    ASSERT_EQ(1, a.count());
}

TEST(Bitset2D, CanInvertBits) {
    int width = 3, height = 1;
    Bitset2D a(width, height);
    a.set(1, 0, true);
    Bitset2D b = ~a;
    ASSERT_TRUE(b.get(0, 0));
    ASSERT_FALSE(b.get(1, 0));
    ASSERT_EQ(2, b.count());
}

TEST(Bitset2D, CanAndToCreateCopy) {
    int width = 4, height = 1;
    Bitset2D a(width, height);
    Bitset2D b(width, height);
    a.set(2, 0, true);
    a.set(3, 0, true);
    b.set(1, 0, true);
    b.set(3, 0, true);

    Bitset2D c = b & a;
    ASSERT_FALSE(c.get(0, 0));
    ASSERT_FALSE(c.get(1, 0));
    ASSERT_FALSE(c.get(2, 0));
    ASSERT_TRUE(c.get(3, 0));
    ASSERT_EQ(1, c.count());
}

TEST(Bitset2D, CanAndWithOtherBitset) {
    int width = 4, height = 1;
    Bitset2D a(width, height);
    Bitset2D b(width, height);
    a.set(2, 0, true);
    a.set(3, 0, true);
    b.set(1, 0, true);
    b.set(3, 0, true);

    b &= a;
    ASSERT_FALSE(b.get(0, 0));
    ASSERT_FALSE(b.get(1, 0));
    ASSERT_FALSE(b.get(2, 0));
    ASSERT_TRUE(b.get(3, 0));
    ASSERT_EQ(1, b.count());
}

TEST(Bitset2DTest, CanSetBitsWithShortcutSet) {
    Bitset2D b(1, 1);
    b.set(0, 0);
    ASSERT_TRUE(b.get(0, 0));
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
