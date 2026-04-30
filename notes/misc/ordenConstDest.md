# Constructores y destructores con herencia en C++

Este tema es MUY importante en C++, porque aquí es donde realmente entiendes cómo se construyen y destruyen objetos complejos en memoria.

Si entiendes bien esto:

- entiendes RAII,
- entiendes por qué existen los `virtual destructors`,
- entiendes por qué algunos bugs de memoria ocurren,
- y empiezas a pensar “como C++”.

## Idea fundamental

Cuando tienes herencia:

```cpp
class Base {};
class Derived : public Base {};
```

Un objeto `Derived` **contiene internamente** una parte `Base`.

Es decir:

```cpp
[ Base subobject ][ Derived subobject ]
```

Entonces:

- primero debe construirse la parte `Base`
- después la parte `Derived`

Y al destruir:

- primero se destruye `Derived`
- después `Base`

Porque se destruye en orden inverso.

## Orden de construcción

### Regla principal

#### Construcción:

```cpp
Base → Derived
```

#### Destrucción:

```cpp
Derived → Base
```

### Ejemplo básico

```cpp
#include <iostream>

class Base {
public:
    Base() {
        std::cout << "Base constructor\n";
    }

    ~Base() {
        std::cout << "Base destructor\n";
    }
};

class Derived : public Base {
public:
    Derived() {
        std::cout << "Derived constructor\n";
    }

    ~Derived() {
        std::cout << "Derived destructor\n";
    }
};

int main() {
    Derived d;
}
```

Salida:

```cpp
Base constructor
Derived constructor
Derived destructor
Base destructor
```

### ¿Por qué este orden?

Piensa así:

#### Construcción

Para construir un `Derived`, primero necesitas que exista correctamente la parte `Base`.

Entonces:

1. se construye `Base`
2. luego `Derived`

#### Destrucción

Cuando destruyes:

1. primero destruyes la parte especializada (`Derived`)
2. luego la base (`Base`)

Porque `Derived` puede depender de cosas de `Base`.

## Inicialización real vs asignación

MUCHA gente se confunde aquí.

Cuando haces:

```cpp
Derived() {
}
```

NO estás construyendo `Base` dentro del cuerpo.

La base ya fue construida ANTES de entrar al cuerpo del constructor.

## Cómo pasar parámetros al constructor base

Se hace mediante lista de inicialización.

### Ejemplo

```cpp
#include <iostream>

class Base {
public:
    Base(int x) {
        std::cout << "Base constructor: " << x << "\n";
    }
};

class Derived : public Base {
public:
    Derived(int x)
        : Base(x) // llamada explícita
    {
        std::cout << "Derived constructor\n";
    }
};

int main() {
    Derived d(42);
}
```

Salida:

```cpp
Base constructor: 42
Derived constructor
```

### ¿Qué pasa si NO llamas al constructor base?

C++ intenta llamar:

```cpp
Base()
```

o sea el constructor default.

#### Ejemplo válido

```cpp
class Base {
public:
    Base() {}
};

class Derived : public Base {
public:
    Derived() {}
};
```

#### Ejemplo inválido

```cpp
class Base {
public:
    Base(int x) {}
};

class Derived : public Base {
public:
    Derived() {}
};
```

Error:

```cpp
no matching constructor for Base()
```

Porque `Base` no tiene constructor default.

## Orden REAL de inicialización

MUY importante.

El orden NO depende de la lista de inicialización.

Depende del orden de declaración.

### Primero: clases base

Luego:

### miembros de la clase

Luego:

### cuerpo del constructor

#### Ejemplo

```cpp
class A {
public:
    A() { std::cout << "A\n"; }
};

class B {
public:
    B() { std::cout << "B\n"; }
};

class Test : public A {
    B b;

public:
    Test() {
        std::cout << "Test\n";
    }
};
```

Salida:

```cpp
A
B
Test
```

## Orden de destrucción completo

Destrucción:

```cpp
cuerpo destructor
↓
miembros
↓
bases
```

En orden inverso a construcción.

### Ejemplo completo

```cpp
#include <iostream>

class A {
public:
    A() { std::cout << "A ctor\n"; }
    ~A() { std::cout << "A dtor\n"; }
};

class B {
public:
    B() { std::cout << "B ctor\n"; }
    ~B() { std::cout << "B dtor\n"; }
};

class Test : public A {
    B b;

public:
    Test() {
        std::cout << "Test ctor\n";
    }

    ~Test() {
        std::cout << "Test dtor\n";
    }
};

int main() {
    Test t;
}
```

Salida:

```cpp
A ctor
B ctor
Test ctor

Test dtor
B dtor
A dtor
```

## Herencia múltiple

Aquí se vuelve más interesante.

### Ejemplo

```cpp
class A {
public:
    A() { std::cout << "A\n"; }
};

class B {
public:
    B() { std::cout << "B\n"; }
};

class C : public A, public B {
public:
    C() {
        std::cout << "C\n";
    }
};
```

Salida:

```cpp
A
B
C
```

### Regla

Las bases se construyen:

> **en el orden en que aparecen declaradas**

NO en la lista de inicialización.

### Ejemplo peligroso

```cpp
class C : public A, public B {
public:
    C()
        : B(), A()
    {}
};
```

AUN ASÍ:

```cpp
A
B
```

Porque el orden real es:

```cpp
class C : public A, public B
```

## Destructores virtuales

**ESTO ES CRÍTICO.**

### Problema

```cpp
class Base {
public:
    ~Base() {
        std::cout << "Base dtor\n";
    }
};

class Derived : public Base {
public:
    ~Derived() {
        std::cout << "Derived dtor\n";
    }
};

int main() {
    Base* ptr = new Derived;
    delete ptr;
}
```

Resultado:

```cpp
Base dtor
```

> **¡¡¡ERROR GRAVÍSIMO!!!**

No se llamó el destructor de `Derived`.

Eso puede provocar:

- memory leaks
- recursos abiertos
- mutexes no liberados
- sockets abiertos
- corrupción

### Solución

Destructor virtual.

```cpp
class Base {
public:
    virtual ~Base() {
        std::cout << "Base dtor\n";
    }
};
```

Ahora:

```cpp
Derived dtor
Base dtor
```

### Regla de oro

> **Si una clase será usada polimórficamente:**

usa destructor virtual.

## Constructor virtual NO existe

Esto es MUY importante.

No existen constructores virtuales.

Porque:

- para llamar algo virtual necesitas un objeto ya construido
- pero el constructor precisamente está construyendo el objeto

Entonces no tendría sentido.

## Qué NO debes hacer en constructores

### 1. Llamar funciones virtuales

Ejemplo:

```cpp
class Base {
public:
    Base() {
        foo();
    }

    virtual void foo() {
        std::cout << "Base foo\n";
    }
};

class Derived : public Base {
public:
    void foo() override {
        std::cout << "Derived foo\n";
    }
};
```

Resultado:

```cpp
Base foo
```

NO llama `Derived::foo`.

Porque durante el constructor de `Base`:

```cpp
Derived todavía NO existe completamente
```

### Igual aplica en destructores

Durante destrucción:

- primero muere `Derived`
- luego entra `Base::~Base`

Entonces ya NO existe la parte derivada.

## Flujo mental correcto

Cuando construyes:

```cpp
Base empieza a existir
↓
Base termina
↓
Derived empieza a existir
↓
Derived termina
```

## Flujo de destrucción

```cpp
Derived empieza a morir
↓
Derived termina
↓
Base empieza a morir
↓
Base termina
```

## Inicialización de miembros y bases

Orden real:

1. Bases
2. Miembros (en orden de declaración)
3. Constructor body

NO:

```cpp
lista de inicialización
```

## Ejemplo MUY importante

```cpp
class Test {
    int b;
    int a;

public:
    Test()
        : a(1), b(a)
    {}
};
```

¿Valor de `b`?

NO es 1 necesariamente.

Porque:

```cpp
int b;
int a;
```

`b` se inicializa ANTES que `a`.

Entonces:

```cpp
b(a)
```

usa `a` sin inicializar.

BUG clásico de C++.

## Recomendación profesional

Siempre declara miembros:

- en el orden en que dependen unos de otros
- y escribe la lista de inicialización en el mismo orden

## Resumen ultra importante

### Construcción

```cpp
Bases
↓
Miembros
↓
Constructor body
```

### Destrucción

```cpp
Destructor body
↓
Miembros
↓
Bases
```

### Orden

Siempre:

```cpp
construcción → arriba hacia abajo
destrucción → abajo hacia arriba
```

## Reglas profesionales reales

### Usa lista de inicialización

Especialmente para:

- referencias
- const
- objetos complejos
- bases

### Usa destructores virtuales en jerarquías polimórficas

Casi obligatorio.

### NO llames virtuales en constructores/destructores

Produce comportamiento engañoso.

### Recuerda:

La inicialización REAL ocurre antes del cuerpo del constructor.

El constructor NO “crea” miembros dentro del cuerpo.

Ya existen antes de entrar ahí.

## Modelo mental definitivo

Piensa que el objeto se arma por capas:

```cpp
Base
↓
Derived
↓
MoreDerived
```

Y se desarma al revés:

```cpp
MoreDerived
↓
Derived
↓
Base
```

Así es exactamente como funciona internamente C++.
