#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <locale.h>
#include <Windows.h>
#include "pLibSS.h"

#define BUF_SIZE 16384  /* Optimal in several experiments. Small values such as 256 give very bad performance */


// EJERCICIO 9:
void GeneraNombreArchivoTmp(char * pNombre) {
	char rutaTmp[MAX_PATH];
	CheckError(0 == GetTempPath(MAX_PATH, rutaTmp), "No se pudo obtener la ruta de archivos temporales", 1);
	UINT res = GetTempFileName(rutaTmp, "Pre", 0, pNombre);
	CheckError(0 == res || ERROR_BUFFER_OVERFLOW == res, "No se pudo obtener un nombre de archivo temporal", 1);
}

int main(int argc, char *argv[])
{

	HANDLE hIn, hOut;
	DWORD bytesLeidos, bytesEscritos;
	CHAR buffer[BUF_SIZE];    // <-- Reserva hueco.
	CHAR nombreArchTmp[MAX_PATH];    // Nombre del archivo temporal de salida

	BOOL Estado;

	setlocale(LC_ALL, "Spanish"); // Previo
	CheckError(2 != argc, "Debe haber 1 argumento: nombre_archivo", 1);

	GeneraNombreArchivoTmp(nombreArchTmp);
	// Abrir archivo de lectura
	hIn = CreateFile(argv[1], GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING,    // Lo abre si existe, si no existe falla
		FILE_ATTRIBUTE_NORMAL, NULL);
	CheckError(INVALID_HANDLE_VALUE == hIn, "No se pudo abrir el archivo de entrada", 1);

	// Abrir archivo de escritura
	hOut = CreateFile(nombreArchTmp, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING,    // <-- Se crea si no existe, si existe se sobreescribe
		FILE_ATTRIBUTE_NORMAL, NULL);
	CheckError(INVALID_HANDLE_VALUE == hOut, "No se pudo abrir el archivo de salida", 1);
	bytesLeidos = 1; // Esto es solo para forzar la primera entrada en el bucle while
	DWORD upLow = 0;
	while (bytesLeidos > 0) {
		Estado = ReadFile(hIn, buffer, BUF_SIZE, &bytesLeidos, NULL);    // Lee del archivo de entrada
		CheckError(FALSE == Estado, "No se ha leido correctamente", 1);

		// Proceso de los datos...
		if (upLow == 0) {
			if (buffer[0] >= 'A' && buffer[0] <= 'Z')
				upLow = LCMAP_LOWERCASE;
			else
				upLow = LCMAP_UPPERCASE;
		}
		LCMapString(LOCALE_USER_DEFAULT, upLow, buffer, bytesLeidos, buffer, bytesLeidos);


		Estado = WriteFile(hOut, buffer, bytesLeidos, &bytesEscritos, NULL);   	 // Escribe en el archivo de salida
		CheckError(FALSE == Estado && bytesLeidos != bytesEscritos, "No se ha escrito correctamente", 1);
	}

	// Cerrar archivos
	CloseHandle(hIn);
	CloseHandle(hOut);

	// Qué hacer con el archivo temporal? Normalmente borrarlo cuando ya no se necesite
	//CheckError(FALSE == DeleteFile(nombreArchTmp), "No se pudo borrar el archivo temporal", 1);
	printf("\nPulse una tecla para terminar\n");  //Solo para depurar. Quitar si no es necesario
	getchar();
	return 0;
}




