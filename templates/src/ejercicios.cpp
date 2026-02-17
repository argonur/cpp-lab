#include "ejercicios.h"
#include "templates.h"
#include "millisecond.h"
#include <iostream>


/*
Aufgabe: Erstes Template schreiben
1. Schreibe eine Funktion pow, die einen Wert übergeben bekommt und diesen mit einem Integer potenziert.
Der Wert soll einen beliebigen Typ haben können. Das Ergebnis soll zurückgegeben werden.
2. Welche Eigenschaften muss ein Typ haben, damit er für die pow-Funktion eingesetzt werden kann? Ergänze
die Millisecond Klasse, so dass pow für deren Objekte funktioniert.

int i=3; int iRes = pow(i,1); // 3^1 = 3
float f=2.5; float fRes = pow(f, 2); // 2.5^2=6.25
Millisecond t(5); Millisecond tRes = pow(t, 3); // 5ms^3=125m
*/
void ejercicio1() {
    int i = 3;
    int iRes = pow(i, 1);
    std::cout << "power(" << i << ", 1) = " << iRes << std::endl;

    float f = 2.5f;
    float fRes = pow(f, 2);
    std::cout << "power(" << f << ", 2) = " << fRes << std::endl;
    
    Millisecond t(5);
    Millisecond tRes = pow(t, 3);
    std::cout << "power(" << t.get() << ", 3) = " << tRes.get() << std::endl;
}