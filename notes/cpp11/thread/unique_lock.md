# `std::unique_lock`

`std::unique_lock` es básicamente un **wrapper más flexible para manejar mutexes**. Si `std::lock_guard` es un “candado automático simple”, `unique_lock` es un “candado configurable”.

La diferencia clave:

- 👉 `lock_guard` = simple, siempre bloquea al crear y desbloquea al destruir
- 👉 `unique_lock` = puedes decidir **cuándo bloquear, desbloquear, transferir ownership, etc**.

## 🧠 Idea mental correcta

Un `unique_lock` **posee (own)** un `mutex`.

Pero ese ownership puede:

- adquirirse después
- soltarse antes
- moverse a otro `unique_lock`

## 🔧 Ejemplo básico

```cpp
std::mutex mtx;

void f() {
    std::unique_lock<std::mutex> lock(mtx); // lock aquí
    // sección crítica
} // unlock automático aquí
```

👉 Igual que `lock_guard`… pero esto no es lo interesante.

## 🔓 Lock diferido (deferred locking)

Puedes crear el lock **sin bloquear inmediatamente**:

```cpp
std::unique_lock<std::mutex> lock(mtx, std::defer_lock);

// hacer cosas sin lock...

lock.lock(); // ahora sí bloquea
```

💡 Esto sirve cuando quieres controlar exactamente **cuándo** entrar a la sección crítica.

## 🔄 Unlock manual antes de salir

```cpp
std::unique_lock<std::mutex> lock(mtx);

do_something();

lock.unlock(); // liberas antes del final

do_other_stuff(); // ya sin lock
```

👉 Esto no lo puedes hacer con `lock_guard`.

## 🔁 Transferencia de ownership (move)

```cpp
std::unique_lock<std::mutex> lock1(mtx);

std::unique_lock<std::mutex> lock2 = std::move(lock1);
```

👉 Ahora `lock2` es el dueño del mutex, `lock1` queda vacío.

Esto es útil cuando:

- regresas locks desde funciones
- los pasas entre scopes

## ⏱️ Uso con condition_variable (MUY importante)

Aquí es donde `unique_lock` es obligatorio:

```cpp
std::mutex mtx;
std::condition_variable cv;
bool ready = false;

void worker() {
    std::unique_lock<std::mutex> lock(mtx);

    cv.wait(lock, [] { return ready; });

    // continúa cuando ready == true
}
```

👉 `condition_variable` necesita poder:

- desbloquear el mutex internamente
- volverlo a bloquear

Eso **solo lo permite** `unique_lock`, no `lock_guard`.

## 🔥 Ejemplo más real (producer-consumer)

```cpp
std::queue<int> q;
std::mutex mtx;
std::condition_variable cv;

void producer() {
    {
        std::unique_lock<std::mutex> lock(mtx);
        q.push(42);
    } // unlock aquí

    cv.notify_one();
}

void consumer() {
    std::unique_lock<std::mutex> lock(mtx);

    cv.wait(lock, [] { return !q.empty(); });

    int value = q.front();
    q.pop();
}
```

## ⚠️ Errores comunes

### 1. Usarlo cuando no necesitas flexibilidad

Si solo haces esto:

```cpp
std::unique_lock<std::mutex> lock(mtx);
```

👉 probablemente deberías usar `lock_guard` (más ligero y claro)

### 2. Olvidar que puede estar “desbloqueado”

```cpp
std::unique_lock<std::mutex> lock(mtx, std::defer_lock);

// ERROR si haces esto:
critical_section(); // sin lock real
```

## 🧩 Regla práctica (esto te va a servir en entrevistas)

Usa:

- `lock_guard` → por defecto
- `unique_lock` → cuando necesites:
    - `condition_variable`
    - lock/unlock manual
    - deferred locking
    - mover el lock

## 🧠 Conexión con tu thread pool

En tu caso (thread pool), `unique_lock` es clave aquí:

```cpp
std::unique_lock<std::mutex> lock(mtx);
cv.wait(lock, [this] { return !tasks.empty() || stop; });
```

Porque:

- el hilo se duerme
- el mutex se libera automáticamente mientras espera
- luego se vuelve a adquirir

Sin eso → deadlock o busy waiting.
