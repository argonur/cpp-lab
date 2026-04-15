#pragma once

/*
Para entender por qué usarías una std::list en lugar de un vector en un entorno profesional, tenemos que enfocarnos en su superpoder: la inserción y eliminación constante (O(1)) en cualquier posición sin mover el resto de los elementos.

En el mundo laboral, las listas se usan mucho para implementar Sistemas de Caché o Gestores de Tareas donde el orden cambia constantemente.
El Reto: Gestor de Tareas Prioritarias (Task Manager)

Contexto: Estás desarrollando el motor de un gestor de tareas. Las tareas entran en una lista, pero algunas son "Urgentes" y deben saltar al principio, otras se cancelan y deben salir de la lista sin afectar el rendimiento, y otras se mueven de posición.
Requisitos del ejercicio:

    1. Inserción Estratégica: Crea una lista de strings. Inserta 5 tareas normales usando push_back.

    2. El Salto de Prioridad: Recibe una tarea nueva llamada "CRÍTICO: Backup del servidor". En lugar de ponerla al final, úsala para practicar push_front.

    3. Inserción en Medio (Iteradores): Encuentra la tercera tarea de la lista e inserta justo antes de ella una tarea llamada "Revisión de seguridad". Aquí practicarás el uso de std::advance para moverte por la lista (ya que no puedes hacer lista[2]).

    4. Eliminación Eficiente: Borra la tarea que está al final (pop_back) y la que está al principio (pop_front).

    5. Ordenamiento Nativo: Usa el método .sort() propio de la lista (no el de <algorithm>) para ordenar las tareas alfabéticamente. Dato pro: Las listas tienen su propio sort porque el algoritmo estándar requiere acceso aleatorio, algo que las listas no tienen.

    6. Transferencia de Nodos (splice): Esta es la joya de la corona de las listas. Crea una segunda lista de "Tareas Completadas" y mueve un nodo de la lista principal a la de completadas usando .splice(). Esto mueve el elemento sin copiarlo ni destruirlo, solo cambiando los punteros.

Conceptos clave que estarás practicando:

    Iteradores Bidireccionales: Cómo navegar una lista sin índices.

    std::advance y std::next: Herramientas esenciales para moverte en estructuras no contiguas.

    splice(): La operación más eficiente de std::list que los vectores no pueden hacer.

    Gestión de punteros implícita: Entender cómo se reconectan los nodos.

¿Por qué std::list aquí?

Si usaras un vector para un gestor de tareas donde insertas cosas en medio constantemente, cada "tarea urgente" obligaría a desplazar miles de strings en memoria. Con std::list, solo cambias a dónde apuntan un par de flechas (punteros).
*/

#include <list>
#include <string>

void myListExercise();
