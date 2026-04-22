# Pointer to pointer

Te la pongo directo: sí se usa, pero no en código “bonito” de alto nivel. Donde aparece de verdad es en **interfaces de bajo nivel, C APIs, y casos donde necesitas modificar un puntero desde fuera**.

Primero, qué es:
Un *pointer to pointer* (`T**`) es simplemente un puntero que apunta a otro puntero. O sea, dos niveles de indirección.

## 🧠 ¿Para qué sirve realmente?

1. Modificar un puntero dentro de una función

Este es el caso más importante.

En C/C++, los argumentos se pasan por valor. Si quieres que una función cambie el puntero original, necesitas pasar su dirección.

```cpp
void allocate(int** ptr) {
    *ptr = new int(42);
}

int* p = nullptr;
allocate(&p);

// ahora p apunta a memoria válida
```

👉 Sin `int**`, no podrías modificar p desde la función.

2. APIs estilo C (muy común en sistemas / embedded)

Muchas librerías en C usan esto porque no hay referencias como en C++.

Ejemplo típico:

```cpp
int create_object(MyObject** obj);
```

La función crea el objeto y te lo devuelve vía puntero doble.

Esto lo vas a ver en:

- librerías legacy en C
- sistemas embebidos
- drivers
- APIs tipo POSIX

3. Manejo de arreglos de punteros (matrices dinámicas)

Cuando tienes algo como:

```cpp
int** matrix;
```

Esto puede representar una matriz dinámica (arreglo de arreglos):

```cpp
matrix = new int*[rows];
for (int i = 0; i < rows; i++) {
    matrix[i] = new int[cols];
}
```

👉 Aunque aquí te digo algo importante:
esto hoy en día **no es la mejor práctica en C++ moderno** (mejor `std::vector` o un bloque contiguo).

4. Estructuras de datos (listas enlazadas, árboles)

A veces se usa para simplificar lógica al modificar nodos.

Ejemplo clásico en listas enlazadas:

```cpp
void insert(Node** head, int value) {
    Node* newNode = new Node{value, *head};
    *head = newNode;
}
```

👉 Permite modificar la cabeza de la lista sin devolver nada.

5. Manejo fino de memoria / performance

En código muy optimizado o de sistemas (tipo kernel, firmware, parsing de buffers), puedes verlos para evitar copias o manejar punteros de forma indirecta.

## ⚠️ ¿Por qué tú no lo usas casi?

Porque en C++ moderno tienes mejores herramientas:

- referencias (`T*&`)
- `std::unique_ptr` / `std::shared_ptr`
- `std::vector`, `std::array`

Ejemplo equivalente más “C++”:

```cpp
void allocate(int*& ptr) {
    ptr = new int(42);
}
```

👉 Esto reemplaza `int**` en muchos casos.

## 🧩 Lo que realmente querían evaluar en la entrevista

No querían que fueras experto usándolo diario. Querían ver si:

- entiendes **niveles de indirección**
- sabes cómo **modificar un puntero desde una función**
- conoces contexto de **C vs C++**
- no te rompes con memoria dinámica

Y tú hiciste lo correcto: lo dedujiste.

## 💡 Te dejo una forma mental clara

Piensa así:

- `T*` → “tengo un objeto”
- `T**` → “tengo una dirección donde guardo un puntero”
- `T***` → “ya estamos en terreno peligroso” 😅