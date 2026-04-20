# `static_assert`

`static_assert` es de esas herramientas que parecen simples, pero en práctica te evitan bugs caros antes de que el programa exista.

## 🧠 ¿Qué es `static_assert`?

Es una **verificación en tiempo de compilación**.

> Si la condición es falsa → el código **NO compila**

## 🔧 Sintaxis

```cpp
static_assert(condición, "mensaje de error");
```

## 🔥 Ejemplo básico

```cpp
static_assert(sizeof(int) == 4, "int debe ser de 4 bytes");
```

👉 Si no se cumple → error de compilación

## ⚠️ Diferencia con **assert**

**assert** (runtime)

```cpp
assert(x > 0);
```

- 👉 Se evalúa en ejecución
- 👉 Puede estar desactivado
- 👉 Ya es tarde si falla

## **static_assert** (compile-time)

```cpp
static_assert(x > 0, "x debe ser positivo");
```

- 👉 Se evalúa al compilar
- 👉 No hay programa si falla

## 🧠 Requisito clave

La condición debe ser **constexpr**

```cpp
constexpr int x = 10;

static_assert(x > 5, "ok");
```

## ❌ Esto no sirve

```cpp
int x = 10;

static_assert(x > 5, "");  // ❌
```

👉 porque no es compile-time

## 🔥 Casos donde realmente sirve

1. 🧱 Validar tamaños (MUY importante en sistemas)

```cpp
static_assert(sizeof(uint32_t) == 4, "uint32_t incorrecto");
```

👉 Evita bugs por arquitectura

2. ⚙️ Validar configuraciones

```cpp
constexpr int BUFFER_SIZE = 128;

static_assert(BUFFER_SIZE % 4 == 0, "Debe ser múltiplo de 4");
```

👉 Alineación, DMA, etc.

3. 🔌 Hardware / registros

```cpp
constexpr uint32_t BASE_ADDR = 0x40000000;

static_assert((BASE_ADDR & 0x3) == 0, "Dirección no alineada");
```

4. 🧠 Validar templates

```cpp
template<typename T>
void process() {
    static_assert(sizeof(T) <= 8, "Tipo demasiado grande");
}
```

👉 Evita uso incorrecto

5. 📦 Layout de structs (muy clave en embedded)

```cpp
struct Packet {
    uint8_t header;
    uint16_t value;
};

static_assert(sizeof(Packet) == 3, "Padding inesperado");
```

👉 Detecta padding automático del compilador

## ⚠️ Este caso es MUY real

Sin static_assert:

- 👉 Tu struct puede ser 4 bytes
- 👉 Tu protocolo espera 3
- 👉 Bug silencioso

## 🔥 Uso con `constexpr`

Aquí es donde se vuelve poderoso:

```cpp
constexpr int baudDiv(int clk, int baud) {
    return clk / baud;
}

static_assert(baudDiv(16000000, 9600) > 0, "Baud inválido");
```

👉 Validas lógica antes de ejecutar

## 🧩 Patrón útil

### Validar invariantes

```cpp
constexpr int MAX_USERS = 100;

static_assert(MAX_USERS < 256, "No cabe en uint8_t");
```

## ⚠️ Buenas prácticas

### ✔️ Mensajes claros

```cpp
static_assert(size <= 256, "Buffer excede límite de hardware");
```

👉 El error debe decirte qué rompiste

### ❌ No abuses

```cpp
static_assert(true, "ok");  // inútil
```

## 🧠 Insight importante

`static_assert` es:

> “Documentación ejecutable en compile-time”

## 🎯 En tu contexto (sistemas)

Esto es donde realmente brilla:

- ✔ Validar tamaños
- ✔ Validar alineación
- ✔ Validar configuración
- ✔ Validar restricciones de hardware
- ✔ Validar templates

## 🔥 Ejemplo completo (muy bueno)

```cpp
constexpr int BUFFER_SIZE = 128;
constexpr int ALIGNMENT = 4;

static_assert(BUFFER_SIZE % ALIGNMENT == 0,
              "Buffer no alineado correctamente");

uint8_t buffer[BUFFER_SIZE];
```

👉 Esto evita bugs difíciles de detectar

## 🧠 Conexión con lo anterior

- `constexpr` → calcula en compile-time
- `static_assert` → valida en compile-time

👉 Juntos = código más seguro y eficiente

## 🎯 Resumen directo

- `static_assert` detiene el build si algo está mal
- Solo funciona con condiciones compile-time
- Es clave para sistemas y templates
- Te ahorra debugging en runtime
