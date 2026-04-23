#include "threadPool.h"
#include <iostream>

ThreadPool::ThreadPool(size_t numThreads) : stop(false) {
    for (size_t i = 0; i < numThreads; ++i) {
        workers.emplace_back([this]() {
            while (true) {
                std::function<void()> task;

                {
                    std::unique_lock<std::mutex> lock(mtx);

                    cv.wait(lock, [this]() {
                        return stop || !tasks.empty();
                    });

                    if (stop && tasks.empty())
                        return;

                    task = std::move(tasks.front());
                    tasks.pop();
                }

                try {
                    task();
                } catch (const std::exception& e) {
                    std::cerr << "Task exception: " << e.what() << "\n";
                } catch (...) {
                    std::cerr << "Unknown exception in task\n";
                }
            }
        });
    }
}

ThreadPool::~ThreadPool() {
    {
        std::unique_lock<std::mutex> lock(mtx);
        stop = true;
    }

    cv.notify_all();

    for (std::thread &worker : workers) {
        worker.join();
    }
}

void ThreadPool::enqueue(std::function<void()> task) {
    {
        std::lock_guard<std::mutex> lock(mtx);
        
        if (stop)
            throw std::runtime_error("enqueue on stopped ThreadPool");

        tasks.push(std::move(task));
    }

    cv.notify_one();
}
