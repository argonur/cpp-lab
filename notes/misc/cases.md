# Convenciones típicas en C++

Muchos proyectos usan algo así:

|Elemento|Convención|
|-|-|
|clases|PascalCase|
|funciones|camelCase|
|variables|snake_case o camelCase|
|constantes|SCREAMING_SNAKE_CASE|
|miembros privados|m_value o _value|

Ejemplo:

```cpp
class RiskEngine
{
private:
    double m_current_exposure;

public:
    double calculateRisk();
};
```

💡 Consejo práctico (importante):

No existe una convención universal.
Lo importante es la consistencia dentro del proyecto.