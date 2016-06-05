#include <stdexcept>
#include "gtest/gtest.h"
#include "bitset.h"

TEST(BitsetTest, CanReadAndWriteBits) {
    Bitset b(10);
    for (int i = 0; i < 10; i++) {
        ASSERT_FALSE(b.get(i));
    }
    b.set(3, true);
    for (int i = 0; i < 3; i++) {
        ASSERT_FALSE(b.get(i));
    }
    ASSERT_TRUE(b.get(3));
    for (int i = 4; i < 10; i++) {
        ASSERT_FALSE(b.get(i));
    }
}

TEST(BitsetTest, ThrowsExceptionWhenOOB) {
    Bitset b(10);
    ASSERT_THROW(b.get(-1), std::runtime_error);
    ASSERT_THROW(b.get(111), std::runtime_error);
    ASSERT_THROW(b.set(10, true), std::runtime_error);
}
