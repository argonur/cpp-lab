# `constinit`

`constinit` es de esas cosas que no lucen, pero te quitan una clase completa de bugs raros en sistemas.

## 🧠 ¿Qué es `constinit`?

> **Garantiza que una variable tenga inicialización estática (en compile-time / antes de `main`).**

No dice que sea constante. Solo dice:

- **se inicializa temprano**
- **no hay inicialización dinámica en runtime**

## 🔧 Ejemplo básico

```cpp
constinit int x = 10;
```

👉 `x`:

- se inicializa antes de `main`
- pero sí **puede cambiar después**

```cpp
x = 20;  // ✅ válido
```

## 🔥 Problema que resuelve

### ❌ “Static initialization order fiasco”

Ejemplo típico:

```cpp
// file1.cpp
int a = getValue();  // runtime

// file2.cpp
extern int a;
int b = a + 1;       // ¿a ya está inicializado?
```

- 👉 El orden entre archivos **no está garantizado**
- 👉 Puede romperse en runtime 😬

### ✅ Con `constinit`

```cpp
constinit int a = 10;
constinit int b = a + 1;
```

- 👉 Ambos se inicializan en fase estática
- 👉 No hay orden ambiguo

## ⚠️ Restricción clave

La inicialización debe ser **constante (compile-time)**

```cpp
int f();

constinit int x = f();  // ❌ error
```

👉 Porque `f()` es runtime

## 🧠 Diferencia con `constexpr`

### `constexpr`

```cpp
constexpr int x = 10;
```

- 👉 constante
- 👉 no se puede modificar

### `constinit`

```cpp
constinit int x = 10;
```

- 👉 no constante
- 👉 pero inicialización garantizada

## 🔥 Comparación directa

|Feature|`constexpr`|`constinit`|
|-|-|-|
|Compile-time init|✅|✅|
|Inmutable|✅|❌|
|Uso en funciones|✅|❌|
|Uso en globals|✅|✅|

## 🧩 Ejemplo útil en sistemas

Configuración global segura

```cpp
constinit uint32_t systemClock = 16000000;
```

👉 Sabes que:

- está listo antes de cualquier uso
- no depende de runtime

## 🔥 Caso real importante

### ❌ Sin `constinit`

```cpp
int global = compute();  // runtime init
```

- 👉 puede ejecutarse antes/después de otras cosas
- 👉 orden incierto

### ✅ Con `constinit`

```cpp
constinit int global = 42;
```

👉 seguro y determinista

## ⚠️ Importante

`constinit` **no obliga a ser** `constexpr` **variable**

```cpp
constinit int x = 10;

x = 20;  // sigue siendo mutable
```

## 🧠 Insight importante

`constinit` expresa:

> “Esto debe estar listo antes de que el programa empiece a correr”

## 🎯 Cuándo usarlo

Usa `constinit` cuando:

- tienes **variables globales o estáticas**
- quieres evitar inicialización dinámica
- necesitas comportamiento determinista
- estás en sistemas / embedded / bajo nivel

## ❌ Cuándo NO sirve

- variables locales
- lógica compleja
- funciones

## 🔥 Ejemplo completo

```cpp
#include <cstdint>

constexpr uint32_t BASE_CLOCK = 16000000;

constinit uint32_t systemClock = BASE_CLOCK;
```

- 👉 `BASE_CLOCK` → constante
- 👉 `systemClock` → inicializado temprano pero modificable

## 🧠 Conexión con todo lo que viste

- `constexpr` → calcular en compile-time
- `consteval` → obligar compile-time
- `constinit` → garantizar inicialización temprana

## 🎯 Resumen directo

- `constinit` = inicialización estática garantizada
- no implica inmutabilidad
- evita bugs de orden de inicialización
- útil en globals y sistemas
