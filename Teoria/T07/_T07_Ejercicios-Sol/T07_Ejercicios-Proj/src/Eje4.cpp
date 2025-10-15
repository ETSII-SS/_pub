#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <locale.h>
#include <Windows.h>
#include "pLibSS.h"

// EJERCICIO 4
//Ejercicio 7.4. 	Escribir el entero 258 en un archivo 
//(y volverlo a leer.
int main(int argc, char *argv[])
{
	HANDLE h;
	DWORD dato = 258;
	DWORD datoI;
	DWORD escritos;

	setlocale(LC_ALL, "Spanish"); // Previo

	h = CreateFile("t.dat", GENERIC_WRITE, 0, NULL,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	CheckError(INVALID_HANDLE_VALUE == h, "...", 1);
	//Escribir el entero 258
	CheckError(FALSE ==
		WriteFile(h, ..., 0)
		|| ¿condición de error escritura?,
		"Error al escribir", 1);
	//CheckError(FALSE == WriteFile(h, &dato, sizeof(dato), &escritos, 0)|| sizeof(dato) != escritos,	
	//"Error al escribir", 1);
	CloseHandle(h);
	
	//Leer*****
	h = CreateFile("t.dat", GENERIC_READ, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	CheckError(INVALID_HANDLE_VALUE == h, "...", 1);
	//Leer el entero 
	CheckError(FALSE ==
		ReadFile(h,...s, 0)
		|| ¿condición de error lectura?,
		"Error al leer", 1);
	//CheckError(FALSE == ReadFile(h, &datoI, sizeof(datoI), &escritos, 0) || sizeof(datoI) != escritos
	//	, "Error al leer", 1);
	CloseHandle(h);



	printf("\nPulse una tecla para terminar\n");  //Solo para depurar. Quitar si no es necesario
	getchar();
	return 0;
}




