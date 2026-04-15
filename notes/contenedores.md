# Contenedores

Los contenedores en C++ son las estructuras de datos fundamentales de la STL (Standard Template Library). Se dividen principalmente en tres categorías: secuenciales, asociativos y asociativos no ordenados.

Aquí tienes un resumen de los más importantes, incluyendo los que mencionaste y algunos adicionales que son esenciales:

1. Contenedores Secuenciales (Ordenados por posición)

- `std::vector`: Es el contenedor por excelencia. Es un arreglo dinámico que permite acceso aleatorio rápido.

    - Ventaja: Acceso por índice en tiempo constante O(1).

    - Desventaja: Insertar o eliminar elementos en medio es costoso O(n).

- `std::list`: Una lista doblemente enlazada.

    - Ventaja: Inserción y eliminación rápida en cualquier punto O(1) si ya tienes el iterador.

    - Desventaja: No permite acceso directo (no puedes hacer lista[5]); debes recorrerla.

- `std::deque`: Una "cola de dos finales". Similar al vector pero optimizado para insertar/eliminar al inicio y al final.

2. Contenedores Asociativos (Ordenados por clave)

Utilizan internamente estructuras de árboles (generalmente árboles rojinegros), por lo que mantienen sus elementos siempre ordenados.

- `std::set`: Una colección de claves únicas. No permite duplicados.

    - Búsqueda/Inserción: O(logn).

- `std::map`: Diccionario de parejas clave-valor. Las claves son únicas y el contenedor se ordena mediante la clave.

    - Búsqueda/Inserción: O(logn).

3. Contenedores Asociativos No Ordenados (Hash Tables)

Introducidos en C++11, no mantienen un orden, pero son extremadamente rápidos para búsquedas directas.

- `std::unordered_map`: Implementa una tabla hash para pares clave-valor.

    - Eficiencia: Búsqueda en tiempo promedio constante O(1). Es preferible al map si no necesitas que los elementos estén alfabética o numéricamente ordenados.

- `std::unordered_set`: Similar al set, pero sin orden y con mayor velocidad de acceso O(1).

## Tabla Comparativa de Complejidad

|Contenedor|Acceso Aleatorio|Inserción (promedio)|Estructura interna|
|-|-|-|-|
|vector|O(1)|O(n) (al final es O(1) amortizado)|Arreglo continuo|
|list|O(n)|O(1)|Lista enlazada|
|set / map|O(logn)|O(logn)|Árbol Binario|
|unordered_map|O(1)|O(1)|Tabla Hash|

## ¿Hay otros importantes?

Sí, existen los Adaptadores de Contenedores, que limitan la interfaz de los anteriores para usos específicos:

1. `std::stack`: Estructura LIFO (Last-In, First-Out).
2. `std::queue`: Estructura FIFO (First-In, First-Out).
3. `std::priority_queue`: Una cola donde el elemento con mayor prioridad siempre sale primero (usualmente implementada como un Heap).

## Estructura interna

Entender la estructura interna es la clave para saber qué contenedor elegir, ya que de eso depende qué tan rápido (o lento) será tu programa.

Aquí tienes el desglose de las estructuras que mencionaste y cómo se relacionan con C++.
Estructuras de Datos Internas

- Arreglo Continuo (`std::vector`): Imagina una fila de casilleros numerados uno tras otro en memoria. El acceso es instantáneo porque solo necesitas el número de casillero.

- Lista Doblemente Enlazada (`std::list`): Cada elemento es un "nodo" que tiene el dato y dos flechas: una que apunta al vecino anterior y otra al siguiente. No están juntos en memoria, están dispersos y conectados por estos "punteros".

- Árbol Binario de Búsqueda: Es una estructura donde cada nodo tiene máximo dos hijos. El hijo izquierdo es menor y el derecho es mayor. Esto permite descartar la mitad de los datos en cada paso al buscar algo.

- Árbol Rojo-Negro (`std::map`, `std::set`): Es un tipo especial de árbol binario que se "auto-equilibra". Esto evita que el árbol se vuelva una línea larga y delgada, garantizando que siempre sea eficiente buscar en él, sin importar cuántos datos metas.

- Tabla Hash (`std::unordered_map`): Usa una "función hash" que convierte tu clave (como un nombre) en un número índice. Imagina un clasificador de correos donde cada carta va a un buzón específico según su código. Si la función es buena, encuentras lo que buscas casi al instante.

## Tabla de Relación: Contenedor vs. Estructura

|Contenedor C++|Estructura de Datos Interna|Ordenado|¿Permite Duplicados?|
|-|-|-|-|
|`std::vector`|Arreglo Dinámico (Continuo)|Sí (por índice)|Sí|
|`std::list`|Lista Doblemente Enlazada|Sí (por orden de inserción)|Sí|
|`std::set`|Árbol Rojo-Negro|Sí (por valor)|No|
|`std::map`|Árbol Rojo-Negro|Sí (por clave)|No (claves únicas)|
|`std::unordered_set`|Tabla Hash|No|No|
|`std::unordered_map`|Tabla Hash|No|No (claves únicas)|

## Ejemplo de uso: `std::set`

El set es extremadamente útil cuando quieres eliminar duplicados automáticamente o simplemente saber si algo existe en una lista sin importar el orden en que lo agregaste.

Imagina que estás procesando una lista de IDs de usuarios que entraron a un sitio y quieres saber cuántos usuarios únicos hubo:

```cpp
#include <iostream>
#include <set>
#include <string>

int main() {
    // 1. Creamos el set
    std::set<std::string> invitados;

    // 2. Insertamos elementos (algunos repetidos)
    invitados.insert("Carlos");
    invitados.insert("Ana");
    invitados.insert("Beto");
    invitados.insert("Carlos"); // Este no se agregará de nuevo

    // 3. Verificar si alguien está en la lista (Búsqueda rápida)
    if (invitados.count("Ana")) {
        std::cout << "Ana está en la lista." << std::endl;
    }

    // 4. Imprimir la lista
    // Notarás que salen en orden alfabético automáticamente
    std::cout << "Lista de invitados únicos:" << std::endl;
    for (const auto& nombre : invitados) {
        std::cout << "- " << nombre << std::endl;
    }

    return 0;
}
```

Puntos clave del ejemplo:

- Aunque insertamos a "Carlos" dos veces, el `set` solo guarda uno.

- Al imprimir, verás: Ana, Beto, Carlos. El `set` los ordenó por ti (gracias al árbol rojo-negro).

- `invitados.count("Nombre")` devuelve 1 si existe y 0 si no. Es la forma más fácil de buscar.

## Requisitos

Para que un contenedor como std::set o std::map pueda organizar sus elementos en un árbol rojo-negro (que, como vimos, es un árbol binario de búsqueda auto-equilibrado), necesita saber si un elemento es "menor" que otro para decidir si lo coloca a la izquierda o a la derecha.

### El Operador Clave: < (Menos que)

Por defecto, los contenedores asociativos de C++ utilizan la plantilla std::less<T>, que a su vez invoca al operador de menor que (operator<).

Si intentas meter una clase personalizada (por ejemplo, una clase Jugador) en un std::set y no has definido este operador, el compilador lanzará un error bastante largo porque no sabe cómo ordenar a los jugadores.

### Cómo implementarlo en tu propia Clase

Imagina que tienes una clase Punto y quieres guardarlos en un set. Debes definir el operador dentro de la clase de la siguiente manera:
C++

```cpp
#include <iostream>
#include <set>

class Punto {
public:
    int x, y;

    Punto(int _x, int _y) : x(_x), y(_y) {}

    // Definición del operador < (debe ser const)
    bool operator<(const Punto& otro) const {
        if (x != otro.x) {
            return x < otro.x; // Primero comparamos por X
        }
        return y < otro.y;     // Si X es igual, comparamos por Y
    }
};

int main() {
    std::set<Punto> nubeDePuntos;
    
    nubeDePuntos.insert(Punto(10, 5));
    nubeDePuntos.insert(Punto(2, 8));
    nubeDePuntos.insert(Punto(10, 2));

    for (const auto& p : nubeDePuntos) {
        std::cout << "(" << p.x << ", " << p.y << ")" << std::endl;
    }

    return 0;
}
```

### ¿Por qué solo el operador <?

C++ es muy eficiente en esto. Para saber si dos objetos son equivalentes, el contenedor hace una doble comprobación lógica:

- Si `!(a < b)` y `!(b < a)` son ambos verdaderos, entonces el contenedor asume que `a` y `b` son iguales y no insertará el duplicado. No necesita el operador `==`.

### ¿Qué pasa con los No Ordenados (`unordered_map`)?

Aquí la regla cambia totalmente. Como usan una Tabla Hash, no necesitan el orden (no usan `<`), pero necesitan dos cosas diferentes:

1. `operator==`: Para saber si dos objetos que cayeron en el mismo "buzón" de la tabla hash son idénticos.

2. Una función Hash: Una función que convierta tu objeto en un número entero (`size_t`).

Resumen de requisitos:

- `map` / `set`: `Requieren operator<`.

- `unordered_map` / `unordered_set`: Requieren `operator==` y una especialización de `std::hash`.

## La Magia de las Tablas Hash (`unordered_map`)

"¿Cómo busco el hash sin tener que buscarlo?". La clave es que el hash no se busca, se calcula y se usa como una dirección directa.

- ¿Cuándo se calcula el hash? Se calcula siempre: tanto al insertar como al buscar. Es una función matemática pura. Si insertas la clave `"Carlos"`, la función hash siempre devolverá, por ejemplo, el número `42`.

- ¿Dónde se guarda? El hash en sí no suele guardarse permanentemente. Lo que existe es un arreglo (bucket array). El número `42` que obtuvimos es el índice de ese arreglo.

- El acceso constante (O(1)): No recorres la tabla buscando el `42`. Vas directamente a la posición `42` de la memoria (como en un vector). Es como tener una hilera de buzones numerados; si sabes que tu carta está en el buzón 42, no miras el 1, 2, 3... vas directo al 42.

- ¿Están ordenados? No. En la memoria, los elementos están donde la función hash decida. Por eso, al recorrer un `unordered_map`, los elementos salen en un orden que parece aleatorio y que puede cambiar si la tabla crece.

## Inserción en un Árbol Binario y Reordenamiento

En un árbol binario de búsqueda (BST) estándar:

1. Comparas el nuevo elemento con la raíz.

2. Si es menor, vas a la izquierda; si es mayor, a la derecha.

3. Repites hasta encontrar un espacio vacío.

¿Se reordenan?

- En un árbol binario simple: No. Los elementos se quedan donde cayeron. El problema es que si insertas 1, 2, 3, 4, el árbol se convierte en una línea recta (una lista) y pierde su eficiencia.

- En un Árbol Rojo-Negro (el que usa std::map): Sí, se reordenan. Estos árboles realizan "Rotaciones". Cuando una rama se vuelve muy larga, el árbol "rota" los nodos para que el que estaba abajo suba y el árbol se mantenga equilibrado (bajito y ancho). Esto garantiza que siempre tardes O(logn) en encontrar algo.

## ¿Por qué se llaman "Asociativos"?

Se llaman así porque establecen una asociación lógica entre dos entidades, a diferencia de los contenedores secuenciales (como `vector` o `list`) donde la relación es puramente por posición.

- Contenedor Secuencial: El dato "Carlos" está ahí porque es el elemento número 5. La relación es `Índice -> Valor`.

- Contenedor Asociativo: El dato está ahí porque está vinculado a una Clave. La relación es `Clave -> Valor`.

Incluso el `std::set` es asociativo, aunque no veas un "valor" aparte, porque la clave es el propio dato. La estructura interna se organiza en función del valor del dato mismo para permitir búsquedas rápidas, "asociando" el contenido con su ubicación en la estructura (árbol o hash).

## Resumen de acceso rápido

|Tipo|¿Cómo encuentra el dato?|
|-|-|
|Secuencial|Sabe que es el "quinto" elemento.|
|Ordenado (map)|Sabe que es "mayor que A" y "menor que C" (Árbol).|
|No Ordenado (unordered)|Aplica una fórmula matemática a la clave y va directo a la dirección resultante.|

## ¿Cuál es el método óptimo para encontrar un elemento en un `map` o un `set`?

Esa pregunta es el clásico "termómetro" para ver si conoces la arquitectura interna de la Standard Template Library (STL) de C++. Iterar funciona, pero en un entorno de alto rendimiento como el financiero o automotriz, iterar un contenedor que no está diseñado para eso es como usar un desarmador para clavar un clavo: funciona, pero es ineficiente.

Aquí está la respuesta que separa a un programador que "le pica" de uno que entiende la estructura de datos:

1. La respuesta "Senior": El método `.find()`

Tanto `std::map` como `std::set` tienen su propio método miembro llamado `find()`.

Por qué es óptimo: Estos contenedores suelen estar implementados como Árboles Rojo-Negros (Red-Black Trees). Estar iterando con un for te da una complejidad lineal O(n), mientras que el método `.find()` usa la estructura del árbol para darte una complejidad logarítmica O(logn).

Diferencia abismal: Si tienes 1,000,000 de elementos, iterar te toma un millón de pasos. `.find()` lo hace en aproximadamente 20 pasos.

```cpp
auto it = miMapa.find(llave_a_buscar);

if (it != miMapa.end()) {
    // ¡Lo encontré! 
    // En un mapa, accedes al valor con it->second
} else {
    // No existe en el contenedor
}
```

2. El "Bonus" de C++20: `.contains()`

Si solo necesitas saber si el elemento está o no está (sin recuperar el valor), C++20 introdujo un método mucho más limpio:

```cpp
if (miSet.contains(elemento)) {
    // Existe, haz algo...
}
```

Es más legible y sigue siendo O(logn).

3. Es importante entender la diferencia entre un Algoritmo Genérico y un Método Miembro:

- Algoritmo Genérico (std::find): Viene en `<algorithm>`. Funciona con cualquier cosa (vectores, listas), pero como no conoce la estructura interna, itera. Es O(n).

- Método Miembro (map::find): Viene dentro de la clase. Conoce que es un árbol balanceado y salta directamente entre nodos. Es O(logn).

> **Regla de oro en C++**: Si el contenedor tiene un método con el mismo nombre que un algoritmo de la librería estándar (como find, count o lower_bound), usa siempre el del contenedor. Es más rápido.

4. ¿Y si fuera un unordered_map?

Si el mapa es "unordered" (basado en Hash Tables), el método .find() es todavía más rápido: O(1) (tiempo constante). Es casi instantáneo sin importar el tamaño.

## ¿Cuándo usar cada contenedor?

Elegir el contenedor correcto es la diferencia entre una aplicación que vuela y una que se arrastra.

1. `std::vector` (El Rey de los Contenedores)

    Cuándo usarlo: Casi siempre. Es la opción por defecto en C++. Úsalo si necesitas acceso rápido por índice y tus inserciones son mayormente al final.

    Aplicación Real: Sistema de partículas en un videojuego. Si tienes 10,000 balas o chispas en pantalla, el `vector` es el más rápido porque los datos están juntos en memoria (caché friendly), lo que permite al procesador procesarlos masivamente sin saltos.

2. `std::list` (La Especialista)

    Cuándo usarlo: Solo si vas a insertar o eliminar elementos frecuentemente en medio de la colección y ya tienes un iterador apuntando ahí.

    Aplicación Real: Historial de "Deshacer" (Undo) en un editor. Si tienes una lista de 500 acciones y el usuario borra una rama de acciones intermedias, la `list` permite "desenganchar" esos nodos y re-conectar los extremos instantáneamente sin mover todos los demás elementos en memoria.

3. `std::set` (El Filtro Ordenado)

    Cuándo usarlo: Cuando necesitas que no haya duplicados y que los elementos estén siempre ordenados.

    Aplicación Real: Índice de palabras de un libro. Si procesas un texto y quieres una lista de todas las palabras únicas que aparecieron, ordenadas alfabéticamente para un glosario final, el set lo hace solo mientras insertas.

4. `std::map` (El Diccionario Jerárquico)

    Cuándo usarlo: Cuando necesitas asociar una clave con un valor y quieres poder recorrerlos en orden (por ejemplo, de la A a la Z).

    Aplicación Real: Sistema de configuración de un software. Guardas `clave: "brillo" -> valor: 80`. Al ser un `map`, cuando el usuario abre el menú de opciones, estas ya aparecen ordenadas alfabéticamente por nombre de categoría.

5. `std::unordered_set` (El Detector de Presencia)

    Cuándo usarlo: Cuando solo te importa saber si algo "existe" o no, no te importa el orden y quieres la máxima velocidad posible (O(1)).

    Aplicación Real: Filtro de SPAM o "Lista Negra". Tienes una base de datos con millones de correos marcados como basura. Cuando llega un correo nuevo, necesitas saber instantáneamente si está en la lista negra. El `unordered_set` te da la respuesta sin tener que buscar en un árbol.

6. `std::unordered_map` (La Base de Datos de Alto Rendimiento)

    Cuándo usarlo: Cuando tienes una relación clave-valor y el rendimiento es crítico (búsquedas constantes). Es el "Go-to" para bases de datos en memoria.

    Aplicación Real: Sistema de Inventario en un MMO (Juego en línea). El servidor recibe el ID de un objeto (`12345`) y debe devolver instantáneamente sus propiedades (daño, peso, nombre). Con millones de peticiones por segundo, el acceso por tabla hash (O(1)) es vital.

Resumen rápido:

|Contenedor|Prioridad|Estructura|Fortaleza|
|-|-|-|-|
|`vector`|Alta|Arreglo|Velocidad de acceso y memoria.|
|`list`|Baja|Lista Enlazada|Inserciones baratas en cualquier punto.|
|`set/map`|Media|Árbol (Rojo-Negro)|Siempre ordenados, sin duplicados.|
|`unordered_*`|Alta|Tabla Hash|Búsqueda instantánea (la más rápida).|

## Notación Big O

Cuando hablamos de O(n) u O(log n), nos referimos a la Notación Big O. Es una forma de describir el comportamiento o la tasa de crecimiento del tiempo de ejecución (o el uso de memoria) de un algoritmo, en relación con el tamaño de la entrada.

Piensa en ello como una forma de responder a la pregunta: "Si mi entrada crece, ¿cuánto más lento se vuelve mi algoritmo?"

Vamos a explicarlo con analogías y ejemplos.

### El Concepto Clave: El "Orden" de Magnitud

La "O" viene de "Orden" (del alemán Ordnung). No nos interesa el tiempo exacto en segundos (que depende de la máquina), sino la tendencia general.

### Los casos más comunes explicados con analogías

Imagina que tienes un libro de 1000 páginas (`n = 1000`).

1. O(1) - Tiempo Constante

- Significado: El algoritmo siempre toma el mismo tiempo, sin importar el tamaño de la entrada.

- Analogía: Quieres leer el número de la página 735. Lo buscas directamente. No importa si el libro tiene 100 páginas o 1,000,000, siempre tomará el mismo tiempo encontrar esa página si sabes el número.

- Ejemplo en código: Acceder a un elemento de un array por su índice (`mi_array[735]`).

2. O(log n) - Tiempo Logarítmico

- Significado: El tiempo aumenta muy lentamente a medida que la entrada crece. Cada paso del algoritmo reduce drásticamente el tamaño del problema.

- Analogía: Quieres encontrar una palabra en un diccionario (ordenado alfabéticamente). Abres el libro por la mitad, ves que la palabra que buscas está "después", descartas toda la mitad anterior. Vuelves a abrir por la mitad de la parte restante, y así sucesivamente. Cada paso divides el problema por la mitad. No necesitas revisar página por página.

- Ejemplo en código: Búsqueda binaria en un array ordenado.

3. O(n) - Tiempo Lineal

- Significado: El tiempo de ejecución crece de forma directamente proporcional al tamaño de la entrada. Si la entrada se duplica, el tiempo se duplica.

- Analogía: Quieres encontrar una hoja suelta con una nota adhesiva roja en un libro desordenado. La única forma es revisar página por página de principio a fin. Si el libro tiene el doble de páginas, te tomará el doble de tiempo.

- Ejemplo en código: Un bucle for simple que recorre todos los elementos de una lista para buscar uno.

4. O(n log n) - Tiempo Log-Lineal

- Significado: Es un poco más lento que el lineal, pero mucho más rápido que el cuadrático. Es el "santo grial" para la mayoría de los algoritmos de ordenamiento.

- Analogía: Quieres ordenar una baraja de cartas desordenada. Los buenos métodos (como mezclar y partir) tienen este costo. Es más rápido que revisar cada posible par de cartas (O(n²)).

- Ejemplo en código: Los algoritmos de ordenamiento eficientes como Merge Sort y Quick Sort (en su caso promedio).

5. O(n²) - Tiempo Cuadrático

- Significado: El tiempo crece de forma exponencial con la entrada. Si la entrada se duplica, el tiempo se cuadruplica (se multiplica por 4). Esto se vuelve inmanejable rápidamente con datos grandes.

- Analogía: Quieres comprobar si en una fiesta de 10 personas hay dos que cumplen años el mismo día. La forma más ingenua es preguntarle a cada persona por su fecha y compararla con las de todos los demás. Con 10 personas son 100 preguntas (10*10). Con 20 personas son 400 preguntas, y la fiesta se vuelve un caos.

- Ejemplo en código: Dos bucles for anidados que iteran sobre la misma lista.

### Tabla Comparativa Visual

|Notación|Nombre|Crecimiento|Ejemplo|
|-|-|-|-|
|O(1)|Constante|🟢 Excelente|Acceso a un elemento de un array|
|O(log n)|Logarítmico|🟢 Muy Bueno|Búsqueda binaria|
|O(n)|Lineal|🟡 Bueno|Búsqueda simple en una lista|
|O(n log n)|Log-Lineal|🟠 Aceptable|Merge Sort, Quick Sort (promedio)|
|O(n²)|Cuadrático|🔴 Malo|Burbujeo (Bubble Sort), bucles anidados|

### La Gráfica lo dice todo

Si pudieras verlo en una gráfica donde el eje X es el tamaño de la entrada y el eje Y es el tiempo, verías esto:

- O(1) es una línea horizontal plana.

- O(log n) es una curva que crece pero se aplana rápidamente.

- O(n) es una línea diagonal recta.

- O(n log n) es una curva ligeramente más pronunciada que la lineal.

- O(n²) es una curva que se dispara hacia arriba como un cohete.

### Resumen Final

Cuando ves O(n) u O(log n), estás viendo la "etiqueta de eficiencia" de un algoritmo:

- La 'n' siempre es el tamaño de tus datos.

- La expresión dentro del paréntesis te dice cómo se comporta el tiempo a medida que 'n' se hace grande.

- El objetivo siempre es intentar que esa expresión crezca lo más lentamente posible.

¿Tiene más sentido ahora? Es la herramienta fundamental para elegir el algoritmo correcto para el problema correcto, especialmente cuando trabajamos con grandes volúmenes de información.

