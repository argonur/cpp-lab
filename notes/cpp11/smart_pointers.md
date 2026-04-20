# Smart pointers

## 🧠 Problema que resuelven los smart pointers

Antes:

```cpp
int* p = new int(10);
// ...
delete p;
```

👉 Problemas:

- leaks
- doble delete
- excepciones
- ownership poco claro

## 🔥 Idea central

> Un smart pointer es un objeto que **administra automáticamente la vida de un recurso**

## 🧩 Tipos principales

`std::unique_ptr` → un solo dueño
`std::shared_ptr` → múltiples dueños

## 🔥 1. `std::unique_ptr` (el más importante)

### 🧠 Modelo mental

> “Este objeto tiene un único dueño. Nadie más lo comparte.”

### 🔧 Ejemplo básico

```cpp
#include <memory>

std::unique_ptr<int> p = std::make_unique<int>(10);
```

👉 Cuando `p` muere → se libera memoria automáticamente

### 🔥 NO se puede copiar

```cpp
auto p2 = p;  // ❌ error
```

👉 Porque rompería la unicidad

### ✅ Se puede mover

```cpp
auto p2 = std::move(p);
```

👉 Aquí entra move semantics:

- `p2` ahora es dueño
- `p` queda vacío `(nullptr)`

### ⚙️ Esto es clave

`unique_ptr` **usa move semantics internamente**

👉 Sin `&&`, `unique_ptr` no existiría como lo conocemos

### 🔥 Ejemplo claro

```cpp
std::unique_ptr<int> create() {
    return std::make_unique<int>(42);
}

auto p = create();  // move (o elision)
```

### 🧩 Pasar `unique_ptr` a funciones

#### ❌ Mala idea (por valor sin move)

```cpp
void foo(std::unique_ptr<int> p);

foo(p);  // ❌
```

#### ✅ Correcto

```cpp
foo(std::move(p));
```

👉 Transferiste ownership

### 🟢 Alternativa (no transferir)

```cpp
void foo(const std::unique_ptr<int>& p);
```

👉 Solo observas, no eres dueño

## 🔥 2. `std::shared_ptr`

### 🧠 Modelo mental

> “Este recurso tiene múltiples dueños”

### 🔧 Ejemplo

```cpp
auto p1 = std::make_shared<int>(10);
auto p2 = p1;
```

- 👉 Ambos apuntan al mismo objeto
- 👉 Hay un contador de referencias

### 🔥 Cuando count llega a 0

👉 Se libera memoria

### ⚠️ Problema importante

#### ❌ Overhead

- contador atómico
- sincronización

👉 Más lento que `unique_ptr`

#### ❌ Ciclos (memory leaks)

```cpp
struct A {
    std::shared_ptr<A> other;
};
```

👉 Nunca se libera

#### ✅ Solución: `std::weak_ptr`

No es dueño, solo observa

## 🔥 Comparación directa

|Característica|`unique_ptr`|`shared_ptr`|
|-|-|-|
|Ownership|único|compartido|
|Copy|❌|✅|
|Move|✅|✅|
|Overhead|mínimo|mayor|
|Uso típico|default|casos especiales|

## 🧠 Conexión directa con move semantics

Esto es lo importante:

👉 `unique_ptr` fuerza el uso correcto de move

```cpp
auto p2 = std::move(p);
```

👉 Te obliga a pensar:

> “¿Quién es el dueño ahora?”

## ⚙️ En sistemas / embebidos (esto te interesa)

### 🟢 `unique_ptr` → SÍ

- ✔️ Cero overhead
- ✔️ Seguro
- ✔️ Determinista

👉 Muy buena opción incluso en sistemas

### 🔴 `shared_ptr` → cuidado

- overhead
- no determinista
- contadores atómicos

👉 En embedded:

- muchas veces se evita

### 🔥 Caso práctico (muy bueno)

Sin smart pointer

```cpp
Device* d = new Device();
init(d);
delete d;
```

Con `unique_ptr`

```cpp
auto d = std::make_unique<Device>();
init(d.get());
```

👉 `get()` → acceso sin transferir ownership

## 🧠 Insight clave (este es el que importa)

Smart pointers no son solo “punteros bonitos”.

👉 Son una forma de expresar:

> quién es responsable de destruir el objeto

## 🎯 Reglas prácticas

- Usa `unique_ptr` por defecto
- Usa `shared_ptr` solo si realmente necesitas ownership compartido
- Evita `new/delete` directo
- Usa `std::move` cuando transfieres ownership
