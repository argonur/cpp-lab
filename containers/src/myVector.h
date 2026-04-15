#pragma once

/*
Para dominar el std::vector en un entorno laboral, no basta con saber hacer push_back. Es crucial entender la gestión de memoria (capacidad vs. tamaño) y la eficiencia (evitar reasignaciones innecesarias).

Aquí tienes un ejercicio diseñado para simular un escenario real: Un Sistema de Procesamiento de Logs de Servidor.
El Reto: Analizador de Tráfico en Tiempo Real

Contexto: Trabajas en el equipo de infraestructura. Tu tarea es recibir una ráfaga de datos de tráfico (IDs de paquetes), filtrarlos y prepararlos para un análisis estadístico.
Requisitos del ejercicio:

    1. Optimización Preventiva: Sabes que vas a recibir exactamente 1,000,000 de IDs de paquetes. Usa el método adecuado de std::vector para preparar la memoria antes de empezar a insertar, evitando que el vector tenga que crecer y copiar datos internamente 20 veces.

    2. Carga Eficiente: Llena el vector con números aleatorios que representen los IDs.

    3. Filtrado In-place (El patrón Erase-Remove): En el mundo laboral, borrar elementos de un vector uno por uno es muy lento (O(n2)). Debes eliminar todos los IDs que sean "pares" (simulando paquetes corruptos) usando la combinación de std::remove_if y el método erase del vector. Este es el estándar de oro en C++ moderno.

    4. Gestión de Exceso de Memoria: Después de filtrar los elementos, tu vector tendrá una "capacidad" mucho mayor que su "tamaño" actual. Usa la función introducida en C++11 para liberar la memoria sobrante y ajustar el contenedor al tamaño real de los datos.

    5. Acceso Seguro vs. Rápido: Implementa una función que reciba el vector y un índice, y devuelva el ID. Usa el método que lanza una excepción si el índice está fuera de rango (seguridad laboral) en lugar del operador [].

Conceptos clave que estarás practicando:

    reserve() vs resize(): Entender la capacidad.

    shrink_to_fit(): Optimización de recursos.

    at() vs operator[]: Programación defensiva.

    Algoritmos de la STL: Integración de <algorithm> con vectores.

*/

#include <vector>
#include <cstdint>
#include <string>

void imprimirEstado(const std::string& paso, const std::vector<uint32_t>& v);
void myVectorExercise();

