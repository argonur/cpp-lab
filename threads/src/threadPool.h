#pragma once

#include <vector>
#include <thread>
#include <functional>
#include <mutex>
#include <condition_variable>
//#include <atomic>
#include <queue>

class ThreadPool {

private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;

    std::mutex mtx;
    std::condition_variable cv;
    bool stop;

public:
    ThreadPool(size_t numThreads);

    ~ThreadPool();

    void enqueue(std::function<void()> task);

};