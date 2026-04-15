# C++11

🔹 C++11 – El gran salto moderno 🚀

Este es el cambio más importante en la historia del lenguaje.

Features clave:

- auto → inferencia de tipos
- nullptr
- range-based for
- lambda expressions
- move semantics (rvalue references &&)
- std::thread (multithreading nativo)
- smart pointers (unique_ptr, shared_ptr)
- enum class (type-safe)
- static_assert

> **Impacto real**:
Hace que C++ sea usable como lenguaje moderno, seguro y eficiente sin tanto boilerplate.

## ¿Qué es auto en C++11?

auto le dice al compilador:

“Deduce el tipo de esta variable a partir de la expresión con la que la inicializo”

auto x = 10;        // int
auto y = 3.14;      // double
auto z = x + y;     // double

👉 No es dinámico.
👉 El tipo sigue siendo estático y conocido en compile-time.

⚙️ ¿Por qué existe realmente?

Antes de C++11:

std::vector<int>::iterator it = v.begin();

Con auto:

auto it = v.begin();

👉 Reduce ruido, especialmente con:

iteradores
templates
tipos largos
🔥 Lo importante de verdad (esto es lo que te hace bueno)
1. auto elimina referencias y const por defecto
const int a = 10;
auto x = a;   // int (pierde const)

👉 Esto es CLAVE. Puede introducir bugs si no lo entiendes.

2. Para conservar referencias necesitas &
int a = 10;
auto x = a;    // copia
auto& y = a;   // referencia
3. Para conservar const correctamente
const int a = 10;

auto x = a;        // int
const auto y = a;  // const int
auto& z = a;       // const int&
4. auto con punteros
int* p = &a;

auto x = p;   // int*
auto* y = p;  // int* (más explícito)

👉 auto* es buena práctica cuando quieres dejar claro que es puntero.

5. auto evalúa la expresión completa
auto x = 1 + 2.5;  // double

👉 Sigue las reglas normales de promoción de tipos.

⚠️ Errores comunes (muy comunes)
❌ Error 1: copiar sin querer
std::vector<int> v = {1,2,3};

for (auto x : v) {
    x = 10;  // NO modifica el vector
}

✔️ Correcto:

for (auto& x : v) {
    x = 10;
}
❌ Error 2: perder referencia en APIs
auto val = getReference();  // copia

👉 Si la función devuelve referencia:

auto& val = getReference(); // correcto
❌ Error 3: ocultar tipos importantes
auto config = getHardwareRegister();

👉 En embebidos esto puede ser peligroso si:

no sabes si es volatile
no sabes si es puntero
no sabes el tamaño
⚙️ Uso en sistemas embebidos (aquí está el punto fino)
🟢 Cuándo SÍ usar auto

✔️ Iteradores

for (auto it = buffer.begin(); it != buffer.end(); ++it)

✔️ Expresiones complejas

auto result = compute_crc(data);

✔️ Tipos template-heavy

🔴 Cuándo NO usarlo

❌ Registros de hardware

auto reg = *(volatile uint32_t*)0x40001000; // mala idea

✔️ Mejor:

volatile uint32_t* reg = (uint32_t*)0x40001000;

❌ Interfaces críticas

auto speed = readSensor();

👉 Si el tipo importa (ej: uint16_t vs float), sé explícito.

🧩 Regla práctica (quédate con esto)

👉 Usa auto cuando:

el tipo es obvio
el tipo es irrelevante
o el tipo es demasiado complejo

👉 NO lo uses cuando:

el tipo comunica significado importante
estás cerca del hardware
necesitas control fino
🎯 Nivel pro (esto te diferencia)
auto + referencias en loops eficientes
for (auto& item : container)
const auto& para evitar copias
for (const auto& item : container)

👉 Esto es estándar en código de alto rendimiento.

🧠 Insight importante (mentalidad correcta)

auto no es para escribir menos.
Es para escribir código más robusto frente a cambios de tipo.