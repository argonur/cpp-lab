#include "myList.h"

// ============================================================
//  Gestor de Tareas Prioritarias — Ejercicio con std::list
// ============================================================

#include <iostream>
#include <list>
#include <string>
#include <algorithm> // solo para std::advance

// Función auxiliar para imprimir cualquier lista
void imprimirLista(const std::list<std::string>& lista, const std::string& titulo) {
    std::cout << "\n=== " << titulo << " ===\n";
    if (lista.empty()) {
        std::cout << "  (vacía)\n";
        return;
    }
    int i = 1;
    for (const auto& tarea : lista) {
        std::cout << "  " << i++ << ". " << tarea << "\n";
    }
}

void myListExercise() {

    // --------------------------------------------------------
    // PASO 1 — Inserción estratégica: 5 tareas con push_back
    // --------------------------------------------------------
    // push_back agrega al FINAL de la lista en O(1)
    std::list<std::string> tareas;

    tareas.push_back("Actualizar dependencias del proyecto");
    tareas.push_back("Escribir pruebas unitarias");
    tareas.push_back("Refactorizar módulo de pagos");
    tareas.push_back("Documentar la API REST");
    tareas.push_back("Desplegar versión en staging");

    imprimirLista(tareas, "PASO 1 — Lista inicial (5 tareas)");

    // --------------------------------------------------------
    // PASO 2 — Salto de prioridad: tarea urgente con push_front
    // --------------------------------------------------------
    // push_front agrega al INICIO en O(1). Con un vector esto
    // costaría O(n) porque habría que desplazar todos los elementos.
    // ¡Aquí std::list brilla!
    tareas.push_front("CRÍTICO: Backup del servidor");

    imprimirLista(tareas, "PASO 2 — Tarea urgente al frente");

    // --------------------------------------------------------
    // PASO 3 — Inserción en medio con std::advance + insert
    // --------------------------------------------------------
    // Como std::list no tiene acceso aleatorio (no hay lista[i]),
    // usamos un iterador y lo avanzamos con std::advance.
    //
    // Queremos insertar ANTES de la 3.ª tarea (índice 2).
    // std::advance mueve el iterador n posiciones en O(n).
    auto it = tareas.begin();
    std::advance(it, 2); // it apunta ahora a la 3.ª tarea

    // insert(pos, valor) inserta ANTES de 'pos' en O(1)
    tareas.insert(it, "Revisión de seguridad");

    imprimirLista(tareas, "PASO 3 — 'Revisión de seguridad' insertada en posición 3");

    // --------------------------------------------------------
    // PASO 4 — Eliminación eficiente: pop_front y pop_back
    // --------------------------------------------------------
    // Ambas operaciones son O(1) en una lista enlazada.
    // En un vector, pop_front sería O(n) (desplazamiento).
    std::cout << "\n=== PASO 4 — Eliminando extremos ===\n";
    std::cout << "  Eliminando al frente: \"" << tareas.front() << "\"\n";
    tareas.pop_front();

    std::cout << "  Eliminando al final:  \"" << tareas.back()  << "\"\n";
    tareas.pop_back();

    imprimirLista(tareas, "Lista tras eliminar frente y final");

    // --------------------------------------------------------
    // PASO 5 — Ordenamiento nativo: list::sort()
    // --------------------------------------------------------
    // std::sort de <algorithm> requiere iteradores de acceso
    // aleatorio (como los de vector). std::list solo ofrece
    // iteradores bidireccionales, por eso tiene su propio
    // .sort() que implementa merge sort en O(n log n).
    tareas.sort();

    imprimirLista(tareas, "PASO 5 — Lista ordenada alfabéticamente");

    // --------------------------------------------------------
    // PASO 6 — Transferencia de nodos con splice
    // --------------------------------------------------------
    // splice es la GRAN ventaja de std::list:
    // mueve nodos entre listas cambiando punteros, sin copiar
    // ni destruir el objeto. Costo: O(1).
    std::list<std::string> tareasCompletadas;

    // Queremos mover la PRIMERA tarea de la lista principal
    // a la lista de completadas.
    auto itSplice = tareas.begin(); // apunta al primer elemento

    std::cout << "\n=== PASO 6 — Moviendo con splice ===\n";
    std::cout << "  Transfiriendo: \"" << *itSplice << "\"\n";

    // splice(destino_pos, lista_origen, origen_pos)
    // Mueve el elemento en 'origen_pos' de 'lista_origen'
    // e inserta ANTES de 'destino_pos' en la lista receptora.
    tareasCompletadas.splice(tareasCompletadas.begin(), tareas, itSplice);

    // Después del splice, 'itSplice' sigue siendo válido
    // y ahora pertenece a 'tareasCompletadas'.

    imprimirLista(tareas,            "Lista principal (tras splice)");
    imprimirLista(tareasCompletadas, "Tareas completadas");

    // --------------------------------------------------------
    // RESUMEN FINAL
    // --------------------------------------------------------
    std::cout << "\n======================================\n";
    std::cout << "  Tareas pendientes : " << tareas.size()            << "\n";
    std::cout << "  Tareas completadas: " << tareasCompletadas.size() << "\n";
    std::cout << "======================================\n\n";
}