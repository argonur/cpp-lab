#pragma once

#include <vector>
#include <thread>
#include <functional>
#include <mutex>
#include <condition_variable>
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

class ThreadPoolCpp20 {

private:

    std::queue<std::function<void()>> tasks;
    std::mutex mtx;
    std::condition_variable_any cv;
    std::vector<std::jthread> workers; // debido a que se declaró al final, se destruye primero que los miembros anteriores, así que no hay riesgo de acceso a miembros ya destruidos

public:
    ThreadPoolCpp20(size_t numThreads);
    ~ThreadPoolCpp20();
    void enqueue(std::function<void()> task);

private:
    void worker_loop(std::stop_token st);
};