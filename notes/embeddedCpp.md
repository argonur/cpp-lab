# Embedded Programming

## C++ enfocado a embedded

### Memoria

- Stack vs heap
- new/delete vs allocadores custom
- Fragmentación (muy importante en embedded)
- RAII (pero aplicado a recursos reales: mutex, file handles, periféricos)

### Control fino del lenguaje

- const, constexpr
- [volatile (clave para hardware, registros, ISR)](embedded/volatile.md)
- static (storage duration, linkage)
  - [static fuera de clases](embedded/staticFuera.md)
  - [static en clases](embedded/staticEnClases.md)
  - [storage duration](embedded/storageDuration.md)
- Punteros vs referencias
- [extern](embedded/extern.md)

### Bitwise operations

- Máscaras, shifts (<<, >>)
- Manipulación de registros

### Estructuras de datos sin STL pesado

- Arrays, buffers circulares
- Evitar STL dinámico en sistemas críticos (o saber cuándo sí/no)

### Zero-cost abstractions

- Templates básicos
- Inline functions

## Fundamentos de sistemas (esto separa juniors de seniors)

### Cómo funciona la memoria realmente

- Endianness
- Alignment / padding
- Layout de structs

### Compilación y linking

- .text, .data, .bss
- Linker scripts (aunque sea superficial)

### Interrupciones (ISR)

- Qué puedes y NO puedes hacer dentro de una ISR
- Latencia

### Concurrencia

- Race conditions
- Mutex vs spinlock
- Atomic operations

## Embedded puro

### Microcontroladores

- Diferencia entre MCU vs MPU

### Periféricos

- UART, SPI, I2C
- Timers
- ADC

### Acceso a registros

- Memory-mapped I/O

### RTOS (si aplica)

- Tasks, scheduling
- Prioridades
- Deadlocks
