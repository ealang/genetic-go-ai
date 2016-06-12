#ifndef TASK_QUEUE_H
#define TASK_QUEUE_H

#include <algorithm>
#include <atomic>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <thread>
#include <vector>

// Provides a threaded task queue
template <typename T>
class TaskQueue {
    TaskQueue(const TaskQueue&) = delete;
    TaskQueue& operator=(const TaskQueue&) = delete;

    std::vector<std::thread> threadPool;

    std::vector<std::function<T(void)>> taskQueue;
    std::vector<T> resultsQueue;

    std::mutex taskMutex, resultsMutex;
    std::condition_variable taskCondition, resultsCondition;

    std::atomic<int> pendingResults;
    std::atomic<bool> terminateRequested;
public:
    TaskQueue(): 
        pendingResults(0),
        terminateRequested(false) 
    {
        auto workerMain = [this]() {
            while (true) {
                std::function<T(void)> task;
                {
                    std::unique_lock<std::mutex> lock(taskMutex);
                    taskCondition.wait(lock, [this]() {
                        return !taskQueue.empty() || terminateRequested.load();
                    });
                    if (terminateRequested.load()) {
                        return;
                    }
                    task = taskQueue.back();
                    taskQueue.pop_back();
                }

                T result = task();
                {
                    std::lock_guard<std::mutex> lg(resultsMutex);
                    resultsQueue.push_back(result);
                    --pendingResults;
                    resultsCondition.notify_all();
                }
            }
        };

        int numThreads = std::max((unsigned int)1,
                                  std::thread::hardware_concurrency());
        for (int i = 0; i < numThreads; i++) {
            threadPool.push_back(std::thread(workerMain));
        }
    }

    ~TaskQueue() {
        terminateRequested.store(true);
        taskCondition.notify_all();
        for (auto& thread: threadPool) {
            thread.join();
        }
    }

    void submit(std::function<T(void)> task) {
        std::lock_guard<std::mutex> lock(taskMutex);
        ++pendingResults;
        taskQueue.push_back(task);
        taskCondition.notify_one();
    }

    // fire callback as results become available
    // blocks waiting for outstanding tasks to complete
    void forEach(std::function<void(const T&)> callback) {
        auto drainResults = [this, &callback](){
            std::vector<T> partResults;
            {
                std::lock_guard<std::mutex> lock(resultsMutex);
                partResults = resultsQueue;
                resultsQueue.clear();
            }
            for (auto r: partResults) {
                callback(r);
            }
        };

        while (true) {
            drainResults();
            {
                std::unique_lock<std::mutex> lock(resultsMutex);
                if (pendingResults.load() == 0) {
                    break;
                }
                resultsCondition.wait(lock);
            }
        }
        drainResults();
    }
};

#endif
