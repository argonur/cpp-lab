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
Tarea: SafeArray
1. Escribe un template de clase SafeArray con parámetros de template para el tipo de elemento y el tamaño. Dos funciones miembro get y set deben permitir la lectura y escritura de elementos. Internamente, se debe usar un array C normal para almacenar los datos.
2. Si el índice de la función get es inválido, se debe devolver un valor predeterminado. El valor predeterminado debe poder establecerse mediante el constructor. El constructor también debe establecer todos los valores del SafeArray inicialmente en este valor predeterminado.
*/
void ejercicio2() {

    std::cout << "Ejercicio 2: SafeArray" << std::endl;

   // Aufgabenteil 1
    SafeArray<int, 2> arr1(0); // 2 valores int, valor predeterminado 0
    int value = 10;
    arr1.set(0, value); // Escribir primer elemento
    int res = arr1.get(0); // Leer primer elemento

    std::cout << "arr1.get(0) = " << res << std::endl; // Esperado: res es 10, ya que hemos establecido este valor

    // Aufgabenteil 2
    static const int invalidValue = -1;
    SafeArray<int, 2> arr2(invalidValue);
    int res2 = arr2.get(3); // Esperado: res es invalidValue, ya que el índice es inválido

    std::cout << "arr2.get(3) = " << res2 << std::endl; // Esperado: res es -1, ya que el índice es inválido

    std::cout << "-----------------------------------" << std::endl;
}

void comparaciónSafeArray() {

    std::cout << "Comparación de SafeArray:" << std::endl;
    
    SafeArray<int,2> arrA(0); 
    arrA.set(0, 1); 
    arrA.set(1, 2);
    
    SafeArray<int,2> arrB(0); 
    arrB.set(0, 1); 
    arrB.set(1, 2);
    
    bool equal = (arrA == arrB);
    
    std::cout << "Los arrays son iguales: " << (equal ? "true" : "false") << "\n";

        std::cout << "-----------------------------------" << std::endl;
}

/*
Tarea: Un memcpy mejor
void* memcpy( void* dest, const void* src, std::size_t count );

Copia count bytes desde el objeto apuntado por src al objeto apuntado por dest.

1. Examina la salida del punto de inicio enlazado. Deberían copiarse todos los elementos del array fuente al array destino posición por posición. ¿Qué sale mal? ¿Por qué?
2. Escribe una función template copy_items, que pueda usarse como memcpy, pero que mediante su signatura evite con un error de compilación que se llame incorrectamente (como en el ejemplo de abajo). Como tercer parámetro, en lugar de la longitud en bytes, se debe especificar el número de elementos a copiar. La función debería usar internamente el memcpy normal.
*/

void ejercicio3() {

    std::cout << "Ejercicio 3: Un memcpy mejor" << std::endl;

    int src[5] = {1, 2, 3, 4, 5};
    int dest[5] = {0};

    // Llamada incorrecta a memcpy - conduce a comportamiento indefinido
    //memcpy(dest, src, sizeof(src)); // Esto copia los bytes, pero no los elementos correctamente

    // Llamada correcta a la función template copy_items
    copy_items(dest, src, 5); // Copia 5 elementos de src a dest

    std::cout << "dest array after copy_items: ";
    for (int i = 0; i < 5; ++i) {
        std::cout << dest[i] << " "; // Esperado: 1 2 3 4 5
    }
    std::cout << std::endl;

    std::cout << "-----------------------------------" << std::endl;
}

void sfinaeExample() {

    std::cout << "Ejemplo de SFINAE:" << std::endl;

    struct S_with_x { int x = 42; };
    struct S_with_y { int y = 7; };
    struct S_without { };
    
    S_with_x sx;
    S_with_y sy;
    S_without swo;

    print_x_or_y(sx);
    print_x_or_y(sy);
    //print_x_or_y(swo); //no instance of overloaded function "print_x_or_y" matches the argument list

    std::cout << "-----------------------------------" << std::endl;
}