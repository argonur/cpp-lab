# Construcción y destrucción de miembros en C++

Este tema es MUY importante en C++, porque aquí se define realmente el ciclo de vida interno de un objeto. Muchísimos bugs complejos salen de no entender bien esto.

Voy a explicártelo como realmente funciona el compilador, no como lo simplifican normalmente.

## Idea fundamental

Cuando creas un objeto en C++, **no se ejecuta mágicamente el constructor completo de golpe**.

La construcción ocurre en etapas muy precisas:

1. Construcción de clases base
2. Construcción de variables miembro
3. Ejecución del cuerpo del constructor

Y al destruir:

1. Ejecución del cuerpo del destructor
2. Destrucción de variables miembro
3. Destrucción de clases base

## Orden REAL de construcción

Supongamos esto:

```cpp
class Engine
{
public:
    Engine()
    {
        std::cout << "Engine construido\n";
    }

    ~Engine()
    {
        std::cout << "Engine destruido\n";
    }
};

class Wheels
{
public:
    Wheels()
    {
        std::cout << "Wheels construidas\n";
    }

    ~Wheels()
    {
        std::cout << "Wheels destruidas\n";
    }
};

class Car
{
private:
    Engine engine;
    Wheels wheels;

public:
    Car()
    {
        std::cout << "Car construido\n";
    }

    ~Car()
    {
        std::cout << "Car destruido\n";
    }
};
```

Uso:

```cpp
int main()
{
    Car car;
}
```

Salida:

```cpp
Engine construido
Wheels construidas
Car construido
Car destruido
Wheels destruidas
Engine destruido
```

## REGLA CRÍTICA

> **Los miembros se construyen en el ORDEN EN QUE SON DECLARADOS EN LA CLASE**

NO importa el orden en el initializer list.

Esto es importantísimo.

### Ejemplo tramposo

```cpp
class Test
{
private:
    B b;
    A a;

public:
    Test()
        : a(),
          b()
    {
    }
};
```

Mucha gente cree que:

1. se construye `a`
2. luego `b`

PERO NO.

El orden REAL es:

```cpp
B b;
A a;
```

Entonces:

1. se construye b
2. luego a

SIEMPRE.

### ¿Por qué existe esta regla?

Porque si el orden dependiera del initializer list:

```cpp
Test() : a(), b()
```

y otro constructor hiciera:

```cpp
Test(int x) : b(), a()
```

Entonces el orden de memoria del objeto cambiaría dependiendo del constructor.

Eso sería una pesadilla para el compilador.

Por eso C++ define:

> El orden de construcción depende EXCLUSIVAMENTE del orden de declaración en la clase.

### Esto genera warnings IMPORTANTES

Ejemplo:

```cpp
class Example
{
private:
    int x;
    int y;

public:
    Example()
        : y(10),
          x(y)
    {
    }
};
```

Muchos creen que:

```cpp
y = 10
x = y
```

Pero NO.

El orden real:

```cpp
int x;
int y;
```

Entonces:

1. `x` se construye PRIMERO
2. `y` después

Cuando `x(y)` corre, `y` todavía NO existe correctamente.

Resultado: **Undefined Behavior**

Y compiladores serios te lanzan warnings como:

```cpp
warning: field 'y' will be initialized after field 'x'
```

## Regla práctica PROFESIONAL

**SIEMPRE escribe el initializer list en el mismo orden de declaración**

BIEN:

```cpp
class Example
{
private:
    int x;
    int y;

public:
    Example()
        : x(0),
          y(10)
    {
    }
};
```

MAL:

```cpp
Example()
    : y(10),
      x(0)
{
}
```

Aunque funcione.

Porque:

- genera warnings
- confunde
- puede esconder bugs reales

En empresas grandes esto normalmente se considera mala práctica.

## Orden de destrucción

La destrucción ocurre EXACTAMENTE al revés.

### Regla

> Los miembros se destruyen en orden inverso al de construcción.

Ejemplo:

```cpp
class Example
{
private:
    A a;
    B b;
    C c;
};
```

Construcción:

```cpp
a
b
c
```

Destrucción:

```cpp
c
b
a
```

#### ¿Por qué al revés?

Porque muchos objetos dependen de otros.

Ejemplo:

```cpp
class Logger
{
};

class Database
{
private:
    Logger& logger;
};
```

Si `Database` usa `Logger`, entonces:

```cpp
Logger logger;
Database db;
```

`logger` debe existir ANTES que `db`.

Y destruirse DESPUÉS.

El orden inverso garantiza eso automáticamente.

## Inicialización vs asignación

Este punto es IMPORTANTÍSIMO.

Muchos principiantes creen que esto:

```cpp
class Example
{
private:
    std::string name;

public:
    Example()
    {
        name = "Arturo";
    }
};
```

"construye" `name`.

NO.

Lo que pasa realmente:

1. `name` se construye primero usando constructor default
2. luego se ejecuta:

```cpp
name.operator=("Arturo");
```

O sea:

- construcción
- luego asignación

DOS operaciones.

### Forma correcta

Example()
    : name("Arturo")
{
}

Aquí:

> name se construye DIRECTAMENTE con "Arturo"

Una sola operación.

Más eficiente.

### Casos donde DEBES usar initializer list

Hay miembros que NO puedes asignar después.

#### Referencias

```cpp
class Test
{
private:
    int& ref;

public:
    Test(int& r)
        : ref(r)
    {
    }
};
```

Las referencias deben inicializarse inmediatamente.

#### Const members

```cpp
class Test
{
private:
    const int value;

public:
    Test()
        : value(10)
    {
    }
};
```

No puedes hacer:

```cpp
value = 10;
```

Porque ya sería modificación.

#### Objetos sin constructor default

```cpp
class Engine
{
public:
    Engine(int hp)
    {
    }
};

class Car
{
private:
    Engine engine;

public:
    Car()
        : engine(500)
    {
    }
};
```

Si no haces eso:

```cpp
Engine engine;
```

el compilador intentará usar:

```cpp
Engine()
```

pero no existe.

## Orden completo con herencia

Ahora juntamos TODO.

```cpp
class Base
{
public:
    Base()
    {
        std::cout << "Base\n";
    }

    ~Base()
    {
        std::cout << "~Base\n";
    }
};

class Member
{
public:
    Member()
    {
        std::cout << "Member\n";
    }

    ~Member()
    {
        std::cout << "~Member\n";
    }
};

class Derived : public Base
{
private:
    Member member;

public:
    Derived()
    {
        std::cout << "Derived\n";
    }

    ~Derived()
    {
        std::cout << "~Derived\n";
    }
};
```

Salida:

```cpp
Base
Member
Derived
~Derived
~Member
~Base
```

### Orden REAL COMPLETO

#### Construcción

1. Clases base
2. Miembros
3. Constructor derivado

#### Destrucción

1. Destructor derivado
2. Miembros
3. Clases base

### Algo MUY importante: el cuerpo del constructor ocurre AL FINAL

Muchos creen esto:

```cpp
Car()
{
}
```

es donde "nace" el objeto.

NO.

Cuando entras al cuerpo:

```cpp
{
}
```

todos los miembros YA EXISTEN.

Todos.

El cuerpo del constructor es simplemente lógica adicional.

#### Error clásico peligrosísimo

```cpp
class Test
{
private:
    std::mutex mtx;
    std::lock_guard<std::mutex> lock;

public:
    Test()
        : lock(mtx)
    {
    }
};
```

Esto funciona SOLO si:

```cpp
std::mutex mtx;
```

está declarado ANTES que `lock`.

Si inviertes el orden:

```cpp
std::lock_guard<std::mutex> lock;
std::mutex mtx;
```

Entonces:

1. `lock` intenta construirse primero
2. usa un mutex que todavía no existe

Resultado:

- Undefined Behavior
- crashes rarísimos

Este tipo de bugs sí pasan en producción.

## Regla de oro profesional

Cuando diseñes clases:

> **Declara los miembros en orden de dependencia**

Si algo depende de otra cosa:

```cpp
Logger logger;
Database db;
```

NO al revés.

## Resumen brutalmente importante

### Construcción

Orden fijo:

1. Base classes
2. Miembros (en orden de declaración)
3. Cuerpo constructor

### Destrucción

Orden inverso:

1. Cuerpo destructor
2. Miembros (orden inverso)
3. Base classes

## REGLA MÁS IMPORTANTE DE TODAS

> **El initializer list NO controla el orden de construcción**

SOLO controla QUÉ constructor usar.

El orden real lo controla la declaración de miembros.

Eso es probablemente lo más importante de todo este tema.
