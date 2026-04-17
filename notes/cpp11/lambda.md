# Lambda expresions

Las **lambdas** son donde C++11 deja de sentirse “viejo” y empieza a ser realmente expresivo. Pero también es donde más bugs sutiles aparecen si no entiendes **captura**.

## 🧠 ¿Qué es una lambda?

Es una función anónima que puedes definir inline.

```cpp
auto suma = [](int a, int b) {
    return a + b;
};

int r = suma(2, 3);  // 5
```

## 🔧 Sintaxis (desglosada)

```cpp
[captura](parámetros) -> tipo_retorno {
    cuerpo
}
```

Ejemplo:

```cpp
int x = 10;

auto f = [x](int y) -> int {
    return x + y;
};
```

## 🔥 Lo importante de verdad: captura

Aquí está el 90% del valor (y de los errores).

### 🔹 1. Captura por valor `[=]`

```cpp
int x = 10;

auto f = [=]() {
    return x;
};
```

👉 Copia x dentro de la lambda
👉 No puedes modificar el original

### 🔹 2. Captura por referencia `[&]`

```cpp
int x = 10;

auto f = [&]() {
    x = 20;
};
```

👉 Modifica la variable original

### 🔹 3. Captura explícita

```cpp
int a = 1, b = 2;

auto f = [a, &b]() {
    // a por valor
    // b por referencia
};
```

👉 Esto es lo más limpio en código serio

## ⚠️ Error clásico (MUY importante)

❌ Capturar referencia a algo que ya no existe

```cpp
auto f;

{
    int x = 10;
    f = [&]() { return x; };  // ❌
}

f();  // UB (dangling reference)
```

👉 Esto es bug serio en sistemas

## ⚙️ Lambdas como callbacks

Aquí es donde brillan.

### 🧩 Ejemplo clásico (estilo STL)

```cpp
#include <algorithm>
#include <vector>

std::vector<int> v = {1,2,3,4};

std::for_each(v.begin(), v.end(), [](int x) {
    // callback
});
```

## 🔥 Ejemplo real de callback (más cercano a sistemas)

Imagina un driver o sistema que recibe una función:

```cpp
void processData(const std::vector<int>& data,
                 void (*callback)(int))
{
    for (auto x : data) {
        callback(x);
    }
}
```

Uso con lambda:

```cpp
processData({1,2,3}, [](int x) {
    // callback inline
    // por ejemplo:
    printf("%d\n", x);
});
```

## 🚀 Ejemplo más potente (callback con estado)

Aquí es donde lambdas superan a funciones normales.

```cpp
int suma = 0;

processData({1,2,3}, [&](int x) {
    suma += x;
});
```

👉 La lambda “recuerda” `suma`

## ⚠️ Importante: lambdas ≠ siempre function pointers

Esto compila:

```cpp
[](int x) { return x; }
```

👉 Solo si:

- NO capturas nada

Esto NO compila como function pointer:

```cpp
int suma = 0;

[&](int x) { suma += x; }  // ❌ no convertible a function pointer
```

👉 Necesitas `std::function` o templates

## 🔧 Versión moderna correcta

```cpp
#include <functional>

void processData(const std::vector<int>& data,
                 std::function<void(int)> callback)
{
    for (auto x : data) {
        callback(x);
    }
}
```

## ⚙️ En sistemas embebidos (esto te importa)

### 🟢 Cuándo SÍ usar lambdas

- ✔️ Callbacks simples
- ✔️ Algoritmos (for_each, etc.)
- ✔️ Código local y claro

### 🔴 Cuándo tener cuidado

❌ Capturas por referencia en código async

```cpp
startTask([&]() {
    use(x);  // ¿x sigue vivo?
});
```

👉 Puede romperse feo

### ❌ std::function en sistemas críticos

- Puede usar heap
- Overhead

👉 Mejor:

- templates
-  o function pointers si es simple

## 🧠 Insight importante

Lambdas son básicamente:

> “objetos con operador `()`”

Ejemplo conceptual:

```cpp
struct Lambda {
    int x;
    int operator()(int y) { return x + y; }
};
```

👉 Eso es lo que el compilador genera

## 🎯 Conclusión directa

- Lambdas = funciones inline + estado
- Poder real = captura
- Riesgo real = lifetimes + referencias

## 🧠 Regla práctica

- Usa `[&]` solo si estás seguro del lifetime
- Prefiere captura explícita `[a, &b]` en código serio
- Evita `std::function` en embedded crítico

[Regresar a temas de C++11](../cpp11.md)
