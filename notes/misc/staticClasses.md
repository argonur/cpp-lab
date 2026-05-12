# Static Classes

Aquí hay una trampa importante:

> En C++ **no existen “static classes” como en C#**.

No puedes hacer esto:

```cpp
static class Utils {  // ❌ NO existe en C++
};
```

Entonces, ¿qué se hace en C++?

Se simula el comportamiento usando:

- miembros `static`
- constructores privados
- namespaces

## 1. Patrón típico: clase utilitaria

```cpp
class Math {
public:
    static int add(int a, int b) {
        return a + b;
    }
};
```

Uso:

```cpp
Math::add(2, 3);
```

### Pero todavía puedes crear objetos

```cpp
Math m; // válido
```

Aunque no tenga sentido.

### Cómo impedir instanciación

#### Constructor privado o borrado

```cpp
class Math {
public:
    static int add(int a, int b) {
        return a + b;
    }

private:
    Math() = delete;
};
```

Ahora:

```cpp
Math m; // ❌ error
```

**Esto es lo más cercano a una “static class” en C++**.

## 2. En C++ moderno muchas veces prefieres `namespace`

Porque si no necesitas estado:

```cpp
namespace Math {
    int add(int a, int b) {
        return a + b;
    }
}
```

Uso:

```cpp
Math::add(2, 3);
```

### ¿Por qué `namespace` suele ser mejor?

Porque:

- expresa intención claramente
- no crea la ilusión de objeto
- evita boilerplate inútil

### Entonces, ¿cuándo usar clase?

Usa clase si necesitas:

- encapsulación OOP
- herencia
- templates complejos
- acceso privado/protegido
- interfaces

Usa namespace si:

- solo agrupas funciones/utilidades

## En embedded sí aparecen “clases estáticas” simuladas

Ejemplo típico:

```cpp
class UART {
public:
    static void init();
    static void send(uint8_t byte);

private:
    UART() = delete;
};
```

Porque:

- solo existe un periférico físico
- no quieres múltiples instancias

### Pero cuidado (esto es importante)

Muchos sistemas embedded viejos:

- abusan de `static`
- terminan pareciendo C disfrazado de C++

## Diferencia importante con C#

En C#:

- el lenguaje impide instanciación
- todos los miembros deben ser static

En C++:

- no existe esa keyword
- es un patrón, no una característica del lenguaje

## Respuesta corta nivel entrevista

> “C++ no tiene `static classes` como C#. El comportamiento se simula usando clases con solo miembros `static` y constructores privados o eliminados. En muchos casos, un `namespace` es una mejor alternativa.”
