# Control de acceso y herencia en C++

Este tema suele confundir porque en C++ las palabras `public`, `protected` y `private` se usan en dos contextos distintos:

1. Como **especificadores de acceso de miembros**

    → controlan quién puede acceder a variables y funciones de una clase.
2. Como **modificadores de herencia**

    → controlan cómo se heredan esos accesos hacia clases derivadas.

Además existe `friend`, que rompe parcialmente las reglas de encapsulación.

## 1. Especificadores de acceso de miembros

Primero entiende esto, porque la herencia depende completamente de estas reglas.

Los tres niveles básicos son:

|Especificador|Quién puede acceder|
|-|-|
|`public`|cualquiera|
|`protected`|la clase y sus derivadas|
|`private`|solo la propia clase|

## 2. public, protected y private en miembros de clase

### public

Los miembros públicos forman la **interfaz externa** de la clase.

```cpp
class Car
{
public:
    void start();
    int speed;
};
```

Cualquiera puede hacer:

```cpp
Car c;
c.start();
c.speed = 100;
```

### private

Los miembros privados solo pueden ser usados dentro de la misma clase.

```cpp
class Car
{
private:
    int speed;
};
```

Esto NO compila:

```cpp
Car c;
c.speed = 100;
```

Pero sí:

```cpp
class Car
{
private:
    int speed;

public:
    void set_speed(int s)
    {
        speed = s;
    }
};
```

### protected

`protected` es parecido a `private`, PERO las clases hijas sí pueden acceder.

```cpp
class Vehicle
{
protected:
    int speed;
};

class Car : public Vehicle
{
public:
    void accelerate()
    {
        speed += 10;
    }
};
```

Desde afuera sigue siendo inaccesible:

```cpp
Car c;
c.speed = 50; // ERROR
```

## 3. Encapsulación

La idea principal es:

- `private`
  - → ocultar implementación interna.
- `protected`
  - → permitir extensión por herencia.
- `public`
  - → exponer interfaz.

En diseño moderno de C++:

- casi todo suele ser `private`
- `protected` se usa con moderación
- `public` solo para interfaz real

Muchos arquitectos consideran que abusar de `protected` acopla demasiado las clases hijas a la implementación interna del padre.

## 4. friend

`friend` permite romper encapsulación de forma controlada.

Una función o clase amiga puede acceder a miembros privados/protected.

### friend function

```cpp
class Box
{
private:
    int value = 42;

    friend void print_box(const Box& b);
};

void print_box(const Box& b)
{
    std::cout << b.value << std::endl;
}
```

Sin `friend`, eso sería ilegal.

### friend class

```cpp
class Engine;

class Car
{
private:
    int speed = 100;

    friend class Engine;
};

class Engine
{
public:
    void inspect(Car& c)
    {
        std::cout << c.speed << std::endl;
    }
};
```

## 5. ¿Qué significa heredar públicamente o privadamente?

Aquí cambia completamente el contexto.

Ahora `public/protected/private` NO controlan miembros.

Controlan cómo se transforma la interfaz heredada.

## 6. Herencia pública

```cpp
class Base
{
public:
    void f();
};

class Derived : public Base
{
};
```

Esto significa:

> "Derived ES UN Base"

Es la herencia normal y la más usada.

### Reglas

Con herencia pública:

|En Base|En Derived|
|-|-|
|public|public|
|protected|protected|
|private|inaccesible|

### Ejemplo

```cpp
class Base
{
public:
    void public_func();

protected:
    void protected_func();

private:
    void private_func();
};

class Derived : public Base
{
public:
    void test()
    {
        public_func();      // OK
        protected_func();   // OK
        // private_func();  // ERROR
    }
};
```

Y desde afuera:

```cpp
Derived d;
d.public_func(); // OK
```

## 7. Herencia protegida

```cpp
class Derived : protected Base
{
};
```

Ahora:

|En Base|En Derived|
|-|-|
|public|protected|
|protected|protected|
|private|inaccesible|

### Consecuencia importante

Lo que era público deja de ser público.

```cpp
class Base
{
public:
    void f();
};

class Derived : protected Base
{
};

Derived d;
d.f(); // ERROR
```

Pero dentro de `Derived` sí existe.

### Uso típico

Muy raro en código moderno.

Se usa cuando quieres reutilizar implementación,
pero NO quieres exponer la interfaz base públicamente.

## 8. Herencia privada

```cpp
class Derived : private Base
{
};
```

Transforma todo en privado.

|En Base|En Derived|
|-|-|
|public|private|
|protected|private|
|private|inaccesible|

### Ejemplo

```cpp
class Base
{
public:
    void f();
};

class Derived : private Base
{
public:
    void test()
    {
        f(); // OK
    }
};

Derived d;
d.f(); // ERROR
```

## 9. Diferencia conceptual IMPORTANTÍSIMA

### Herencia pública → relación IS-A

```cpp
Car IS-A Vehicle
```

La clase derivada puede usarse donde se espera la base.

```cpp
Vehicle* v = new Car();
```

### Herencia privada → implementación

```cpp
Derived IMPLEMENTS USING Base
```

NO implica sustitución polimórfica pública.

## 10. ¿Qué pasa con private de la base?

Los miembros privados:

- sí existen dentro del objeto derivado
- pero NO son accesibles directamente

```cpp
class Base
{
private:
    int x;
};
```

La memoria sigue ahí.

Simplemente el compilador impide acceso directo.

## 11. Tabla completa de transformación

### Herencia pública

|Base|Resultado|
|-|-|
|public|public|
|protected|protected|
|private|inaccesible|

### Herencia protegida

|Base|Resultado|
|-|-|
|public|protected|
|protected|protected|
|private|inaccesible|

### Herencia privada

|Base|Resultado|
|-|-|
|public|private|
|protected|private|
|private|inaccesible|

## 12. ¿Cuál se usa realmente?

En práctica profesional:

|Tipo|Uso real|
|-|-|
|public inheritance|MUY común|
|private inheritance|raro|
|protected inheritance|muy raro|

La mayoría de los casos que parecen requerir herencia privada realmente deberían usar composición:

```cpp
class Car
{
private:
    Engine engine;
};
```

En vez de:

```cpp
class Car : private Engine
{
};
```

## 13. Default access

### class

Por defecto:

```cpp
class X
{
    int a;
};
```

es:

```cpp
class X
{
private:
    int a;
};
```

### struct

Por defecto:

```cpp
struct X
{
    int a;
};
```

es:

```cpp
struct X
{
public:
    int a;
};
```

Lo mismo aplica para herencia:

```cpp
class D : Base
```

equivale a:

```cpp
class D : private Base
```

Mientras que:

```cpp
struct D : Base
```

equivale a:

```cpp
struct D : public Base
```

## 14. Otros modificadores relacionados importantes

Además de estos, existen otros keywords relacionados con clases/herencia:

|Keyword|Propósito|
|-|-|
|`virtual`|polimorfismo dinámico|
|`override`|verificar override correcto|
|`final`|impedir override/herencia|
|`explicit`|evitar conversiones implícitas|
|`static`|miembro compartido|
|`const`|no modificar estado|
|`mutable`|permitir modificación en métodos const|
|`constexpr`|evaluación en compile-time|
|`inline`|sugerencia de inline / ODR|
|`friend`|romper encapsulación|
|`volatile`|acceso especial a memoria|
|`noexcept`|promesa de no lanzar excepciones|

## 15. final

### Clase final

```cpp
class Base final
{
};
```

Impide herencia.

### Método final

```cpp
class Base
{
public:
    virtual void f() final;
};
```

Impide override en clases hijas.

## 16. Recomendaciones reales de diseño

### Regla práctica moderna

#### Variables miembro

- casi siempre `private`

#### Funciones públicas

- interfaz mínima necesaria

#### protected

- solo si realmente diseñas para herencia

#### Herencia

- usar solo para relaciones reales IS-A

#### Preferir composición

- cuando solo quieres reutilizar funcionalidad

## 17. Idea mental correcta

### Access specifiers

Controlan:

> "¿Quién puede tocar esto?"

### Modificadores de herencia

Controlan:

> "¿Cómo se expone la interfaz heredada?"

### friend

Controla:

> "¿A quién le doy acceso especial?"
