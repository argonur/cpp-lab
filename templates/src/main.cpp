#include "ejercicios.h"
#include "templates.h"
#include <iostream>

int main() {

    // Template functions
    templateFunctions();

    // Ejercicio 1
    ejercicio1();
    
    // Nontype template parameter example
    nontypeTemplateParameter();
    
    // Problema con C-Arrays
    problemaConCArrays();

    // Ejercicio 2: SafeArray
    ejercicio2();

    // Comparación de SafeArray
    comparaciónSafeArray();

    // Ejercicio 3: Un memcpy mejor
    ejercicio3();

    // ejemplo SFINAE
    sfinaeExample();

    return 0;
}
