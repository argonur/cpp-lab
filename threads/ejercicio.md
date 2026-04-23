# 🧩 Ejercicio: Job System (Productor–Consumidor con Thread Pool)

Vas a implementar un sistema simple de ejecución de tareas.
Primero en **C++11 (manual y propenso a errores)**, luego lo migras a **C++20 (más robusto)**.

## 🎯 Objetivo

Construir una clase:

```cpp
class ThreadPool {
public:
    ThreadPool(size_t num_threads);
    ~ThreadPool();

    void enqueue(std::function<void()> job);
};
```

## 🧱 PARTE 1 — Implementación en C++11

Requisitos

### 1. Threads internos

Crear `N` threads con `std::thread`
Cada thread ejecuta un loop:

```cpp
while (running) {
    // tomar tarea de la cola
}
```

### 2. Cola de trabajos

Usa:

- `std::queue<std::function<void()>>`
- `std::mutex`
- `std::condition_variable`

### 3. Sincronización

Threads deben:

- esperar si no hay trabajo (`condition_variable`)
- despertar cuando hay trabajo

### 4. Shutdown correcto (aquí está lo importante)

Debes implementar:

- Un flag `bool stop`
- En destructor:
    - marcar `stop = true`
    - notificar a todos (`notify_all`)
    - hacer `join()` a todos los threads

👉 Si haces esto mal:

- deadlock
- threads colgados
- o `std::terminate()`

### 💥 Errores que probablemente vas a cometer (y está bien)

- olvidar `notify_all`
- race condition en la cola
- threads que no salen del loop
- `join()` que nunca regresa

👉 justo ahí está el aprendizaje

### 🧪 Test mínimo

```cpp
ThreadPool pool(4);

for (int i = 0; i < 100; i++) {
    pool.enqueue([i] {
        std::cout << "Job " << i << "\n";
    });
}
```

## 🧠 PARTE 2 — Problemas del diseño C++11

Cuando termines, analiza:

- ¿cómo detienes los threads?
- ¿qué pasa si una tarea se cuelga?
- ¿qué pasa si quieres cancelar?
- ¿qué tan seguro es el shutdown?

👉 aquí empiezas a ver por qué C++11 se queda corto

## 🚀 PARTE 3 — Migración a C++20

Ahora rehaces el mismo sistema usando:

### 1. `std::jthread`

Reemplaza:

```cpp
std::thread
```

por:

```cpp
std::jthread
```

👉 Ya no necesitas `join()` manual

### 2. `std::stop_token`

Tu loop cambia de:

```cpp
while (!stop)
```

a:

```cpp
while (!st.stop_requested())
```

Y el thread:

```cpp
std::jthread t([this](std::stop_token st) {
    worker_loop(st);
});
```

### 3. Eliminación del flag manual

👉 Ya no necesitas:

```cpp
bool stop;
```

### 4. Shutdown limpio

En destructor:

- solo dejas que `jthread` muera
- opcional: `request_stop()`

### 5. Mejora opcional (nivel pro)

Usa:

- `std::atomic<bool>` (si necesitas estado compartido)
- o prueba `atomic_wait` en vez de `condition_variable`

## ⚖️ PARTE 4 — Comparación (esto es lo importante)

Cuando termines, respóndete:

C++11

- ¿cuánto control manual tienes?
- ¿qué tan fácil es romper el sistema?
- ¿qué tan complejo es el shutdown?

C++20

- ¿qué tanto simplifica `jthread`?
- ¿qué tan clara es la cancelación?
- ¿qué tan seguro es el lifecycle?

## 🧠 Lo que realmente estás aprendiendo

Este ejercicio no es sobre threads.

Es sobre:

- lifecycle management
- diseño concurrente
- shutdown correcto (esto es CRÍTICO en sistemas reales)
- evitar UB silencioso

## 🔥 Bonus (si quieres subir nivel)

Después de hacerlo:

1. Agrega:
    - prioridad de tareas
    - límite de cola
    - manejo de excepciones en jobs
2. Luego:
    - mide performance
    - detecta contention

## 🎯 Mi recomendación directa

Haz esto en serio:

1. Implementa C++11
2. Haz que falle (a propósito)
3. Arréglalo
4. Migra a C++20

👉 Ese proceso es lo que te va a convertir en alguien sólido en concurrencia.
