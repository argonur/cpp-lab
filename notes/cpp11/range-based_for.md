# Range-based for

## 🧠 ¿Qué es range-based for?

Sintaxis introducida en C++11:

```cpp
for (declaración : contenedor) {
    // cuerpo
}
```

Ejemplo:

```cpp
std::vector<int> v = {1,2,3};

for (auto x : v) {
    // ...
}
```

👉 Es equivalente (simplificado) a:

```cpp
for (auto it = v.begin(); it != v.end(); ++it) {
    auto x = *it;
}
```

## 🔥 Lo importante: ¿copia o referencia?

Aquí está TODO el tema.

### ❌ Caso 1: por valor (copia)

```cpp
for (auto x : v) {
    x = 10;
}
```

- 👉 `x` es una copia
- 👉 NO modificas el contenedor
- 👉 Puede ser costoso si el objeto es grande

### ✅ Caso 2: por referencia

```cpp
for (auto& x : v) {
    x = 10;
}
```

- 👉 Modificas el contenedor
- 👉 No hay copias
- 👉 Más eficiente

### ✅ Caso 3: referencia constante (el más usado)

```cpp
for (const auto& x : v) {
    // solo lectura
}
```

- 👉 Sin copias
- 👉 Seguro
- 👉 Ideal por defecto

## ⚙️ Regla de oro (memorízala)

- `auto` → copia
- `auto&` → modificar
- `const auto&` → leer eficiente

👉 Si dudas: usa `const auto&`

## ⚠️ Errores reales (muy comunes)

### ❌ Bug silencioso de performance

```cpp
std::vector<BigObject> v;

for (auto x : v) {
    process(x);
}
```

👉 Estás copiando cada `BigObject`

✔️ Correcto:

```cpp
for (const auto& x : v) {
    process(x);
}
```

### ❌ Crees que modificas, pero no

```cpp
for (auto x : v) {
    x.update();
}
```

👉 Modificas la copia, no el original 😬

### ❌ Con punteros

```cpp
std::vector<int*> v;

for (auto p : v) {
    *p = 10;  // OK (copias el puntero, no el dato)
}
```

👉 Aquí copiar es barato (solo puntero)

## 🧩 Cómo funciona internamente (nivel sistema)

El compilador transforma:

```cpp
for (auto& x : v)
```

en algo tipo:

```cpp
auto&& __range = v;
for (auto __it = __range.begin(); __it != __range.end(); ++__it) {
    auto& x = *__it;
}
```

- 👉 No hay magia runtime
- 👉 Todo es compile-time
- 👉 Costo = loop normal

## ⚙️ En sistemas embebidos

Aquí es donde debes ser disciplinado.

## 🟢 Cuándo SÍ usarlo

✔️ Buffers

```cpp
for (auto& byte : buffer)
```

✔️ Arrays

```cpp
for (auto& val : data)
```

✔️ STL ligera (`std::array`)

## 🔴 Cuándo tener cuidado

❌ Acceso a hardware

```cpp
for (auto reg : registers)  // ¿qué es reg?
```

👉 Problema:

- ¿es copia?
- ¿es volatile?
- ¿es referencia?

✔️ Mejor explícito:

```cpp
for (volatile uint32_t& reg : registers)
```

## ❌ Tipos críticos

Si el tipo importa (ej: fixed width):

```cpp
for (auto x : sensorData)
```

👉 Podrías perder:

- tamaño exacto
- signedness

## 🔥 Casos útiles avanzados

1. Arrays clásicos (sin STL)

```cpp
int arr[5] = {1,2,3,4,5};

for (auto& x : arr) {
    x *= 2;
}
```

👉 Funciona perfecto

2. Evitar errores de índices

Antes:

```cpp
for (size_t i = 0; i < v.size(); ++i)
```

Ahora:

```cpp
for (const auto& x : v)
```

👉 Menos bugs

3. Con structs

```cpp
struct Data {
    int a;
    float b;
};

std::vector<Data> v;

for (auto& d : v) {
    d.a = 0;
}
```

## 🎯 Conclusión directa

Range-based for es:

- ✔ Más limpio
- ✔ Igual de eficiente
- ✔ Menos propenso a errores

PERO…

👉 Si usas `auto` mal → introduces:

- copias innecesarias
- bugs silenciosos

## 🧠 Regla mental (quédate con esto)

“Si no estoy 100% seguro, uso `const auto&`”
