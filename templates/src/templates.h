#pragma once

#include <cstring>
#include <iostream>

/*
Templates de Función

template<typename T>
T max (T a, T b) {
    return b < a ? a : b;
}

Los templates se introducen con template< Parámetro1, ...>
typename define un parámetro de template con nombre T.
El tipo real de T (argumento de template) se determina solo cuando se llama la función.

*/

template<typename T>
T max (T a, T b) {
    return b < a ? a : b;
}

/*
Diferentes argumentos de template generan diferentes instancias.

Los parámetros de template pueden utilizarse en el template como tipos ya asignados.

template <typename T>
void DoSomething(T arg1, int arg2) {
    int twoTimesArg2 = 2*arg2;
    T twoTimesArg1 = 2*arg1;
}

Es posible cualquier número de parámetros de template.
Los argumentos de template y normales pueden mezclarse.

template <typename T, typename U>
U DoSomethingElse(T arg1, int arg2, U arg3) { ... }

*/

/*
Ejercicio: Escribir el primer template
*/
template<typename T>
T pow(T base, int exp)
{
    if (exp == 0)
        return T{1};   // aquí sí necesitas 1

    T result = base;

    for (int i = 1; i < exp; ++i)
        result = result * base;

    return result;
}

/*
Los parámetros de template pueden especificar un valor en lugar de un tipo.
Posibles son: valores integrales, enums y punteros a objetos. Desde C++20 todos los tipos literales.
Aquí se deben usar corchetes <> para los templates de función.
*/

template <int Multiplier>
int MultWith(int value) {
    return value*Multiplier;
}

/*
Class Templates

template<class T>
struct vector {  Implementación };

Análogamente a las funciones, también se pueden parametrizar clases enteras con tipos.
Los templates de clase más utilizados en C++ son los tipos contenedor de la biblioteca estándar.

Example:

template <typename T>
struct DataHolder {
    DataHolder(T value)
        : m_value(value) { }
    T read() const { return m_value; }
private:
    T m_value;
};

DataHolder<float> data(2.0f);
float val = data.read();
*/

/*
Arrays seguros

¿Podemos recrear arrays C con templates y mejorarlos?
El tamaño del array está fijado en tiempo de compilación.
Lectura/escritura de elementos mediante métodos get y set.
Protección contra desbordamiento.
Comportamiento razonable para comparaciones de diferentes arrays.
*/

/*
Ejercicio 2: SafeArray
*/
#include <concepts>

template<typename T, int Size>
requires std::equality_comparable<T> // En C++20 obliga a que T sea comparable con el operador == para poder usarlo en el operador de comparación de SafeArray.
class SafeArray {
public:
    SafeArray(const T& defaultValue) : m_defaultValue(defaultValue) {
        for (int i = 0; i < Size; ++i) {
            m_array[i] = defaultValue;
        }
    }

    T get(int index) const {
        if (index < 0 || index >= Size) {
            return m_defaultValue; // Devuelve el valor predeterminado si el índice es inválido
        }
        return m_array[index];
    }

    void set(int index, T value) {
        if (index >= 0 && index < Size) {
            m_array[index] = value;
        }
    }

/*
El operador de comparación

int main() {
SafeArray<int,2> arrA; arrA[0] = 1; arrA[1] = 2;
SafeArray<int,2> arrB; arrB[0] = 1; arrB[1] = 2;
bool equal = (arrA == arrB);
std::cout << "Los arrays son iguales: " << equal << "\n";
}

La clase SafeArray debe definir un operador de comparación para que este código pueda compilar.
*/


    bool operator==(const SafeArray<T, Size>& rhs) const {

        for (int i = 0; i < Size; ++i) {
            if (m_array[i] != rhs.m_array[i]) {
                return false;
            }
        }
        return true;
        
    }

private:
    T m_array[Size];
    T m_defaultValue;
};

/*
Organizar código de template

// Declaración (en el header)
template <int value>
inline int get();

// Definición (¡también en el header!)
template <int value>
inline int get() { return value; }

Siempre definir funciones template como inline. 
Recomendación: No declarar funciones y clases template (solo definir).

Las funciones y clases template deben definirse completamente en archivos de header,
para que el compilador pueda generarlas correctamente durante la instanciación. No es posible,
separar la declaración y definición de funciones o clases template en archivos separados,
como es habitual con funciones o clases normales. 
Por lo tanto, las funciones y clases template siempre deberían definirse en archivos de header para asegurar,
que puedan instanciarse correctamente cuando se usan en otras partes del programa. 
También es una buena práctica declarar funciones template como inline,
para evitar posibles definiciones múltiples cuando el archivo de header se incluye en múltiples unidades de traducción.

*/

/*
Selección automática de template en funciones

template<typename T>
T max (T a, T b) {
    return b < a ? a : b;
}
int i1=1; int i2=2;
max(i1,i2); // T=int

1. Hasta ahora, al llamar a templates de función, los argumentos de template no se especifican.
2. El compilador entonces intenta deducir los argumentos de template automáticamente.
3. Una especificación manual siempre es posible y por ejemplo necesaria para argumentos de template no tipo.

max<int>(i1,i2); // T=int
max<long>(i1,i2); // T=long -> i1 e i2 se convierten automáticamente de int a long
*/

/*
Conceptos: Parámetro de Template vs. Parámetro de Llamada

template <typename T>
void bar(T* val) { printf("bar(T*) called.\n");
int i=2;
bar(&i);

Los parámetros de template se especifican en <>.
Los parámetros de llamada pertenecen a la signatura y se refieren a parámetros de template.
T es un parámetro de template →Deducción al argumento de template int.
val es un parámetro de llamada →Aquí instanciado con int*
*/

/*
Selección de alternativas: Sobrecarga

Sobrecarga habitual de funciones en C++

void foo(int val) { printf("foo(int) called.\n"); }
void foo(int* val) { printf("foo(int*) called.\n");

int i=2;
foo(i); // foo(int) called
foo(&i) // foo(int*) called

Funcionalidad intuitivamente igual en templates

template <typename T>
void bar(T val) { printf("bar(T) called.\n"); }
template <typename T>
void bar(T* val) { printf("bar(T*) called.\n");

int i=2;
bar(i); // bar(T) called
bar(&i) // bar(T*) called
*/

/*
Reglas para sobrecarga

La sobrecarga en templates funciona de manera similar a las funciones normales.
Reglas internas complejas producen generalmente resultados intuitivos.
Las funciones normales y las funciones template se pueden mezclar.
Las funciones normales se prefieren sobre las funciones template.

void func(int val) { printf("A) func(int) called.\n"); }

void func(short* val) { printf("B) func(short*) called.\n"); }

template <typename T>
void func(T val) { printf("C) func(T) called.\n");

int i=2; short s=3;
func(i); // (1)
func(&i); // (2)
func(s); // (3)
func(&s); // (4)

(1) func(int) called. → Se prefiere la función normal
(2) func(T*) called. → Se prefiere la función template, ya que no existe función normal con int*.
(3) func(int) called. → Se prefiere la función normal, ya que short se puede convertir a int.
(4) func(short*) called. → Se prefiere la función normal, ya que short* coincide con short*.

Resumen de la Lógica de Prioridad

Para decidir a quién llamar, el compilador sigue este orden de "preferencia":

    Coincidencia exacta con función no-plantilla.

    Coincidencia mediante promoción (ej. short a int) en función no-plantilla.

    Coincidencia exacta con Plantilla (instanciando el código).

    Coincidencia con conversiones complejas (esta es la última opción y a veces falla).

*/

/*
Ejemplos de parámetros de llamada

1️⃣
template <typename T>
T max(T valueA, T valueB);


Aquí los parámetros se pasan por valor.

Eso implica:

Se copian.

T debe ser copiable.

Puede ser costoso si T es grande.

✔ Bien para tipos pequeños (int, double).

2️⃣
template <typename T>
void Swap(T& valueA, T& valueB);


Aquí se pasan por referencia no const.

Eso significa:

No hay copia.

La función puede modificar los argumentos.

Es eficiente.

Se usa cuando quieres modificar los objetos originales.

3️⃣
template <typename T>
T SumAll(std::vector<T> const& vec);


Aquí se pasa el vector por:

const&


Eso significa:

No copia el vector (eficiente).

No puede modificarlo.

Solo lo lee.

Es la forma correcta para leer contenedores grandes.

4️⃣
template <typename T>
void SetAll(std::vector<T>& vec, T const& newValue);


Aquí tienes dos cosas:

vec → referencia no const (porque lo modificas)

newValue → const referencia (no lo modificas y evitas copia)

Esto es diseño correcto de interfaces.

🎯 En templates, el tipo es genérico, pero la forma de pasar parámetros sigue siendo crítica:

Por valor → copia

Por referencia → evita copia

const& → lectura eficiente

& → modificación

T vs T const& depende del uso

🧠 Lo que deberías estar pensando ahora

Para un tipo genérico T:

¿Cuándo conviene T?

¿Cuándo T&?

¿Cuándo const T&?

Esa decisión impacta rendimiento y restricciones del template.

*/

/*
CTAD (Class Template Argument Deduction o Deducción de Argumentos de Plantillas de Clase).

Lo que te quiere decir, en términos sencillos, es que ya no tienes que ser tan redundante al escribir código. 

Aquí tienes el desglose:

1. El cambio principal (C++17)

Antes de C++17, cuando querías crear un objeto de una clase con plantillas, tenías que decirle al compilador el tipo de dato dos veces: 
una en la declaración y otra en el constructor (o dejar que el compilador lo infiriera mediante funciones auxiliares como std::make_pair).

Ahora, el compilador es capaz de mirar los argumentos que le pasas al constructor y deducir el tipo automáticamente.

2. Análisis de los ejemplos

Compara tres formas de instanciar la estructura DataHolder:

    DataHolder<float> data1(2.0f);

        Cómo funciona: Especificas explícitamente que T es float.

        Compatibilidad: Funciona en todas las versiones de C++. Es la forma "clásica".

    DataHolder data2(2.0f);

        Cómo funciona: No escribes <float>. El compilador ve que 2.0f es un literal de tipo float y deduce que T = float.

        Requisito: Solo funciona a partir de C++17.

    DataHolder data3(4);

        Cómo funciona: El compilador ve un entero (4) y deduce automáticamente que T = int.

        Requisito: También requiere C++17.

3. Notas importantes de la diapositiva

    Deducción Automática: El compilador siempre intentará adivinar el tipo por ti si no se lo das.

    Especificación Manual: Sigue siendo posible (y a veces necesario) indicar el tipo manualmente.

    Casos Obligatorios: La diapositiva menciona que la indicación manual sigue siendo necesaria para Nontype Template Arguments 
    (argumentos que no son tipos, como un tamaño de array template <int N>) o cuando el compilador no tiene suficiente información para deducir.

En resumen: C++17 hace que el código sea más limpio y menos propenso a errores de "copiar y pegar" al permitir que el compilador deduzca el tipo 
de la clase directamente desde el constructor.
*/

/*
Template function to copy elements from one array to another.
This is a safer alternative to memcpy because it deduces the type T automatically,
preventing errors that could occur with memcpy when dealing with non-byte-aligned data.
*/
template<typename T>
void copy_items(T* dest, const T* src, std::size_t count) {
    std::memcpy(dest, src, count * sizeof(T)); // Copiar la cantidad correcta de bytes
}

/*
Pruebas para Templates

¿Cómo se pueden probar funciones normales?

Un programa extra con pruebas unitarias.
Pruebas integradas con assert().

¿Cómo se pueden probar templates?

static_assert permite pruebas en tiempo de compilación.
La condición debe ser una expresión constexpr.

static constexpr int S = 2;
static_assert(S == 3, "S no es 3");

~] $ <source>:2:17: error: static assertion failed: S no es 3 static_assert(S == 3, "S no es 3")
*/

/*
Type Traits

Type Traits
„trait n. ... a distinctive feature characterizing a thing.“ New Shorter Oxford English Dictionary

Los Type Traits en C++ describen propiedades de un tipo de datos.
Muchas propiedades prácticas en #include <type_traits> .
Resumen: https://en.cppreference.com/w/cpp/header/type_traits
Los traits son siempre expresiones constexpr.

bool int_signed = std::is_signed_v<int>; // true
bool int_signed_old = std::is_signed<int>::value; // true, hasta C++17 solo posible así!
bool uchar_signed = std::is_signed_v<unsigned char>; // false

int x = 3; // decltype(x) -> int
static_assert(std::is_const_v<decltype(x)>, "Variable is not const!"); // static assert failed
*/

/*
Type Traits y static_assert

Los tipos no se pueden comparar directamente en C++.

static_assert(T != float, "¡Aquí no floats!"); // Código inválido!

template <typename T>
struct DataHolder {
    DataHolder(T value) : m_value(value) { }
    static_assert(!std::is_same_v<T,float>, "Floats are not allowed here.")
private:
    T m_value;
};

DataHolder<int> data1(2);
DataHolder<float> data2(2.0f); // static assertion failed: Floats are not allowed here
*/

/*
Distinciones en tiempo de compilación
*/

/*
Substitution Failure Is Not An Error (SFINAE)

„This rule applies during overload resolution of function templates: When substituting the explicitly
specified or deduced type for the template parameter fails, the specialization is discarded from the
overload set instead of causing a compile error.“
https://en.cppreference.com/w/cpp/language/sfinae

template <typename T>
decltype(T::x) print_x_or_y(T val) {
    std::cout << "T has member x. Value: " << val.x;
    return val.x;
}

struct S_with_x { int x = 42; };
struct S_without { };

S_with_x sx;
S_without swo;

print_x_or_y(sx);
print_x_or_y(swo);
*/

/*SFINAE para distinción*/

template <typename T>
decltype(T::x) print_x_or_y(T val) {
    std::cout << "T has member x. Value: " << val.x << std::endl;
    return val.x;
}

template <typename T>
decltype(T::y) print_x_or_y(T val) {
    std::cout << "T has member y. Value: " << val.y << std::endl;
    return val.y;
}

