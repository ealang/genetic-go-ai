#include "gtest/gtest.h"
#include "task_queue.h"

using namespace std;

TEST(TaskQueueTest, CanRunJobAndGetResult) {
    struct Result { 
        int x, y; 
        bool operator==(const Result& other) const {
            return x == other.x && y == other.y;
        }
    };

    TaskQueue<Result> q;
    q.submit([]() { return Result{5, 10}; });

    vector<Result> results;
    q.forEach([&results](Result r) {
        results.push_back(r);
    });

    ASSERT_EQ((vector<Result>{Result{5, 10}}), results);
}

TEST(TaskQueueTest, CanProcessManyJobsAtOnce) {
    TaskQueue<int> q;
    vector<thread> threads;
    for (int threadNum = 0; threadNum < 4; threadNum++) {
        threads.push_back(thread([threadNum, &q]() {
            for (int i = 0; i < 100000; i++) {
                q.submit([]() {
                    return 0;
                });
            }
        }));
    }
    for (thread& t: threads) {
        t.join();
    }
    int count = 0;
    q.forEach([&count](int) {
        ++count;
    });
    ASSERT_EQ(400000, count);
}

TEST(TaskQueueTest, ClearsResultsOnceRead) {
    TaskQueue<int> q;
    q.submit([]() { return 1; });
    q.forEach([](int){});
    q.forEach([](int){
        FAIL();
    });
}
