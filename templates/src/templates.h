#pragma once

/*
    Este archivo contiene plantillas que pueden ser utilizadas en todo el proyecto.
    Las plantillas permiten escribir código genérico que puede trabajar con diferentes tipos de datos.
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