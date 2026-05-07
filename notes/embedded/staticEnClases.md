# `static` dentro de clases en C++

`static` dentro de una clase cambia completamente de significado respecto a `static` global o local.

Aquí ya no hablamos principalmente de linkage o lifetime local.

Dentro de clases, `static` significa:

> El miembro pertenece a la clase misma, no a cada instancia.

## Variables miembro `static`

### Variable miembro normal

```cpp
class MyClass {
public:
    int value;
};
```

Cada objeto tiene su propia copia:

```cpp
MyClass a;
MyClass b;

a.value = 10;
b.value = 20;
```

Memoria independiente:

`a.value`
`b.value`

### Variable miembro `static`

```cpp
class MyClass {
public:
    static int counter;
};
```

Aquí:

👉 Existe UNA sola variable compartida por toda la clase.

No importa cuántos objetos existan.

### Definición obligatoria en `.cpp`

Esto es MUY importante.

#### Header

```cpp
class MyClass {
public:
    static int counter;
};
```

#### `.cpp`

```cpp
int MyClass::counter = 0;
```

#### ¿Por qué?

La declaración dentro de la clase NO crea memoria.

Solo declara el miembro.

La definición real debe existir una vez.

### Error clásico

```cpp
class MyClass {
public:
    static int counter = 0; // ❌ (antes de C++17)
};
```

Eso generaba error en estándares antiguos.

### Desde C++17: `inline static`

Ahora sí es válido:

```cpp
class MyClass {
public:
    inline static int counter = 0;
};
```

Esto evita definir en .cpp.

### Acceso a miembros `static`

#### Con objeto (válido, pero menos claro)

```cpp
MyClass obj;
obj.counter++;
```

#### Forma correcta/preferida

```cpp
MyClass::counter++;
```

Porque el miembro pertenece a la clase.

### Memoria y lifetime

Un miembro `static`:

- tiene static storage duration
- vive toda la ejecución del programa

No vive dentro de cada objeto.

### Tamaño del objeto

MUY importante:

```cpp
class MyClass {
    int x;
    static int y;
};

```

`sizeof(MyClass)`:

- incluye `x`
- NO incluye `y`

Porque `y` no pertenece físicamente al objeto.

### Usos típicos

#### Contador de instancias

```cpp
class MyClass {
public:
    static int instanceCount;

    MyClass() {
        instanceCount++;
    }
};
```

#### Configuración compartida

```cpp
class Logger {
public:
    static bool enabled;
};
```

#### Recursos globales controlados

Muy común en embedded:

- drivers
- pools
- estadísticas
- registros compartidos

## Funciones miembro `static`

También existen.

```cpp
class Math {
public:
    static int add(int a, int b);
};
```

### Qué significa aquí

La función:

- NO pertenece a una instancia
- NO recibe `this`

### Consecuencia importante

Dentro de una función `static`:

❌ NO puedes acceder a miembros no estáticos:

```cpp
class Test {
    int x;

    static void foo() {
        x = 5; // ❌
    }
};
```

Porque no existe objeto.

### Sí puedes acceder a miembros `static`

```cpp
class Test {
    static int counter;

    static void foo() {
        counter++;
    }
};
```

### Llamada correcta

```cpp
Math::add(2, 3);
```

No necesitas objeto.

### Relación con OOP

Un miembro `static`:

- pertenece a la clase
- no participa en polimorfismo dinámico normal
- no forma parte del estado individual del objeto

### En embedded esto importa mucho

Porque:

- evita duplicar memoria por instancia
- permite compartir recursos hardware
- facilita singletons/drivers

Pero:

⚠ abusar de `static` puede crear:

- acoplamiento global
- estado oculto
- problemas de concurrencia

### Diferencias importantes

|Tipo de miembro|Pertenece a|
|-|-|
|normal|cada objeto|
|static|la clase|

|Característica|Miembro normal|Miembro static|
|-|-|-|
|requiere objeto|sí|no|
|tiene this|sí|no|
|una copia por objeto|sí|no|
|compartido|no|sí|

### Preguntas típicas de entrevista

#### “¿Dónde vive un miembro static?”

Respuesta:

> Tiene static storage duration y existe independientemente de las instancias.

#### “¿Por qué un método static no puede acceder a miembros no static?”

Respuesta:

> Porque no existe puntero `this`; la función no está asociada a ningún objeto.

#### “¿Los miembros static afectan `sizeof(class)`?”

Respuesta:

> No, porque no forman parte física de cada instancia.
