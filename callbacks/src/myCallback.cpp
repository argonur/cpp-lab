#include "myCallback.h"
#include <iostream>

/*
Ejercicio de callbacks:
Implementa:

void forEach(int* data, int size, ??? callback);

Y úsalo con:

función normal
lambda que modifica
lambda que acumula
*/

void printSquare(int x) {
    std::cout << "Square of " << x << " is " << x * x << std::endl;
}

void forEach(int* data, int size, void (*callback)(int)) {
    std::cout << "Usando function pointer:" << std::endl;
    for (int i = 0; i < size; i++) {
        callback(data[i]);
    }
}

void forEach(int* data, int size, std::function<void(int)> callback) {
    std::cout << "Usando std::function:" << std::endl;
    for (int i = 0; i < size; i++) {
        callback(data[i]);
    }
}

void myCallbackExercise() {
    int data[] = {1, 2, 3, 4, 5};
    int size = sizeof(data) / sizeof(data[0]);

    std::cout << "Uso de una función normal:" << std::endl;
    forEach(data, size, printSquare);

    std::cout << "\nUso de una lambda que modifica:" << std::endl;
    forEach(data, size, [](int& x) {
        std::cout << "Double of " << x << " is ";
        x = x * 2; // Modificamos x afectando al array original
        std::cout  << x << std::endl;
    }); 

    std::cout << "\nUso de una lambda que acumula:" << std::endl;
    int sum = 0;
    forEach(data, size, [&sum](int x) {
        sum += x;
    });
    std::cout << "Sum of the elements is " << sum << std::endl;
}

