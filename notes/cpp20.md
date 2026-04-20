# C++20

C++20 es, sin exagerar, uno de los saltos más importantes del lenguaje desde C++11. Si C++11 “modernizó” el lenguaje, C++20 empieza a hacerlo más usable, expresivo y seguro sin perder su esencia de control y performance.

## 🧠 Main features de C++20 (vista rápida)

- Concepts
- Ranges (biblioteca de rangos)
- Coroutines
- Modules
- Three-way comparison (operator `<=>`)
- constexpr mejorado (mucho más potente)
- [consteval](cpp20/consteval.md) y [constinit](cpp20/constinit.md)
- Designated initializers
- Template improvements (como lambdas en templates, NTTPs más flexibles)
- Calendar y Time Zones (chrono expandido)
- Format library (`std::format`)
- span (vista sobre memoria continua)
- bit operations (`<bit>`)
- atomic smart pointers y mejoras en concurrencia
- Stop tokens y jthread (mejor manejo de threads)

## 📈 Impacto general de C++20

C++20 no es solo “más features”, cambia cómo escribes C++ en varios niveles:

1. Más cercano a lenguajes modernos

    Reduce mucho el “ruido” típico de C++. Cosas que antes eran verbosas o propensas a errores ahora son más declarativas.

2. Mejor seguridad en tiempo de compilación

    Con cosas como Concepts y mejoras en `constexpr`, puedes mover más lógica al compile-time y detectar errores antes.

3. Mejor manejo de abstracciones complejas

    Ranges y Coroutines hacen que trabajar con datos y asincronía sea mucho más limpio, acercándose a lenguajes como Python o C# pero sin perder control.

4. Ataque directo a problemas históricos

    - Modules buscan reemplazar el caos de headers/includes
    - `<=>` simplifica operadores
    - `std::format` reemplaza printf/iostreams viejos

5. Adopción más lenta en la industria (pero inevitable)

    Aquí te doy una visión realista (no marketing):

    - Muchos proyectos grandes siguen en C++14/17
    - Modules y Coroutines aún tienen fricción en toolchains
    - Pero features como `span`, `format`, `constexpr` improvements sí se adoptan rápido
