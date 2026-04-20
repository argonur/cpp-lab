# `constexpr`

`constexpr` sí es una de esas features que **aportan valor real**, especialmente en sistemas.

Porque la idea es poderosa:

> **Mover trabajo de runtime a compile-time**

Eso significa:

- menos CPU en ejecución
- más seguridad
- más optimización

Y en embedded eso vale oro.

## 🧠 ¿Qué significa `constexpr`?

Significa:

> “Esto puede evaluarse en tiempo de compilación”

## 🔧 Ejemplo simple

```cpp
constexpr int square(int x) {
    return x * x;
}

constexpr int a = square(5);
```

- 👉 `a` se calcula al compilar
- 👉 No se calcula en runtime

## 🔥 Qué problema resuelve

Antes:

```cpp
const int a = square(5);
```

Eso **no garantiza** compile-time.

👉 `const` significa:

> “No cambia”

Pero no significa:

> “Se calcula al compilar”

## 🧠 Diferencia clara

### `const`

```cpp
const int x = foo();
```

👉 `foo()` puede correr en runtime

### `constexpr`

```cpp
constexpr int x = square(5);
```

👉 Debe poder resolverse en compile-time

## 🔥 constexpr en funciones

```cpp
constexpr int add(int a, int b) {
    return a + b;
}
```

Si llamas:

```cpp
constexpr int x = add(2,3);
```

👉 compile-time

Si llamas:

```cpp
int y = 5;
int z = add(y, 3);
```

👉 runtime

## 🧠 Esto es importante

Una función `constexpr`:

> **puede ejecutarse en compile-time o runtime**

según el contexto.

## 🔧 Ejemplo útil

```cpp
constexpr int bufferSize() {
    return 128;
}

int buffer[bufferSize()];
```

👉 Esto sirve porque el tamaño del array debe conocerse al compilar.

## ⚙️ En sistemas esto importa mucho

Porque puedes hacer:

- tamaños de buffers
- máscaras
- tablas
- offsets

todo calculado antes de correr.

## 🔥 Ejemplo real embebido

```cpp
constexpr uint32_t bit(int pos) {
    return 1u << pos;
}

constexpr uint32_t MASK = bit(3);
```

👉 `MASK = 8` generado en compile-time

## 🧩 Esto evita macros

Antes:

```cpp
#define BIT(x) (1u << (x))
```

Ahora:

```cpp
constexpr uint32_t bit(int x) {
    return 1u << x;
}
```

## 🟢 Ventajas sobre macros

- ✔ type-safe
- ✔ debuggeable
- ✔ scoped
- ✔ sin side effects raros

## 🔥 `constexpr` con objetos

Puedes hacer:

```cpp
struct Config {
    int size;
    int timeout;
};

constexpr Config cfg{128, 100};
```

👉 Objeto constante en compile-time

## ⚠️ Restricciones en C++11

En C++11, una función `constexpr` debía ser muy simple:

```cpp
constexpr int square(int x) {
    return x * x;
}
```

Solo una sentencia `return`.

## ❌ Esto NO en C++11

```cpp
constexpr int f(int x) {
    int y = x * 2;
    return y;
}
```

> ✅ Eso sí en C++14+

## 🧠 Esto es importante para entrevistas:

Si preguntan `constexpr` en C++11:

👉 “las funciones constexpr estaban muy limitadas; en C++14 se flexibilizaron”

## 🔥 Uso potente: tablas precalculadas

```cpp
constexpr int square(int x) {
    return x * x;
}

constexpr int table[] = {
    square(1),
    square(2),
    square(3)
};
```

👉 tabla generada al compilar

En embedded esto ahorra CPU.

## ⚠️ Importante: no todo puede ser `constexpr`

Esto no:

```cpp
constexpr int f() {
    int* p = new int(5);  // ❌
    return *p;
}
```

Porque requiere runtime.

## 🎯 Regla práctica

Usa `constexpr` para:

- constantes calculadas
- tamaños
- masks
- funciones matemáticas simples
- configuración estática

## 🔥 Ejemplo embebido muy bueno

```cpp
constexpr uint32_t baudRateDiv(uint32_t clock, uint32_t baud) {
    return clock / baud;
}

constexpr uint32_t UART_DIV = baudRateDiv(16000000, 9600);
```

👉 Registro precalculado en compile-time

## 🧠 Insight importante

`constexpr` expresa intención:

> “Esto debería resolverse antes de ejecutar”

Eso ayuda al compilador y al lector.

## 🧩 `constexpr` vs `const`

|Feature|`const`|`constexpr`|
|-|-|-|
|Inmutable|✅|✅|
|Compile-time garantizado|❌|✅|
|Funciones|❌|✅|

## 🎯 Regla moderna

> Si el valor debe conocerse al compilar, usa `constexpr`

## 🚀 Esto conecta directamente con el siguiente tema

👉 [`static_assert`](static_assert.md)

Porque si ya calculas cosas en compile-time, ahora puedes:

> validarlas en compile-time

Ejemplo:

```cpp
static_assert(bufferSize() <= 256, "Buffer demasiado grande");
```

Eso en sistemas es brutalmente útil.
