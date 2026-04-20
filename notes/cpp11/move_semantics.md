# Move semantics

## 🧠 El problema que resuelve

Antes de C++11:

```cpp
std::vector<int> a = {1,2,3};
std::vector<int> b = a;  // copia
```

- 👉 Se copia TODO el buffer
- 👉 Costoso (memoria + tiempo)

## 🔥 La idea clave

Hay objetos que **ya no necesitas** → puedes **robar sus recursos**

## 🧩 Ejemplo intuitivo

```cpp
std::vector<int> a = {1,2,3};
std::vector<int> b = std::move(a);
```

- 👉 No copia
- 👉 Transfiere el buffer interno

Después:

```cpp
a  // válido pero en estado “vacío” o indefinido útil
b  // tiene los datos
```

## ⚙️ ¿Qué es `std::move` realmente?

No mueve nada.

👉 Solo hace esto:

> “Trata este objeto como temporal (rvalue)”

## 🧠 Concepto clave: lvalue vs rvalue

### 🔹 lvalue (tiene nombre)

```cpp
int x = 10;
```

👉 `x` vive, puedes usarlo después

### 🔹 rvalue (temporal)

```cpp
int x = 10 + 20;
```

👉 10 + 20 es temporal

## 🔥 Nueva pieza: rvalue reference (&&)

```cpp
int&& r = 10;
```

👉 Solo puede bindear a temporales

## 🚀 Move constructor (el corazón)

Ejemplo simplificado:

```cpp
class Buffer {
    int* data;
    size_t size;

public:
    Buffer(size_t s) : size(s), data(new int[s]) {}

    // COPY
    Buffer(const Buffer& other) {
        size = other.size;
        data = new int[size];
        std::copy(other.data, other.data + size, data);
    }

    // MOVE 🔥
    Buffer(Buffer&& other) {
        data = other.data;
        size = other.size;

        other.data = nullptr;
        other.size = 0;
    }
};
```

- 👉 Copy = duplicar
- 👉 Move = transferir#

## ⚠️ Sin move semantics

```cpp
Buffer makeBuffer() {
    Buffer b(1000);
    return b;
}
```

Antes:

- copia enorme

Con move:

- se transfiere

## 🔥 Ejemplo práctico (muy claro)

```cpp
std::vector<int> create() {
    std::vector<int> v = {1,2,3};
    return v;
}

auto x = create();  // move, no copia
```

👉 Esto es lo que hace viable C++ moderno

## ⚠️ Estado después de mover

```cpp
std::vector<int> a = {1,2,3};
auto b = std::move(a);
```

👉 `a` queda:

- válido
- pero NO confiable en contenido

✔️ Puedes:

```cpp
a.clear();
a.push_back(1);
```

❌ No asumas que aún tiene datos

## 🧩 Regla clave

> Después de `std::move`, el objeto queda en estado válido pero indefinido

## ⚙️ En sistemas / embebidos (esto te importa)

### 🟢 Ventajas reales

- ✔️ Evitas copias grandes
- ✔️ Control fino de memoria
- ✔️ Mejor performance

### 🔴 Cuidado

❌ Mover sin querer

```cpp
foo(std::move(x));
```

👉 Ya no puedes usar `x` igual

### ❌ Mover objetos que no deben moverse

Ej:

- registros hardware
- objetos con ownership compartido mal definido

## 🔥 Caso típico importante

### Sin move

```cpp
void setData(std::vector<int> v);
```

👉 copia

### Con move

```cpp
void setData(std::vector<int>&& v) {
    data = std::move(v);
}
```

### Mejor diseño (moderno)

```cpp
void setData(std::vector<int> v) {
    data = std::move(v);
}
```

👉 acepta:

- copia (si le pasas lvalue)
- move (si le pasas rvalue)

## 🧠 Insight de alto nivel

Move semantics =

> “Separar copia de transferencia de ownership”

## 🎯 Resumen claro

- `std::move` → convierte a rvalue
- `&&` → permite capturar temporales
- move constructor → transfiere recursos
- evita copias innecesarias

## 🧠 Regla mental

> “Si ya no voy a usar el objeto, muévelo”

## 🔥 Caso 1: ejemplo

```cpp
Buffer makeBuffer() {
    Buffer b(1000);
    return b;
}
```

### ¿Qué pasa aquí REALMENTE?

#### 🟢 Caso moderno (lo más común hoy):

👉 RVO / NRVO

- El compilador construye `b` directamente en el lugar de retorno
- **No hay copy**
- **No hay move**

#### 🟡 Si NO hay RVO (por alguna razón)

Entonces sí:

1. Se intenta **move constructor**
2. Si no existe → **copy constructor**

## 🧠 Orden real del compilador

Cuando haces:

```cpp
return b;
```

El compilador intenta:

1. **RVO (ideal)** ✅
2. Si no → **move** (si existe) 🔥
3. Si no → **copy** 😐

## 🔥 Caso 2: std::vector

```cpp
std::vector<int> create() {
    std::vector<int> v = {1,2,3};
    return v;
}

auto x = create();
```

### ¿Qué pasa aquí?

#### 🟢 En compiladores modernos:

👉 Guaranteed copy elision (C++17+)

- `v` se construye directamente en `x`
- **NO copy**
- **NO move**

## ⚠️ Entonces… ¿dónde entra el move?

Buena pregunta. Aquí es donde SÍ se ve claro:

## 🔥 Caso donde SÍ hay move

```cpp
std::vector<int> v = {1,2,3};
std::vector<int> x = std::move(v);
```

👉 Aquí sí:

- No hay RVO
- Forzaste rvalue
- → se llama move constructor

## 🔥 Otro caso claro

```cpp
void setData(std::vector<int> v);

std::vector<int> a = {1,2,3};
setData(a);              // copy
setData(std::move(a));   // move
```

👉 Aquí ves la diferencia directa

## 🧩 Entonces… ¿por qué mis ejemplos fueron confusos?

Porque:

👉 **Return-by-value ya no es un buen ejemplo para enseñar move**

Desde C++17:

- casi siempre hay **copy elision**
- move queda oculto

## 🧠 Insight importante (este sí es el bueno)

Move semantics no existe para optimizar `return`.

👉 Existe para optimizar:

- transferencia de ownership
- contenedores
- APIs

## 🎯 Resumen brutalmente claro

|Caso|¿Hay copy?|¿Hay move?|¿Qué pasa?|
|-|-|-|-|
|`return local` (C++17)|❌|❌|RVO|
|`return local` (sin RVO)|❌|✅|move|
|`std::move(x)`|❌|✅|move|
|pasar lvalue|✅|❌|copy|
|pasar rvalue||❌|✅|move|

## 🧠 Regla mental correcta

> “Move ocurre cuando el compilador ve un rvalue y no puede hacer elision”

## 🚀 Ahora sí, ejemplo donde LO VES claro

```cpp
#include <iostream>

struct A {
    A() { std::cout << "ctor\n"; }
    A(const A&) { std::cout << "copy\n"; }
    A(A&&) { std::cout << "move\n"; }
};

A create() {
    A a;
    return a;
}

int main() {
    A x = create();
}
```

👉 En C++17:

```cpp
ctor
```

👉 Sin RVO (forzado):

```cpp
ctor
move
```

## 🔥 Conclusión (la buena)

- No, no basta con tener move constructor
- No, tus ejemplos no garantizan move
- Sí, hoy en día muchas veces no hay ni copy ni move

👉 Move importa cuando:

- usas `std::move`
- trabajas con ownership explícito
- diseñás APIs
