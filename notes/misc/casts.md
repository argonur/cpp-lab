# Casts modernos

Desde C++ (a partir de C++98) existen 4 casts “modernos” que reemplazan en gran parte al cast estilo C:

```cpp
(tipo)variable
```

Los 4 son:

- `static_cast`
- `dynamic_cast`
- `const_cast`
- `reinterpret_cast`

Y honestamente, aprender bien cuándo usar cada uno sí marca una diferencia fuerte entre código C++ serio y código “tipo C con clases”.

## 1. `static_cast`

Es el cast “normal” y el más usado.

Sirve para conversiones que el compilador entiende y considera razonables.

### Ejemplos

#### Conversión numérica

```cpp
double x = 3.14;
int y = static_cast<int>(x);
```

#### Upcasting en herencia

```cpp
class Base {};
class Derived : public Base {};

Derived d;

Base* b = static_cast<Base*>(&d);
```

Esto es seguro.

#### Downcasting (PELIGROSO si te equivocas)

```cpp
Base* b = new Derived();

Derived* d = static_cast<Derived*>(b);
```

Esto compila.

Pero si b NO apunta realmente a un Derived, tienes undefined behavior.

### Cuándo usar `static_cast`

Cuando:

la conversión es lógica
el compilador puede verificarla parcialmente
no necesitas chequeo runtime

Es el cast que deberías usar el 90% de las veces.

## 2. `dynamic_cast`

Este hace casts seguros en jerarquías polimórficas.

Usa RTTI (Run-Time Type Information).

Sirve principalmente para downcasting seguro.

### Requisito IMPORTANTÍSIMO

La clase base debe ser polimórfica.

O sea: debe tener al menos una función virtual.

```cpp
class Base {
public:
    virtual ~Base() = default;
};
```

### Ejemplo seguro

```cpp
class Base {
public:
    virtual ~Base() = default;
};

class Derived : public Base {};

Base* b = new Derived();

Derived* d = dynamic_cast<Derived*>(b);

if (d)
{
    std::cout << "cast correcto";
}
```

### Si el cast falla

Con punteros:

```cpp
Derived* d = dynamic_cast<Derived*>(b);
```

regresa:

```cpp
nullptr
```

Con referencias:

```cpp
Derived& d = dynamic_cast<Derived&>(*b);
```

lanza excepción:

```cpp
std::bad_cast
```

### Cuándo usar `dynamic_cast`

Cuando:

- haces downcasting
- NO estás seguro del tipo real
- quieres seguridad runtime

## 3. `const_cast`

Sirve únicamente para agregar o quitar `const` o `volatile`.

Nada más.

### Ejemplo

```cpp
const int x = 5;

int* p = const_cast<int*>(&x);
```

### PERO OJO

Modificar un objeto originalmente `const` es UB.

Esto es UB:

```cpp
const int x = 5;

int* p = const_cast<int*>(&x);

*p = 10; // UB
```

### Uso válido típico

Cuando una API vieja no usa `const` correctamente.

```cpp
void legacy_function(char* str);

const char* text = "hola";

legacy_function(const_cast<char*>(text));
```

(Aunque incluso esto puede ser peligroso.)

## 4. `reinterpret_cast`

El cast nuclear.

Literalmente reinterpretar bits como otro tipo.

Casi siempre es bajo nivel.

### Ejemplo

```cpp
int x = 65;

char* p = reinterpret_cast<char*>(&x);
```

### Otro ejemplo

```cpp
uintptr_t addr = reinterpret_cast<uintptr_t>(ptr);
```

Muy usado en:

- drivers
- embedded
- networking
- serialización
- hacking
- sistemas operativos
- allocators
- interfacing con hardware

### Este cast es MUY peligroso

El compilador casi no te protege.

Puedes romper:

- alignment
- aliasing rules
- object lifetime
- strict aliasing
- memory safety

### Regla práctica MUY útil

`static_cast`

“Confío en la conversión.”

`dynamic_cast`

“Necesito verificar el tipo real.”

`const_cast`

“Solo quiero cambiar constness.”

`reinterpret_cast`

“Estoy haciendo magia negra.”

## Comparación rápida

|Cast|Runtime check|Seguro|Uso típico|
|-|-|-|-|
|`static_cast`|No|Medio|Conversiones normales|
|`dynamic_cast`|Sí|Alto|Herencia polimórfica|
|`const_cast`|No|Bajo|Quitar/agregar const|
|`reinterpret_cast`|No|Muy bajo|Bajo nivel|

## ¿Y el cast estilo C?

Esto:

```cpp
(int)x
```

o:

```cpp
(MyType*)ptr
```

es básicamente una mezcla peligrosa de TODOS los casts.

El compilador intenta:

- `const_cast`
- `static_cast`
- `reinterpret_cast`

en cierto orden.

Por eso en C++ moderno normalmente se evita.

Porque no deja claro qué intención tienes.

## Algo MUY importante para entrevistas y código profesional

Si alguien usa:

```cpp
(Type)x
```

en C++ moderno…

muchas veces es red flag.

Porque no sabes:

- si está quitando const
- reinterpretando memoria
- haciendo downcast inseguro
- etc.

Los casts modernos hacen explícita la intención.

Y eso importa muchísimo en sistemas complejos.
