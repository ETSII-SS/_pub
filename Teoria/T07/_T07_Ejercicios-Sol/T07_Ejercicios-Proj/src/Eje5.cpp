#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <locale.h>
#include <Windows.h>
#include "sslib\SSLib.h"

#define BUF_SIZE 16384  /* Optimal in several experiments. Small values such as 256 give very bad performance */

// EJERCICIO 5:
int main(int argc, char *argv[])
{

	HANDLE hIn, hOut;
	DWORD bytesLeidos, bytesEscritos;
	CHAR buffer[BUF_SIZE];    // <-- Reserva hueco.
	BOOL Estado;

	setlocale(LC_ALL, "Spanish"); // Previo

	CheckError(3 != argc, 1, "Debe haber 2 argumentos: archivo_salda  archivo_entrada");

	// Abrir archivo de lectura
	hIn = CreateFile(argv[2], GENERIC_READ, 0, NULL, OPEN_EXISTING,    // Lo abre si existe, si no existe falla
		FILE_ATTRIBUTE_NORMAL, NULL);
	CheckError(INVALID_HANDLE_VALUE == hIn, 1, "No se pudo abrir el archivo de entrada");
	// Abrir archivo de escritura
	hOut = CreateFile(argv[1], GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,    // <-- Se crea si no existe, si existe se sobreescribe
		FILE_ATTRIBUTE_NORMAL, NULL);
	CheckError(INVALID_HANDLE_VALUE == hOut, 1, "No se pudo abrir el archivo de salida");
	bytesLeidos = 1; // Esto es solo para forzar la primera entrada en el bucle while
	while (bytesLeidos > 0) {
		Estado = ReadFile(hIn, buffer, BUF_SIZE, &bytesLeidos, NULL);    // Lee del archivo de entrada
		CheckError(FALSE == Estado, 1, "No se ha leido correctamente");
		Estado = WriteFile(hOut, buffer, BUF_SIZE, &bytesEscritos, NULL);   	 // Escribe en el archivo de salida
		CheckError(FALSE == Estado && BUF_SIZE != bytesEscritos, 1, "No se ha escrito correctamente");
	}

	// Cerrar archivos
	CloseHandle(hIn);
	CloseHandle(hOut);


	printf("\nPulse una tecla para terminar\n");  //Solo para depurar. Quitar si no es necesario
	getchar();
	return 0;
}




