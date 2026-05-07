# `static`

`static` fuera de una clase no solo existe, es muy usado en C/C++, especialmente en embedded.

Pero aquí está la clave:

> - Fuera de una clase, `static` **no tiene nada que ver con “compartido entre objetos”.*  
👉 Controla el **linkage (visibilidad entre archivos).**

## 1. `static` en variables globales (archivo `.cpp`)

```cpp
static int counter = 0;
```

Esto significa:

👉 La variable es **global**, pero **solo visible dentro de ese archivo `.cpp`**

- No se puede acceder desde otros `.cpp`
- No se puede usar con `extern`

## ¿Para qué sirve esto?

Encapsulación a nivel archivo:

- Evitar colisiones de nombres
- Ocultar implementación interna
- Hacer tu módulo más limpio

💡 En embedded esto es buena práctica.

## 2. Comparación directa (esto te conviene memorizar)

### Sin `static`

```cpp
int counter = 0;
```

👉 Tiene **external linkage**

- Visible desde otros `.cpp` con `extern`

### Con `static`

```cpp
static int counter = 0;
```

👉 Tiene **internal linkage**

- Solo visible dentro del archivo

## 3. `static` en funciones (fuera de clase)

También existe:

```cpp
static void helper() {
    // función interna
}
```

👉 Solo se puede usar dentro de ese `.cpp`

Esto es muy común para:

- Funciones auxiliares
- Lógica interna del módulo

## 4. `static` dentro de funciones (otro significado)

Esto ya es otro uso completamente distinto:

```cpp
void foo() {
    static int x = 0;
    x++;
}
```

👉 Aquí significa:

- La variable **se inicializa una sola vez**
- Persiste entre llamadas

No es global, pero tampoco “se reinicia”

## 5. Resumen claro (nivel entrevista)

Te pueden preguntar esto tal cual:

> ¿Qué hace `static` fuera de una clase?

Respuesta sólida:

> “A nivel global, `static` le da *internal linkage*, lo que limita la visibilidad de variables o funciones al archivo donde se definen. No afecta su tiempo de vida, solo su alcance a nivel de linker.”

## 6. Error común (evítalo)

Muchos dicen:

> “`static` hace que la variable sea global”

❌ Incorrecto

- Ya era global
- `static` la hace **menos accesible**, no más

## 7. En embedded (esto te suma puntos)

Usar `static` en `.cpp` es buena práctica para:

- Evitar interferencia entre módulos
- Reducir errores difíciles de debuggear
- Mantener encapsulación sin clases

## Te empujo un poco más (vale la pena)

Si quieres destacar, conecta esto con C++ moderno:

- `static` (C-style) vs `anonymous namespace`

```cpp
namespace {
    int counter = 0;
}
```

👉 Hace lo mismo que `static`, pero es preferido en C++
