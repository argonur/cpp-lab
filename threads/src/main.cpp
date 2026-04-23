#include "threadPool.h"
#include <iostream>
#include <sstream>
#include <mutex>

int main() {
    ThreadPool pool(10);

    std::mutex cout_mtx;
    
    for (int i = 0; i < 100; ++i) {

        pool.enqueue([i, &cout_mtx]() {
            std::ostringstream oss;
            oss << "Task " << i
                << " ejecutada en thread "
                << std::this_thread::get_id()
                << "\n";
            
            std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Simula trabajo intenso


            std::lock_guard<std::mutex> lock(cout_mtx);
            std::cout << oss.str();
        });
    }

    return 0;
}