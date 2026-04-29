#include "threadPool.h"
#include <iostream>
#include <sstream>
#include <mutex>
#include <chrono>
#include <thread>

int main() {
    {
        std::cout <<  "Parte 1" << std::endl;

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
    }

    {
        std::cout <<  std::endl;
        std:: cout << "Parte 3" << std::endl;

        ThreadPoolCpp20 pool(4);  // 4 workers

        std::mutex cout_mtx;

        for (int i = 0; i < 10; ++i) {
            pool.enqueue([i, &cout_mtx]() {
                std::ostringstream oss;
                oss << "Task " << i
                    << " en thread "
                    << std::this_thread::get_id()
                    << std::endl;

                std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Simula trabajo intenso

                std::lock_guard<std::mutex> lock(cout_mtx);
                std::cout << oss.str();
            });
        }

        // opcional: dar tiempo para ver ejecución antes de salir
        std::this_thread::sleep_for(std::chrono::seconds(2));

        //std::cout << "Main thread terminando, el destructor de ThreadPoolCpp20 se encargarán de limpiar los workers" << std::endl;   
    }
       
    return 0;
}