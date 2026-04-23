# std::thread en C++11

## Introducción

`std::thread` es la abstracción básica de C++11 para crear y manejar hilos de ejecución. Representa un hilo del sistema operativo y permite ejecutar tareas concurrentemente dentro de un mismo proceso.

> ⚠️ Importante: `std::thread` **no maneja sincronización automáticamente**. El programador es responsable de evitar condiciones de carrera.

---

## Creación de un hilo

### Función simple

```cpp
#include <thread>
#include <iostream>

void work() {
    std::cout << "Hello from thread\n";
}

int main() {
    std::thread t(work);
    t.join();
}
```

### Uso con lambda

```cpp
std::thread t([] {
    // código concurrente
});
t.join();
```

## Ciclo de vida del hilo

Un `std::thread` puede estar en uno de estos estados:

- **Joinable** → asociado a un hilo activo
- **No joinable** → sin hilo asociado

## Operaciones clave

## `join()`

Espera a que el hilo termine.

```cpp
t.join();
```

## `detach()`

Desasocia el hilo del objeto `std::thread`.

```cpp
t.detach();
```

> ⚠️ El hilo sigue ejecutándose en segundo plano.

## Regla crítica

Si un objeto `std::thread` es destruido mientras sigue siendo joinable:

```cpp
std::terminate() será llamado
```

### ¿Qué es `std::terminate()` y en qué te afecta?

`std::terminate()` es una función de la librería estándar que **termina tu programa inmediatamente**.

No es un “error manejable”. Es un corte abrupto.

**Qué pasa cuando se llama:**

- Se detiene el programa
- **No se ejecutan destructores normales** (stack unwinding no ocurre como en excepciones)
- No puedes recuperarte
- Por defecto, suele terminar en `abort()` (crash)

#### ¿Cuándo entra en juego con `std::thread`?

Caso clásico:

```cpp
#include <thread>

void work() {}

int main() {
    std::thread t(work);
} // ← BOOM
```

Aquí:

- `t` sigue siendo joinable
- Se destruye el objeto `std::thread`
- → el estándar dice: **llama a** `std::terminate()`

#### Traducción práctica

Esto es lo que realmente significa:

> “Dejaste un hilo corriendo sin control → mejor mato el programa que dejar un bug silencioso”

Y está bien diseñado así. Prefiere crash que comportamiento indefinido.

#### Cómo te afecta en la vida real

- Tu programa puede crashear en producción
- Puede parecer “aleatorio” si no sabes la causa
- Es un error de diseño, no de sintaxis

#### Regla de oro

Todo `std::thread` debe terminar en:

- `join()` o
- `detach()`

Sin excepción.

## Paso de argumentos

```cpp
void work(int x) {}

std::thread t(work, 10);
```

### Paso por referencia

Por defecto, los argumentos se copian. Para pasar por referencia:

```cpp
#include <functional>

void work(int& x) {}

int value = 10;
std::thread t(work, std::ref(value));
```

## Verificación de estado

```cpp
if (t.joinable()) {
    t.join();
}
```

### ¿Cuándo un `std::thread` es joinable?

Un `std::thread` es **joinable** cuando está asociado a un hilo real del sistema.

Formalmente:

```cpp
t.joinable() == true
```

#### Casos donde es joinable

```cpp
std::thread t(work);  // ✅ joinable
```

Mientras no hagas:

- `join()`
- `detach()`
- move a otro thread

#### Casos donde NO es joinable

1. Después de `join()`

```cpp
t.join();  
t.joinable(); // ❌ false
```

2. Después de `detach()`

```cpp
t.detach();
t.joinable(); // ❌ false
```

3. Thread por defecto

```cpp
std::thread t;
t.joinable(); // ❌ false
```

(No tiene hilo asociado)

4. Después de move

```cpp
std::thread t1(work);
std::thread t2 = std::move(t1);

t1.joinable(); // ❌ false
t2.joinable(); // ✅ true
```

#### Intuición correcta

Un thread es *joinable* si:

> “Todavía eres responsable de ese hilo”

Cuando deja de ser joinable:

- ya lo esperaste (`join`)
- lo soltaste (`detach`)
- o lo transferiste (`move`)

#### ⚠️ Punto fino (nivel pro)

Este patrón es obligatorio en código serio:

```cpp
if (t.joinable()) {
    t.join();
}
```

Porque:

- Evita errores si no sabes el estado exacto
- Hace tu código robusto ante cambios

## Identificadores de hilo

```cpp
std::thread::id id = t.get_id();
```

También:

```cpp
std::this_thread::get_id();
```

## Problema: Data Race

Ejemplo incorrecto:

```cpp
int counter = 0;

void increment() {
    for (int i = 0; i < 1000; i++) {
        counter++;
    }
}
```

Ejecutar esto en múltiples hilos produce **comportamiento indefinido (UB)**.

Sincronización con `std::mutex`

```cpp
#include <mutex>

std::mutex m;
int counter = 0;

void increment() {
    for (int i = 0; i < 1000; i++) {
        std::lock_guard<std::mutex> lock(m);
        counter++;
    }
}
```

### Buenas prácticas

- Usar `std::lock_guard` (RAII)
- Evitar `lock()` / `unlock()` manual

## Deadlocks

Un deadlock ocurre cuando dos o más hilos esperan recursos mutuamente.

Ejemplo:

```cpp
std::mutex m1, m2;

void f1() {
    std::lock_guard<std::mutex> l1(m1);
    std::lock_guard<std::mutex> l2(m2);
}

void f2() {
    std::lock_guard<std::mutex> l1(m2);
    std::lock_guard<std::mutex> l2(m1);
}
```

Puede hacer deadlock porque:

- f1 bloquea m1 → espera m2
- f2 bloquea m2 → espera m1

👉 clásico “circular wait”

### ✅ ¿Qué usar en C++11 entonces?

Tienes dos opciones principales:

1. `std::lock` + `std::lock_guard` (la forma correcta en C++11)

```cpp
std::mutex m1, m2;

void f() {
    std::lock(m1, m2); // evita deadlock

    std::lock_guard<std::mutex> l1(m1, std::adopt_lock);
    std::lock_guard<std::mutex> l2(m2, std::adopt_lock);

    // sección crítica
}
```

#### 🔥 Lo importante aquí

- `std::lock(m1, m2)`:
    -  bloquea ambos mutex sin deadlock
    -  usa un algoritmo seguro
- `std::adopt_lock`:
    - le dice a `lock_guard`:
        > *“ya está bloqueado, no intentes bloquearlo otra vez”*

⚠️ Error común

Esto está mal:

```cpp
std::lock_guard<std::mutex> l1(m1);
std::lock_guard<std::mutex> l2(m2);
```

→ vuelve al problema original de deadlock.

2. Orden consistente de locks (más manual)

Otra estrategia:

```cpp
void f1() {
    std::lock_guard<std::mutex> l1(m1);
    std::lock_guard<std::mutex> l2(m2);
}

void f2() {
    std::lock_guard<std::mutex> l1(m1); // mismo orden
    std::lock_guard<std::mutex> l2(m2);
}
```

Pros:

- Simple

Contras:

- Frágil (depende de disciplina humana)
- Difícil de escalar en código grande

#### 🧠 Comparación rápida

|Enfoque|Seguro|Escalable|Recomendado|
|-|-|-|-|
|Orden manual|⚠️ depende|❌|solo casos simples|
|`std::lock` + adopt|✅|✅|✔️ C++11|
|`std::scoped_lock`|✅|✅|✔️ C++17+|

#### 🎯 Conclusión clara

Si estás en C++11:

> Usa `std::lock` + `std::lock_guard` con `std::adopt_lock`

Si puedes usar C++17:

> Usa `std::scoped_lock` y olvídate del problema

## Buenas prácticas generales

1. Siempre gestionar el ciclo de vida
    - Llamar `join()` o `detach()`
    - Preferir `join()`
2. Minimizar estado compartido
    - Preferir variables locales
    - Evitar compartir datos innecesariamente
3. Usar RAII

Ejemplo de guard para threads:

```cpp
class ThreadGuard {
    std::thread& t;
public:
    explicit ThreadGuard(std::thread& t) : t(t) {}
    ~ThreadGuard() {
        if (t.joinable())
            t.join();
    }
};
```

4. Evitar `detach()` salvo casos controlados

Riesgos:

- Acceso a memoria liberada
- Dificultad para depuración
- Falta de control del ciclo de vida

## Limitaciones de `std::thread`

- No provee mecanismos de sincronización
- No permite cancelación directa
- Manejo manual del ciclo de vida
- Fácil de usar incorrectamente

## Relación con otras herramientas de concurrencia

`std::thread` se usa junto con:

- `std::mutex`
- `std::lock_guard`
- [`std::unique_lock`](thread/unique_lock.md)
- `std::condition_variable`
- `std::atomic`

## Conclusión

`std::thread` es la base del multithreading en C++, pero por sí solo es insuficiente para escribir código concurrente seguro.

El dominio real requiere:

- Entender sincronización
- Evitar data races
- Diseñar correctamente el acceso a memoria compartida
