# `nullptr`

`nullptr` soluciona problemas reales que con NULL eran fuente de bugs sutiles (y en sistemas, esos bugs duelen).

## 🧠 ¿Qué es `nullptr`?

Es un valor especial para representar un puntero nulo, introducido en C++11.

```cpp
int* p = nullptr;
```

👉 Es de tipo: `std::nullptr_t`
👉 Es type-safe (esto es lo importante)

## ⚠️ El problema con NULL

Antes:

```cpp
int* p = NULL;
```

El problema es que `NULL` en realidad suele ser:

```cpp
#define NULL 0
```

👉 O sea… es un int, no un puntero.

## 🔥 Problema real: sobrecarga de funciones

```cpp
void foo(int);
void foo(char*);

foo(NULL);  // ¿cuál se llama?
```

👉 Puede llamar a `foo(int)` 😬

## ✅ Con `nullptr`

```cpp
foo(nullptr);  // llama a foo(char*)
```

👉 Porque `nullptr` solo puede convertirse a puntero, no a entero.

## ⚙️ Otro caso peligroso

```cpp
void func(int x);

func(NULL);     // válido 😬
func(nullptr);  // error de compilación ✅
```

👉 Esto evita bugs silenciosos.

## 🧩 Comparaciones

```cpp
if (p == nullptr) { ... }
```

👉 Mejor que:

```cpp
if (p == NULL)
if (p == 0)
```

Porque:

- es explícito
- es semánticamente correcto

## 🔬 Tipo real: std::nullptr_t

Puedes hacer esto:

```cpp
std::nullptr_t n = nullptr;
```

Y funciones específicas:

```cpp
void foo(std::nullptr_t);
```

👉 Esto era imposible con `NULL`.

## ⚙️ En sistemas embebidos (esto te interesa)

### 🟢 Ventajas reales

- ✔️ Evita ambigüedades en APIs críticas
- ✔️ Mejora legibilidad en código de bajo nivel
- ✔️ Cero costo en runtime

👉 Es literalmente gratis usarlo.

### 🔴 ¿Alguna desventaja?

Ninguna relevante.

Solo:

- toolchains muy viejos pueden no soportarlo (hoy raro)

## 🚫 Cosas que debes evitar

### ❌ Mezclar `NULL` y `nullptr`

```cpp
int* p1 = NULL;
int* p2 = nullptr;
```

👉 Mantén consistencia: usa solo `nullptr`.

### ❌ Usar 0 como puntero nulo

```cpp
int* p = 0;  // estilo C viejo
```

👉 Esto ya no es aceptable en código moderno.

## 🎯 Regla práctica

👉 En C++ moderno:

> Siempre usa `nullptr`. Nunca uses `NULL`.

Sin excepciones.

## 🧠 Insight importante

Esto no es solo sintaxis bonita.

`nullptr`:

- elimina ambigüedad
- hace el código más seguro
- evita bugs en overload resolution

👉 Es exactamente el tipo de mejora que C++11 introdujo:
**más seguridad sin costo de performance**

## 🔧 Ejemplo real (estilo sistema)

```cpp
void init(Device* dev) {
    if (dev == nullptr) {
        return;
    }

    dev->start();
}
```

Simple, claro, sin ambigüedad.

[Regresar a temas de C++11](../cpp11.md)