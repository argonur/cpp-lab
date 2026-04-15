# Object Slicing (Rebanamiento)

El **Object Slicing** ocurre cuando asignas un objeto de una clase derivada a un objeto de una clase base **por valor**. C++ copia únicamente la parte que corresponde a la base y "rebana" (descarta) todo lo que hace única a la clase hija, incluyendo su comportamiento polimórfico (sus funciones `virtual`).

## El Escenario del Desastre (Por Valor)

Imagina que tienes una base `MetodoPago` y dos hijas: `Tarjeta` y `Efectivo`.

```cpp
#include <iostream>
#include <vector>
#include <string>

struct MetodoPago {
    virtual void procesar() const {
        std::cout << "Procesando pago genérico..." << std::endl;
    }
    virtual ~MetodoPago() = default; // Siempre usa destructor virtual en bases
};

struct Tarjeta : public MetodoPago {
    void procesar() const override {
        std::cout << "Validando chip y procesando Tarjeta..." << std::endl;
    }
};

struct Efectivo : public MetodoPago {
    void procesar() const override {
        std::cout << "Abriendo caja registradora para Efectivo..." << std::endl;
    }
};

int main() {
    // ERROR: Vector de objetos por VALOR
    std::vector<MetodoPago> carrito;

    Tarjeta visa;
    Efectivo billete;

    // Aquí ocurre el SLICING:
    // Al meter 'visa' en un vector de 'MetodoPago', C++ crea una COPIA.
    // Como el vector solo tiene espacio para el tamaño de 'MetodoPago',
    // "rebana" la parte de 'Tarjeta'.
    carrito.push_back(visa); 
    carrito.push_back(billete);

    for (const auto& pago : carrito) {
        pago.procesar(); // SALIDA: "Procesando pago genérico..." (X2)
    }

    return 0;
}
```

¿Por qué falló? El vector `std::vector<MetodoPago>` es una fila de "cajas" de tamaño fijo (el tamaño de la base). No importa qué tan grande sea la hija, C++ la fuerza a entrar en la caja pequeña recortando lo que sobra. **El polimorfismo muere aquí.**

## La Solución Profesional (Con Punteros Inteligentes)

Para mantener la identidad de la hija, el vector no debe guardar el objeto, sino una **dirección de memoria** (un puntero). Como todos los punteros miden lo mismo (normalmente 8 bytes), la "caja" del vector siempre es del mismo tamaño y apunta al objeto completo que vive en otra parte (el heap).

```cpp
#include <iostream>
#include <vector>
#include <memory> // Para unique_ptr

// ... (Mismas estructuras de arriba) ...

int main() {
    // SOLUCIÓN: Vector de punteros únicos a la Base
    std::vector<std::unique_ptr<MetodoPago>> carrito;

    // Creamos los objetos dinámicamente
    carrito.push_back(std::make_unique<Tarjeta>());
    carrito.push_back(std::make_unique<Efectivo>());

    for (const auto& pago : carrito) {
        // El polimorfismo se mantiene gracias al puntero
        pago->procesar(); 
    }

    // SALIDA:
    // "Validando chip y procesando Tarjeta..."
    // "Abriendo caja registradora para Efectivo..."

    return 0;
}
```

## Resumen para tus notas

|Característica|`std::vector<Base>`|`std::vector<std::unique_ptr<Base>>`|
|-|-|-|
|Almacenamiento|Por Valor (Copia física)|Por Puntero (Dirección de memoria)|
|Polimorfismo|NO. Se pierde (Object Slicing).|SÍ. Se mantiene el comportamiento.|
|Memoria|Estática dentro del vector.|Dinámica (Heap).|
|Uso de Memoria|Ineficiente si las hijas son grandes.|Óptimo (solo guardas direcciones).|

## ¿Los objetos tienen que estar en el heap forzosamente o podrían estar en el stack?

Esa es una pregunta excelente porque toca el corazón de la **gestión de memoria** y la seguridad en C++. La respuesta corta es: **Sí, pueden estar en el stack, pero es sumamente peligroso** y casi nunca se hace en entornos profesionales por una razón: el **Tiempo de Vida (Lifetime)** de los objetos.

Aquí te explico por qué no conviene y cuál es el riesgo real:

1. El riesgo del "Puntero Colgante" (Dangling Pointer)

Si creas los objetos en el stack dentro de una función o un bloque `{ }`, esos objetos **mueren** en cuanto el programa sale de ese bloque. Si tu vector guarda las direcciones (`&`) de esos objetos, al final tendrás un vector lleno de direcciones que apuntan a basura o a memoria que ya fue reutilizada.

Ejemplo del desastre:

```cpp
std::vector<Base*> miVector;

{
    Derived objetoStack; 
    miVector.push_back(&objetoStack); // Guardas la dirección del stack
} // <--- AQUÍ MUERE 'objetoStack'

// ERROR: Estás accediendo a memoria que ya no existe
miVector[0]->procesar(); // Crash o comportamiento indefinido
```

2. ¿Cuándo SÍ podrían estar en el stack?

Podrían estar en el stack **únicamente** si garantizas que el vector morirá **antes** que los objetos. Por ejemplo, si ambos están en el mismo `main()` y el vector nunca sale de ahí.

Aun así, no se recomienda porque:

- Rigidez: No puedes añadir objetos dinámicamente según la lógica del programa (como leer de una base de datos o un archivo de configuración).

- Tamaño: El stack es limitado. Si tienes miles de objetos grandes, podrías causar un Stack Overflow.

3. Por qué el Heap es el estándar para el Polimorfismo

Al usar el **Heap** (con `std::unique_ptr` o `std::make_unique`), tú tomas el control total:

- **Independencia**: El objeto vive lo que el vector decida que viva.

- **Seguridad**: `std::unique_ptr` destruye el objeto automáticamente cuando el vector se limpia o sale de alcance. No hay fugas de memoria (memory leaks).

- **Flexibilidad**: Puedes pasar ese vector de una función a otra sin miedo a que los objetos desaparezcan.

### La explicación técnica de la V-Table (El "Por qué" de los punteros)

¿Por qué el vector de objetos falla y el de punteros funciona? Por la **V-Table (Tabla Virtual)**.

- Cada clase con funciones `virtual` tiene una **V-Table**: una lista oculta de direcciones a sus funciones.

- Cada objeto de esa clase tiene un puntero oculto llamado **vptr** que apunta a esa tabla.

- **En el Object Slicing**: Al copiar el objeto por valor, C++ copia los datos pero **cambia el vptr** para que apunte a la tabla de la `Base`. El objeto "olvida" quién era.

- **Con Punteros**: El puntero apunta al objeto original en el heap, el cual mantiene su **vptr** original apuntando a la tabla de la clase `Derived`.

### Conclusión para tu estudio:

"Para que el polimorfismo funcione, necesito **indirección** (punteros o referencias). Aunque técnicamente puedo apuntar al stack, la buena práctica dicta usar el **heap** con **smart pointers** para garantizar que el objeto viva lo mismo que el contenedor y evitar errores de memoria segmentada."
