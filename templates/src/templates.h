#pragma once

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
Template Parameter können statt Typ auch einen Wert angeben.
Möglich sind: Integral-Werte, Enums und Pointer auf Objekte. Ab C++20 alle Literal Types.
Hier müssen bei Function Templates <> Klammern verwendet werden.
*/

template <int Multiplier>
int MultWith(int value) {
    return value*Multiplier;
}

/*
Class Templates

template<class T>
struct vector {  Implementierung };

Analog zu Funktionen können auch ganze Klassen mit Typen parametriert werden.
Die meist-verwendeten Klassentemplates in C++ sind die Container Typen der Standardbibliothek.

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
Sichere Arrays

Können wir mit Templates C-Arrays nachbilden und besser machen?
Größe des Arrays ist zur Compile-Zeit fixiert.
Lesen/Schreiben von Elementen über get und set Methoden.
Schutz vor Überlauf.
Vernünftiges Verhalten für Vergleiche verschiedener Arrays.
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
Der Vergleichs-Operator

int main() {
SafeArray<int,2> arrA; arrA[0] = 1; arrA[1] = 2;
SafeArray<int,2> arrB; arrB[0] = 1; arrB[1] = 2;
bool equal = (arrA == arrB);
std::cout << "Arrays sind gleich: " << equal << "\n";
}

Die SafeArray Klasse muss einen Vergleichs-Operator definieren, damit dieser Code kompilieren kann.
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
Template Code organisieren

// Deklaration (im Header)
template <int value>
inline int get();

// Definition (auch im Header!)
template <int value>
inline int get() { return value; }

Template Funktionen immer als inline definieren. 
Empfehlung: Template-Funktionen und -Klassen nicht deklarieren (nur definieren).

Template Funktionen und Klassen müssen vollständig in Header-Dateien definiert werden, 
damit der Compiler sie bei der Instanziierung korrekt generieren kann. Es ist nicht möglich, 
die Deklaration und Definition von Template Funktionen oder Klassen in separate Dateien zu trennen, 
wie es bei normalen Funktionen oder Klassen üblich ist. 
Daher sollten Template Funktionen und Klassen immer in Header-Dateien definiert werden, um sicherzustellen, 
dass sie korrekt instanziiert werden können, wenn sie in anderen Teilen des Programms verwendet werden. 
Es ist auch eine gute Praxis, Template Funktionen als inline zu deklarieren, 
um mögliche Mehrfachdefinitionen zu vermeiden, wenn die Header-Datei in mehreren Übersetzungseinheiten inkludiert wird.

*/

/*
Automatische Template Auswahl bei Funktionen

template<typename T>
T max (T a, T b) {
    return b < a ? a : b;
}
int i1=1; int i2=2;
max(i1,i2); // T=int

1. Bisher bei Aufruf von Function Templates die Template Argumente nicht angegeben.
2. Der Compiler versucht dann die Template Argumente automatisch zu deduzieren.
3. Eine manuelle Angabe ist immer möglich und bspw. bei Nontype Template Argumenten nötig.

max<int>(i1,i2); // T=int
max<long>(i1,i2); // T=long -> i1 und i2 werden automatisch von int auf long gecastet
*/

/*
Begriffe: Template vs. Call Parameter

template <typename T>
void bar(T* val) { printf("bar(T*) called.\n");
int i=2;
bar(&i);

Template Parameter werden in <> angegeben.
Call Parameter gehören zur Signatur und beziehen sich auf Template Parameter.
T ist ein Template Parameter →Deduzierung zum Template Argument int.
val ist ein Call Parameter →Hier instantiiert mit int*
*/

/*
Auswahl von Alternativen: Überladung

Übliche Überladung von Funktionen in C++

void foo(int val) { printf("foo(int) called.\n"); }
void foo(int* val) { printf("foo(int*) called.\n");

int i=2;
foo(i); // foo(int) called
foo(&i) // foo(int*) called

Intuitiv gleiche Funktionsweise bei Templates

template <typename T>
void bar(T val) { printf("bar(T) called.\n"); }
template <typename T>
void bar(T* val) { printf("bar(T*) called.\n");

int i=2;
bar(i); // bar(T) called
bar(&i) // bar(T*) called
*/

/*
Regeln für Überladung

Überladung bei Templates funktioniert ähnlich wie bei normalen Funktionen.
Komplexe interne Regeln erzeugen meist intuitive Ergebnisse.
Normale Funktionen und Template Funktionen lassen sich mischen.
Normale Funktionen werden Template Funktionen vorgezogen.

void func(int val) { printf("A) func(int) called.\n"); }

void func(short* val) { printf("B) func(short*) called.\n"); }

template <typename T>
void func(T val) { printf("C) func(T) called.\n");

int i=2; short s=3;
func(i); // (1)
func(&i); // (2)
func(s); // (3)
func(&s); // (4)

(1) func(int) called. → Normale Funktion wird vorgezogen
(2) func(T*) called. → Template Funktion wird vorgezogen, da keine normale Funktion mit int* existiert.
(3) func(int) called. → Normale Funktion wird vorgezogen, da short zu int konvertiert werden kann.
(4) func(short*) called. → Normale Funktion wird vorgezogen, da short* zu short* passt.

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

