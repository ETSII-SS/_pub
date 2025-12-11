#pragma once
/*
Añadir a:
Propiedades del proyecto->Depuración->Entorno 
 (todas las configuraciones, todas las plataformas)
 La línea:

 PATH=$(AAComm)bin\sslib\(PlatformTarget);%PATH%
 */

 // Uso de _CRT_SECURE_NO_WARNINGS. Ver https://t.ly/aZuv  
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <locale.h>

#include <windows.h>
#include <sslib/SSLib.h>

#include "GestorArchivos.h"
#include "VsSolutionTest.h"

enum {
	ErrorFaltanArgumentos = 1,
	ErrorSobranArgumentos,

};
using namespace std;
