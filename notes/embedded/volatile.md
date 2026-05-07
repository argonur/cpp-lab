# `volatile`

`volatile` es una de esas cosas que parecen simples… pero en embedded es **crítica**. Si no la entiendes bien, puedes tener bugs invisibles.

## Qué es `volatile` (en esencia)

`volatile` le dice al compilador:

> “Esta variable puede cambiar en cualquier momento **fuera del control del programa**. No optimices su acceso.”

Normalmente, el compilador intenta optimizar:

- Guarda variables en registros
- Evita leer memoria si “cree” que no cambió

Con **volatile**, le estás diciendo:

👉 **“No asumas nada. Cada acceso debe ir realmente a memoria.”**

## Ejemplo simple (donde todo se rompe)

```cpp
bool flag = false;

while (!flag) {
    // esperar
}
```

El compilador podría optimizar esto como:

- Leer **flag** una vez
- Como “nadie lo cambia”, asumir que siempre es **false**

👉 Resultado: loop infinito.

Ahora con **volatile**:

```cpp
volatile bool flag = false;

while (!flag) {
}
```

Ahora el compilador:

- Lee `flag` **cada vez desde memoria**

## Por qué es clave en embedded

En sistemas embebidos, hay cosas que modifican memoria **sin que tu código lo sepa directamente:**

### 1. Registros de hardware (lo más importante)

```cpp
#define UART_STATUS (*(volatile uint32_t*)0x40001000)

while (!(UART_STATUS & 0x01)) {
    // esperar dato
}
```

El hardware cambia ese registro.

Sin `volatile`:

- El compilador podría leerlo una sola vez → bug

Con `volatile`:

- Se lee cada vez → correcto

### 2. Variables compartidas con ISR

```cpp
volatile bool interruptFlag = false;

void ISR() {
    interruptFlag = true;
}
```

Código principal:

```cpp
while (!interruptFlag) {
}
```

Sin `volatile`:

- El compilador puede ignorar cambios hechos por la ISR

### 3. Memoria mapeada (memory-mapped I/O)

Todo acceso a periféricos:

- GPIO
- Timers
- ADC
- UART

👉 Siempre requiere `volatile`

## Lo que `volatile` NO hace

Esto es clave para entrevista:

👉 `volatile` **NO hace tu código thread-safe**

NO garantiza:

- Exclusión mutua
- Atomicidad
- Orden entre hilos

Ejemplo peligroso:

```cpp
volatile int x = 0;

x++;  // NO es atómico
```

Esto sigue siendo un problema en concurrencia.

## Entonces… ¿qué sí hace?

- ✔ Evita optimizaciones peligrosas
- ✔ Fuerza acceso real a memoria
- ✔ Garantiza “visibilidad” a nivel compilador (no CPU)

## Error común que debes evitar decir en entrevista

Muchos dicen:

> "`volatile` sirve para multithreading"

❌ Incorrecto

La respuesta correcta:

> “Sirve para evitar optimizaciones del compilador cuando el valor puede cambiar externamente, como en hardware o ISR. No sustituye mecanismos de sincronización.”

## Regla práctica

Usa `volatile` cuando:

- Lees/escribes registros de hardware
- Variables compartidas con ISR
- Flags que cambian fuera del flujo normal

## Declaración, definición y uso

> `volatile` **no cambia las reglas de linkage ni visibilidad entre archivos.**
Se maneja igual que cualquier variable global.

### Caso correcto (variable global compartida)

#### Header (`.h`)

Declaras con `extern`:

```cpp
#pragma once

extern volatile bool flag;
```

#### Archivo 1 (`.cpp`)

Defines la variable:

```cpp
volatile bool flag = false;
```

#### Archivo 2 (`.cpp`)

La usas:

```cpp
#include "flag.h"

void foo() {
    if (flag) {
        // hacer algo
    }
}
```

### Punto clave

👉 El `volatile` debe estar **tanto en la declaración como en la definición**

Si haces esto:

```cpp
extern bool flag;   // ❌ sin volatile
```

pero en otro archivo:

```cpp
volatile bool flag = false;
```

💥 Problema:

- El compilador ve tipos distintos
- Puedes tener comportamiento indefinido o warnings sutiles

#### Qué está pasando realmente

- `extern` → dice “la variable existe en otro lado”
- `volatile` → es parte del tipo

👉 Entonces:

```cpp
volatile bool != bool
```

### Error común en embedded

Definir la variable directamente en el header:

```cpp
// ❌ MAL
volatile bool flag = false;
```

Si incluyes ese header en varios `.cpp`:

- 💥 múltiples definiciones → error de linker

### Caso especial: registros de hardware

Aquí cambia un poco el patrón.

Puedes ver algo así en headers:

```cpp
#define UART_STATUS (*(volatile uint32_t*)0x40001000)
```

O:

```cpp
static volatile uint32_t* const UART_STATUS = (uint32_t*)0x40001000;
```

Aquí:

- No hay `extern`
- Cada `.cpp` tiene su propia “vista” del registro
- Está bien porque es una dirección fija de hardware

### Resumen

> “`volatile` es parte del tipo, así que debe estar presente tanto en la declaración extern en el header como en la definición en el `.cpp`. Se maneja igual que cualquier variable global; `volatile` no afecta el linkage, solo evita optimizaciones del compilador.”
