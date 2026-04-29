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
                        // esperamos a que haya tareas o a que nos pidan parar
                        return stop || !tasks.empty();
                    });

                    // Si nos pidieron parar y no hay tareas → salir
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


ThreadPoolCpp20::ThreadPoolCpp20(size_t numThreads) {
    for (size_t i = 0; i < numThreads; ++i) {
        workers.emplace_back([this](std::stop_token st) {
            worker_loop(st);
        });
    }
}

ThreadPoolCpp20::~ThreadPoolCpp20() {

    // request_stop()
    for (auto& t : workers) {
        t.request_stop();   // explícito
    }

    //std::cout << "Destructor de ThreadPoolCpp20: notificando a los workers..." << std::endl;
    cv.notify_all(); // Eso despierta a todos y cada uno ve su stop_requested()
    //std::cout << "Destructor de ThreadPoolCpp20: esperando a que los workers terminen..." << std::endl;
}

void ThreadPoolCpp20::enqueue(std::function<void()> task) {
    {
        std::lock_guard<std::mutex> lock(mtx);
        tasks.push(std::move(task));
    }
    cv.notify_one();
}

void ThreadPoolCpp20::worker_loop(std::stop_token st) {
    while (true) {
        std::function<void()> task;

        {
            std::unique_lock<std::mutex> lock(mtx);

            // Esperamos a que haya tareas o a que nos pidan parar
            cv.wait(lock, st, [this, &st]() {
                //std::cout << "Thread " << std::this_thread::get_id() << " despertada. st.stop_requested(), tasks.empty(): " << st.stop_requested() << ", " << tasks.empty() << std::endl;
                return st.stop_requested() || !tasks.empty();
            });

            // Si nos pidieron parar y no hay tareas → salir
            if (st.stop_requested() && tasks.empty())
            {
                //std::cout << "Thread " << std::this_thread::get_id() << " saliendo" << std::endl;
                return;
            }

            task = std::move(tasks.front());
            tasks.pop();
        }

        try {
            if (task)
                task();
        } catch (const std::exception& e) {
            std::cerr << "Task exception: " << e.what() << "\n";
        } catch (...) {
            std::cerr << "Unknown exception in task\n";
        }

    }
}
