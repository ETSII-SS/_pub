#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <locale.h>
#include <Windows.h>
#include "pLibSS.h"

// EJERCICIO 3

int main(int argc, char *argv[])
{
	HANDLE hArchivo;

	hArchivo = CreateFile("Hola.txt",GENERIC_READ, FILE_SHARE_READ, NULL,	CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	CheckError(hArchivo == INVALID_HANDLE_VALUE, "No se pudo abrir el archivo", 1);

	// ¿Cómo podemos probar el efecto de dwSharedMode desde el mismo programa? FILE_SHARE_READ | FILE_SHARE_WRITE	


	CloseHandle(hArchivo); // Poner aquí un breakpoint y ver que el archivo se crea, y no lo podemos abrir desde otra aplicación,
	hArchivo = NULL;

	printf("\nPulse una tecla para terminar\n");  //Solo para depurar. Quitar si no es necesario
	getchar();
	return 0;
}




