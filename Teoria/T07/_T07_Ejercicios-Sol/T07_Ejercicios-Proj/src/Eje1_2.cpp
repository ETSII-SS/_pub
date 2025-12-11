#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <locale.h>
#include <Windows.h>
#include "sslib\SSLib.h"	

int main(int argc, char *argv[])
{

	HANDLE hArchivo;

	hArchivo = CreateFile("Hola.txt",
		GENERIC_READ |GENERIC_WRITE,
		0, // No compartido
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL, NULL);
	CheckError(hArchivo == INVALID_HANDLE_VALUE, 1, "No se pudo abrir el archivo");

#if  TRUE
	// Ejercicio 2: ¿Dónde están los errores en este código
	char Cadena[150];
	snprintf(Cadena, sizeof(Cadena), "Hola Mundo");
	DWORD Escritos;
	BOOL res;
		
	res=WriteFile(hArchivo, // Handle del archivo
		Cadena,		// Dirección de inicio del buffer
		sizeof(Cadena), // Tamaño del buffer
		&Escritos,	// <- Aquí devuelve cuánto se ha escrito realmente
		NULL);	// <- Solo para escrituras asincronas. Solo usaremos operaciones síncronas. 
	CheckError(FALSE==res || sizeof(Cadena)!= Escritos, 1, "No se pudo escribir en el archivo");
	
#endif
	CloseHandle(hArchivo);
	hArchivo = NULL; // Poner aquí un breakpoint y ver que el archivo se crea, y no lo podemos abrir desde otra aplicación,
	

	printf("\nPulse una tecla para terminar\n");  //Solo para depurar. Quitar si no es necesario
	getchar();
	return 0;
}




