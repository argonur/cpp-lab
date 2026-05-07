# Storage duration

`static` tiene **dos efectos distintos** dependiendo de dónde se use:

1. **Linkage** (visibilidad entre archivos)
2. **Storage duration** (cuánto vive el objeto)

La parte de *storage duration* es la importante aquí.

## ¿Qué significa “storage duration”?

Es:

> Durante cuánto tiempo existe el objeto en memoria

## Hay varias storage durations en C++

|Tipo|Vive cuándo|
|-|-|
|automatic|durante el scope|
|static|durante toda la ejecución|
|dynamic|hasta `delete/free`|
|thread|mientras viva el hilo|

### 1. Variable normal dentro de función (`automatic storage duration`)

```cpp
void foo() {
    int x = 0;
}
```

Aquí:

- `x` se crea al entrar a la función
- `x` se destruye al salir

Cada llamada crea una nueva instancia.

### 2. Variable static dentro de función (`static storage duration`)

```cpp
void foo() {
    static int x = 0;
    x++;
}
```

Aquí cambia completamente.

👉 `x`:

- Se crea UNA sola vez
- Vive hasta que termina el programa
- Conserva su valor entre llamadas

#### Ejemplo

```cpp
foo(); // x = 1
foo(); // x = 2
foo(); // x = 3
```

Porque:

- No se destruye al salir de la función

## Importante: scope vs lifetime

Muchos los confunden.

### Scope

Dónde puedes acceder a la variable.

### Lifetime

Cuánto tiempo existe en memoria.

---

En este caso:

```cpp
void foo() {
    static int x = 0;
}
```

- Scope:
  - solo dentro de `foo`
- Lifetime:
  - toda la ejecución del programa

## ¿Dónde se almacena?

Normalmente:

- `.data`
- `.bss`

NO en el stack.

Eso es importante en embedded.

### En embedded esto importa muchísimo

Porque:

- Stack suele ser pequeño
- Variables `static` no consumen stack dinámico

#### Ejemplo típico embedded

```cpp
void task() {
    static uint8_t buffer[1024];
}
```

👉 Ese buffer:

- NO está en stack
- Vive permanentemente

## Riesgo importante

Muchos usan static “para que no se reinicie”…

Y terminan creando:

- estado oculto
- funciones no reentrantes
- bugs de concurrencia

## Pregunta típica de entrevista

> “¿Cuál es la diferencia entre una variable local normal y una local static?”

Respuesta sólida:

> “Una variable local normal tiene automatic storage duration y se crea/destruye en cada llamada. Una variable local `static` tiene static storage duration: se inicializa una sola vez y conserva su valor durante toda la ejecución del programa.”

## Otra cosa MUY importante

La inicialización de `static local` en C++ moderno es thread-safe.

```cpp
void foo() {
    static MyClass obj;
}
```

Desde C++11:

- solo un hilo inicializa el objeto

Eso es pregunta de nivel senior a veces.

## Resumen limpio

`static` puede afectar:

|Contexto|Efecto|
|-|-|
|global/file scope|linkage|
|local variable|storage duration|
|class member|shared entre instancias|

Y aquí el punto clave:

> “Static storage duration” significa que el objeto existe durante toda la vida del programa, independientemente del scope donde fue declarado.
