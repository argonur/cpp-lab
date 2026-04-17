# enum class

## 🧠 El problema con enum clásico

```cpp
enum Color {
    RED,
    GREEN,
    BLUE
};
```

### ❌ Problema 1: contaminación de nombres

```cpp
int RED = 5;  // 💥 conflicto
```

👉 Los valores del enum viven en el mismo scope

### ❌ Problema 2: conversiones implícitas peligrosas

```cpp
Color c = RED;

if (c == 0) { }  // 😬 válido
```

👉 RED == 0 → bugs silenciosos

### ❌ Problema 3: mezcla de enums

```cpp
enum Color { RED };
enum Status { RED };  // 💥 conflicto
```

## 🔥 Solución: enum class

```cpp
enum class Color {
    Red,
    Green,
    Blue
};
```

### 🧠 Qué cambia

1. ✅ Scope propio

```cpp
Color c = Color::Red;
```

👉 Ya no contamina el namespace

2. ✅ NO hay conversiones implícitas

```cpp
Color c = Color::Red;

if (c == 0) { }  // ❌ error
```

👉 Evita bugs

3. ✅ Tipos distintos de verdad

```cpp
enum class Color { Red };
enum class Status { Red };

Color c = Color::Red;
Status s = Status::Red;

// c == s  ❌ error
```

👉 Type-safe real

## 🔧 Tipo subyacente (muy importante en sistemas)

Por defecto es int, pero puedes controlarlo:

```cpp
enum class ErrorCode : uint8_t {
    OK = 0,
    FAIL = 1
};
```

👉 Esto es clave en:

- protocolos
- hardware
- memoria limitada

## ⚠️ Conversión explícita

A veces necesitas el valor:

```cpp
uint8_t val = static_cast<uint8_t>(ErrorCode::OK);
```

👉 Sí, es más verboso… pero más seguro

## 🔥 Ejemplo real (mejor práctica)

### ❌ Antes

```cpp
#define STATE_IDLE 0
#define STATE_RUN  1
```

👉 horrible, sin tipo

### 🟢 Con enum class

```cpp
enum class State : uint8_t {
    Idle,
    Run
};
```

Uso:

```cpp
State s = State::Idle;

if (s == State::Run) {
    // ...
}
```

## ⚙️ En sistemas embebidos

Aquí sí brilla.

### 🟢 Ventajas reales

- ✔ Control de tamaño (uint8_t, uint16_t)
- ✔ Evita mezclar estados incorrectos
- ✔ Mejor documentación del código

### 🔴 Cuidado

❌ Overhead mental (no runtime)

```cpp
if (s == 0)  // ya no puedes
```

👉 tienes que ser explícito

❌ Interfacing con C

```cpp
void c_api(int state);
```

👉 necesitas cast:

```cpp
c_api(static_cast<int>(State::Idle));
```

## 🔥 Patrón útil

### Switch seguro

```cpp
switch (s) {
    case State::Idle:
        break;
    case State::Run:
        break;
}
```

👉 Mucho más claro

## 🧠 Insight importante

`enum class` no es solo sintaxis.

👉 Es:

> “convertir constantes en tipos reales”

## 🎯 Cuándo usarlo

✔️ Siempre que:

- representes estados
- tengas valores finitos
- quieras evitar errores de tipo

❌ Evítalo si:

- necesitas interoperabilidad C sin fricción
- código ultra legacy

## 🧩 Comparación clara

|Feature|enum|enum class|
|-|-|-|
|Scope|❌ global|✅ scoped|
|Type safety|❌|✅|
|Conversión implícita|✅|❌|
|Uso moderno|❌|✅|

## 🎯 Regla práctica

- Usa enum class por defecto.
- Usa enum solo si tienes una razón fuerte.

[Regresar a temas de C++11](../cpp11.md)
