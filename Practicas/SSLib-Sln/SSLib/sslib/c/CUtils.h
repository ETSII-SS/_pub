#pragma once

// Ejemplo de código C++ que puede ser llamado desde C o C++

#ifdef _LIB_EXPORTS_
#define LIB_API __declspec(dllexport)
#else
#define LIB_API __declspec(dllimport)
#endif

#ifdef __cplusplus
// Compilación condicional: esta parte solo se compila si se incluye desde un archivo .cpp
extern "C" {

#else

// Esta parte solo se compila si se incluye desde un archivo .c
#ifndef bool

// Esta parte solo se compila si no está definido el tipo bool (algunos compiladores C no lo tienen)
typedef int bool;

#endif

#endif

bool  LIB_API CheckError(bool condicionError, int codigoSalida, const char* mensajeUsuario, ...);

#ifdef __cplusplus
}	// Tan solo sirve para cerrar la llave del extern "C"
#endif



#include <Windows.h>


