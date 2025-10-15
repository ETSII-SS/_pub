#pragma once
//Si usamos Boost, hay que poner en 
// Propiedades del proyecto->Depuración->Entorno
// PATH=$(AAComm)\bin;%PATH%
// Uso de _CRT_SECURE_NO_WARNINGS. Ver https://t.ly/aZuv  
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <locale.h>
#include <string>
#include <iostream>
#include <vector>

#include <windows.h>

#include "sslib/SSLib.h"

using namespace std;
using namespace ss;

static size_t  CalculaBytesEnArchivos(char* listaArchivos[], int nroArchivos);
static int  BuscaTextoEnArchivos(const char* dirBusqueda, const char* textoABuscar);
static BOOL MuestraNroArchivosEncontradosEnDirectorio(int encontrados, char* pDir);


