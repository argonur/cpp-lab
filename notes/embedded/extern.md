# `extern`

`extern` es simple en apariencia, pero en entrevistas lo usan para ver si realmente entiendes **cómo se conecta tu programa entre archivos**.

## Qué es `extern` (idea clara)

> `extern` le dice al compilador:  
> 👉 **“Esta variable o función está definida en otro archivo.”**

No crea nada. Solo **declara**.

## Ejemplo básico (el patrón correcto)

### Header (`.h`)

```cpp
#pragma once

extern int counter;
```

### Archivo A (`.cpp`)

```cpp
int counter = 0;  // definición real
```

### Archivo B (`.cpp`)

```cpp
#include "counter.h"

void foo() {
    counter++;
}
```

## Regla de oro (esto te lo pueden preguntar directo)

> Debe existir exactamente UNA definición, pero puede haber muchas declaraciones `extern`.

Si defines más de una vez:  
💥 error de linker

### Qué pasa si NO usas `extern`

Si haces esto en un header:

```cpp
int counter = 0;  // ❌
```

Y lo incluyes en varios `.cpp`:

💥 múltiples definiciones → linker explota

## `extern` con funciones

Por defecto, las funciones ya son `extern`:

```cpp
void foo();  // ya es extern implícito
```

Esto es equivalente a:

```cpp
extern void foo();
```

👉 Por eso casi nunca ves `extern` en funciones

## `extern` + `volatile` (muy típico en embedded)

```cpp
// header
extern volatile bool flag;

// cpp
volatile bool flag = false;
```

👉 Aquí estás diciendo:

- La variable está en otro archivo
- Y puede cambiar externamente (ISR, hardware)

## `extern` vs `static` (esto es clave)

|Keyword|Visibilidad|
|-|-|
|`extern`|visible entre archivos|
|`static`|solo dentro del archivo|

👉 Son opuestos en intención

## Caso interesante: `extern` con inicialización

Esto **sí es válido** pero poco común:

```cpp
extern int x = 5;  // esto ES una definición
```

👉 Aunque diga `extern`, al inicializar:

- Se convierte en definición

💡 Esto es una trampa clásica de entrevista

## En embedded (cómo se usa en la vida real)

- Variables compartidas entre módulos
- Flags de ISR
- Buffers globales
- Drivers de hardware

Pero cuidado:

> Abusar de `extern` = sistema difícil de mantener

## Respuesta nivel entrevista (compacta)

Si te preguntan:

> ¿Qué hace `extern`?

Responde:

> “Declara una variable o función definida en otro archivo. Permite compartirla entre translation units sin generar múltiples definiciones. Debe existir exactamente una definición real en todo el programa.”
