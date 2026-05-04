# RAII

**RAII (Resource Acquisition Is Initialization)** es un idiom/patrón fundamental de C++ donde la adquisición y liberación de recursos se liga automáticamente al ciclo de vida de un objeto.

La idea central es:

- El recurso se adquiere en el constructor.
- El recurso se libera en el destructor.
- La duración del recurso queda controlada por el scope del objeto.

Esto permite manejo automático y seguro de recursos, incluso cuando ocurren excepciones o returns tempranos.

Recursos típicos:

- Memoria dinámica (new/delete)
- Mutexes
- File handles
- Sockets
- Conexiones a bases de datos
- Threads
- Cualquier recurso que necesite liberación manual

Ejemplo clásico:

```cpp
{
    std::lock_guard<std::mutex> lock(mtx);

    // mutex bloqueado aquí

} // destructor de lock_guard libera el mutex automáticamente
```

Ventajas principales:

- Evita fugas de recursos
- Hace el código exception-safe
- Reduce complejidad manual
- Hace explícita la ownership del recurso
- Aprovecha el manejo automático de scopes en C++

RAII es una de las razones por las que C++ moderno puede manejar recursos de forma extremadamente robusta y eficiente sin garbage collector.