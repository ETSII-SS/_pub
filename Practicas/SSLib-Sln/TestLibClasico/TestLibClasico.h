#pragma once
#include <stdio.h>
#include <locale.h>
#include "sslib/SSLib.h"
using namespace ss;
using namespace std;


int TestP5_2025_main(int argc, char* argv[], char* envp[]);
bool NuevoArchivoTest(const char* nombreArchivo, uint64_t tamArchivoMB, size_t tamBuffer);
#ifdef TEST_LIB_CLASICO_CPP
static int BuscaTextoEnArchivos(const char* dirBusqueda, const char* textoABuscar);
#endif

