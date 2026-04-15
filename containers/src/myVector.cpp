#include <iostream>
#include <vector>
#include <cstdint> // Para uint32_t
#include <random>
#include <algorithm> // Para std::remove_if
#include "myVector.h"

void imprimirEstado(const std::string& paso, const std::vector<uint32_t>& v) {
    std::cout << "--- " << paso << " ---" << std::endl;
    std::cout << "Size:     " << v.size() << std::endl;
    std::cout << "Capacity: " << v.capacity() << std::endl;
    std::cout << "----------------------------\n" << std::endl;
}

void myVectorExercise() {

    // 1. Optimización Preventiva
    std::vector<uint32_t> paquetes;
    
    // AQUÍ: Usa reserve para preparar el espacio sin llenar el vector de basura
    paquetes.reserve(1000000);

    imprimirEstado("Después de reserve(1M)", paquetes);

    // 2. Carga Eficiente

    // --- ENFOQUE MODERNO DE AZAR ---
    std::random_device rd; // Fuente de entropía (el "ruido" inicial del sistema) 
    std::mt19937 motor(rd()); // Motor de generación (sembrado con la entropía)
    std::uniform_int_distribution<uint32_t> dist(1, 1000000); // Distribución (definimos el rango de IDs: de 1 a 1,000,000)

    for (size_t i = 0; i < 1000000; ++i) {
        // Generamos el número usando el motor y la distribución
        paquetes.push_back(dist(motor));
    }

    imprimirEstado("Después de push_back (1M elementos)", paquetes);

    // 3. Filtrado In-place (El patrón Erase-Remove)
    // --- ENFOQUE MODERNO DE FILTRADO ---
    // Usamos remove_if para mover los elementos "buenos" al frente del vector
    auto nuevoFin = std::remove_if(paquetes.begin(), paquetes.end(), [](uint32_t id) {
        return id % 2 == 0; // Marca los IDs pares para eliminación
    });
    imprimirEstado("Después de remove_if (pero antes de erase)", paquetes);
    
    // Eliminamos los elementos marcados para eliminación
    paquetes.erase(nuevoFin, paquetes.end());
    imprimirEstado("Después de erase", paquetes);

    // 4. Gestión de Exceso de Memoria
    paquetes.shrink_to_fit();
    imprimirEstado("Después de shrink_to_fit", paquetes);

    // 5. Acceso Seguro vs. Rápido
    auto obtenerID = [](const std::vector<uint32_t>& v, size_t indice) -> uint32_t {
        try {
            return v.at(indice); // at() lanzará std::out_of_range si el índice es inválido
        } catch (const std::out_of_range& e) {
            std::cerr << "Error: Índice fuera de rango: " << e.what() << std::endl;
            return 0; // O podrías lanzar una excepción personalizada o manejarlo de otra manera según tus necesidades
        }
    };

    std::cout << "ID en el índice 10: " << obtenerID(paquetes, 10) << std::endl; // Ejemplo de acceso seguro
    std::cout << "ID en el índice 1000000: " << obtenerID(paquetes, 1000000) << std::endl; // Ejemplo de acceso fuera de rango

    return;
}