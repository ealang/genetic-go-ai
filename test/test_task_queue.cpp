#include <stdexcept>
#include "gtest/gtest.h"
#include "task_queue.h"

using namespace std;

TEST(TaskQueueTest, ThrowsWhenNoTaskQueued) {
    TaskQueue<int> q;
    ASSERT_THROW(q.get(), runtime_error);
}

TEST(TaskQueueTest, CanRunJobAndGetResult) {
    TaskQueue<int> q;
    q.submit([]() { return 5; });
    ASSERT_EQ(5, q.get());
    ASSERT_THROW(q.get(), runtime_error);
}

TEST(TaskQueueTest, ReturnsResultsInOrder) {
    TaskQueue<int> q;
	for (int i = 0; i < 1000; i++) {
		q.submit([i]() {
			return i;
		});
    }
    for (int i = 0; i < 1000; i++) {
		ASSERT_EQ(i, q.get());
    }
}
