# `consteval`

Vamos a dejar `consteval` bien aterrizado — sin confusiones con `constexpr`.

## 🧠 ¿Qué es `consteval`?

> - Una función `consteval` **DEBE ejecutarse en compile-time.**
> - Si no puede → **error de compilación.**

## 🔧 Ejemplo básico

```cpp
consteval int square(int x) {
    return x * x;
}

int a = square(5);  // ✅ OK (se evalúa en compile-time)
```

## ❌ Caso que falla

```cpp
int y = 10;
int z = square(y);  // ❌ error
```

👉 Porque `y` **no es constante en compile-time**

## 🔥 Diferencia REAL con `constexpr`

### `constexpr`

```cpp
constexpr int square(int x) {
    return x * x;
}

int y = 10;
int z = square(y);  // ✅ runtime
```

👉 flexible

### `consteval`

```cpp
consteval int square(int x) {
    return x * x;
}

int y = 10;
int z = square(y);  // ❌ error
```

👉 estricto

## 🎯 Resumen corto

|Keyword|¿Puede correr en runtime?|
|-|-|
|`constexpr`|✅ sí|
|`consteval`|❌ no|

## 🧠 Modelo mental correcto

- `constexpr` → “si puedes, hazlo en compile-time”
- `consteval` → “hazlo en compile-time o no compiles”

🔥 ¿Para qué sirve de verdad?

Para **forzar invariantes fuertes en compile-time**.

### 🧩 Ejemplo útil (validación)

```cpp
consteval int safe_div(int a, int b) {
    return b == 0 ? throw "div by zero" : a / b;
}
```

Uso:

```cpp
int x = safe_div(10, 2);  // OK
int y = safe_div(10, 0);  // ❌ error en compile-time
```

👉 Evitas errores antes de correr

## 🔥 Ejemplo potente (configuración)

```cpp
consteval int baudRateDiv(int clk, int baud) {
    return clk / baud;
}

constexpr int UART_DIV = baudRateDiv(16000000, 9600);
```

👉 Garantizas:

- cálculo en compile-time
- cero costo runtime

## ⚠️ Importante

`consteval` implica `constexpr`
pero más fuerte:

> Todo `consteval` es `constexpr`, pero no al revés.

## 🧩 Uso típico

- generación de tablas
- validación de parámetros
- metaprogramación ligera
- configuración crítica

## ⚠️ Limitaciones

No puedes hacer cosas de runtime:

```cpp
consteval int f() {
    int* p = new int(5);  // ❌ no permitido
    return *p;
}
```

## 🔥 Ejemplo interesante (errores más claros)

```cpp
consteval int power_of_two(int x) {
    if ((x & (x - 1)) != 0)
        throw "Not power of two";
    return x;
}

constexpr int val = power_of_two(8);  // OK
constexpr int bad = power_of_two(10); // ❌ error en compile-time
```

👉 Esto es brutal para sistemas

## 🧠 Insight importante

`consteval` es una herramienta de **diseño**, no solo optimización.

Te permite decir:

> “Esto es inválido incluso antes de ejecutar el programa”

## 🎯 Cuándo usarlo

Usa `consteval` cuando:

- necesitas garantías fuertes
- quieres evitar errores de configuración
- el valor debe existir en compile-time sí o sí

## ❌ Cuándo NO usarlo

- lógica normal
- datos dinámicos
- APIs flexibles

## 🧠 Conexión con todo lo que viste

- `constexpr` → cálculo opcional en compile-time
- `consteval` → cálculo obligatorio
- `static_assert` → validación
- templates → composición

👉 juntos = **código que se valida antes de existir**

🎯 Regla práctica

> Si fallar en runtime es inaceptable → usa `consteval`
