#include "ejercicios.h"
#include "templates.h"
#include "millisecond.h"
#include <iostream>

void templateFunctions() {

    std::cout << "Ejemplo de función template max:" << std::endl;

    int i1=1; 
    int i2=2; 
    short s3=3; 
    short s4=4;
    
    std::cout << "max(i1,i2) = " << max(i1,i2) << std::endl; // (1) T=int
    std::cout << "max(s3,s4) = " << max(s3,s4) << std::endl; // (2) T=short
    //max(i1,s3); // no instance of function template "max" matches the argument list

    std::cout << "-----------------------------------" << std::endl;
}

/*
Tarea: Escribir el primer template
1. Escribe una función pow, que reciba un valor y lo eleve a la potencia de un número entero.
El valor puede ser de cualquier tipo. El resultado debe ser retornado.
2. ¿Qué propiedades debe tener un tipo para que pueda usarse en la función pow? Complementa
la clase Millisecond, de modo que pow funcione con sus objetos.

int i=3; int iRes = pow(i,1); // 3^1 = 3
float f=2.5; float fRes = pow(f, 2); // 2.5^2=6.25
Millisecond t(5); Millisecond tRes = pow(t, 3); // 5ms^3=125m
*/

void ejercicio1() {

    std::cout << "Ejemplo de función template pow:" << std::endl;

    int i = 3;
    int iRes = pow(i, 1);
    std::cout << "power(" << i << ", 1) = " << iRes << std::endl;

    float f = 2.5f;
    float fRes = pow(f, 2);
    std::cout << "power(" << f << ", 2) = " << fRes << std::endl;
    
    Millisecond t(5);
    Millisecond tRes = pow(t, 3);
    std::cout << "power(" << t.get() << ", 3) = " << tRes.get() << std::endl;

    std::cout << "-----------------------------------" << std::endl;
}

void nontypeTemplateParameter() {

     std::cout << "Ejemplo de nontype template parameter:" << std::endl;   

    int x = 5;
    std::cout << "MultWith<2>(" << x << ") = " << MultWith<2>(x) << std::endl;

    std::cout << "-----------------------------------" << std::endl;
}

void problemaConCArrays() {

    std::cout << "Problema con C-Arrays:" << std::endl;

    int arrA[2] = {1,2};
    int arrB[2] = {1,2};

    bool equal = (arrA == arrB);
    
    std::cout << "Arrarys are equal: " << equal << "\n"; // es falso porque arrA y arrB son punteros a la primera posición de cada array, y no se comparan los contenidos.

    std::cout << "-----------------------------------" << std::endl;
}


/*
Aufgabe: SafeArray
1. Schreibe ein Klassen-Template SafeArray mit Template Parametern für Element-Typ und Größe. Zwei
Member Funktionen get und set sollen das Lesen und Speichern von Elementen ermöglichen. Intern soll
zur Speicherung der Daten ein normales C-Array verwendet werden.
2. Wenn der Index der get Funktion ungültig ist, soll ein Default-Wert zurückgegeben werden. Der Default-Wert
soll per Konstruktor gesetzt werden können. Der Konstruktor soll außerdem alle Werte des SafeArray zu
Beginn auf diesen Default setzen.
*/
void ejercicio2() {

    std::cout << "Ejercicio 2: SafeArray" << std::endl;

   // Aufgabenteil 1
    SafeArray<int, 2> arr1(0); // 2 int-Werte, default value 0
    int value = 10;
    arr1.set(0, value); // Erstes Element schreiben
    int res = arr1.get(0); // Erstes Element lesen

    std::cout << "arr1.get(0) = " << res << std::endl; // Erwartet: res ist 10, da wir diesen Wert gesetzt haben

    // Aufgabenteil 2
    static const int invalidValue = -1;
    SafeArray<int, 2> arr2(invalidValue);
    int res2 = arr2.get(3); // Erwartet: res ist invalidValue, da der Index ungültig ist

    std::cout << "arr2.get(3) = " << res2 << std::endl; // Erwartet: res ist -1, da der Index ungültig ist

    std::cout << "-----------------------------------" << std::endl;
}

void comparaciónSafeArray() {
    
    SafeArray<int,2> arrA(0); 
    arrA.set(0, 1); 
    arrA.set(1, 2);
    
    SafeArray<int,2> arrB(0); 
    arrB.set(0, 1); 
    arrB.set(1, 2);
    
    bool equal = (arrA == arrB);
    
    std::cout << "Arrays sind gleich: " << (equal ? "true" : "false") << "\n";
}