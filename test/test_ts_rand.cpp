#include "gtest/gtest.h"
#include "ts_rand.h"

TEST(TSRandTest, ProducesNumbersInExpectedRange) {
    for (int i = 0; i < 1000; i++) {
        int r = ts_rand();
        ASSERT_TRUE(r >= 0 && r <= RAND_MAX);
    }
}
