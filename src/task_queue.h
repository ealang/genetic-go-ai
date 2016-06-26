#ifndef TASK_QUEUE_H
#define TASK_QUEUE_H

#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <stdexcept>
#include <thread>
#include <vector>

// Provides a threaded task queue. Client is not thread safe.
template <typename T>
class TaskQueue {
    TaskQueue(const TaskQueue&) = delete;
    TaskQueue& operator=(const TaskQueue&) = delete;

    std::vector<std::thread> threadPool;
    std::queue<std::packaged_task<T(void)>> taskQueue;
    std::queue<std::future<T>> resultsQueue;

    std::mutex taskMutex;
    std::condition_variable taskCondition;

    bool terminateRequested;
public:
    TaskQueue()
        : terminateRequested(false) 
    {
        auto workerMain = [this]() {
            while (true) {
                std::packaged_task<T(void)> task;
                {
                    std::unique_lock<std::mutex> lock(taskMutex);
                    taskCondition.wait(lock, [this]() {
                        return !taskQueue.empty() || terminateRequested;
                    });
                    if (terminateRequested) {
                        return;
                    }
                    task = std::move(taskQueue.front());
                    taskQueue.pop();
                }
                task();
            }
        };

        int numThreads = std::max((unsigned int)1,
                                  std::thread::hardware_concurrency());
        for (int i = 0; i < numThreads; i++) {
            threadPool.emplace_back(std::thread(workerMain));
        }
    }

    ~TaskQueue() {
        terminateRequested = true;
        taskCondition.notify_all();
        for (auto& thread: threadPool) {
            thread.join();
        }
    }

    void submit(std::function<T(void)> func) {
        auto task = std::packaged_task<T(void)>(func);
        resultsQueue.emplace(task.get_future());
        {
            std::lock_guard<std::mutex> lock(taskMutex);
            taskQueue.emplace(std::move(task));
        }
        taskCondition.notify_one();
    }

    T get() {
        if (resultsQueue.size() == 0) {
            throw std::runtime_error("No tasks in queue");
        } else {
            T result = resultsQueue.front().get();
            resultsQueue.pop();
            return result;
        }
    }
};

#endif
