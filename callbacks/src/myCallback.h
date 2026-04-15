#pragma once

#include <functional>
#include <iostream>

/*
Ejemplo con function pointer:
⚠️ Importante: lambdas ≠ siempre function pointers

Esto compila:

[](int x) { return x; }

👉 Solo si:

NO capturas nada

Esto NO compila como function pointer:

int suma = 0;
[&](int x) { suma += x; }  // ❌ no convertible a function pointer

👉 Necesitas std::function o templates
*/
void forEach(int* data, int size, void (*callback)(int));

/*
❌ std::function en embedded
Puede usar heap
Overhead

👉 En firmware:

usa function pointers
o templates
*/
void forEach(int* data, int size, std::function<void(int)> callback);

/*
template (la correcta en sistemas)

👉 Esto acepta:

- function pointers
- lambdas con captura
- functors
*/
template<typename Func>
void forEach(int* data, int size, Func callback) {
    std::cout << "Usando template:" << std::endl;
    for (int i = 0; i < size; i++) {
        callback(data[i]);
    }
}

void myCallbackExercise();