#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <locale.h>
#include <Windows.h>
#include "sslib\SSLib.h"	// <- Sustituir por biblioteca privada...

// lista todos los archivos de un directorio
void listaArchivos(char * ruta) {
	WIN32_FIND_DATA FindData;
	HANDLE hFind = INVALID_HANDLE_VALUE;

	CHAR expresionBusqueda[MAX_PATH];

	// construye la expresión de búsqueda
	snprintf(expresionBusqueda, sizeof(expresionBusqueda), "%s\\*", ruta);

	hFind = FindFirstFile(expresionBusqueda, &FindData);
	CheckError(INVALID_HANDLE_VALUE == hFind, 1, "No se pudo hacer la primera búsqueda");
	do	{
		if (FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)		{
			printf("  %s   <DIR>\n", FindData.cFileName);
		}
		else		{
			LARGE_INTEGER filesize;
			filesize.LowPart = FindData.nFileSizeLow;
			filesize.HighPart = FindData.nFileSizeHigh;
			printf("  %s   %ld bytes\n", FindData.cFileName, filesize.QuadPart);
		}
	} while (FindNextFile(hFind, &FindData) != 0);
	CheckError(GetLastError() != ERROR_NO_MORE_FILES, 1, "No se pudo ejecutar FindNextFile");
	FindClose(hFind);
}

int main(int argc, char *argv[])
{
	char rutaDir[MAX_PATH];

	setlocale(LC_ALL, "Spanish");

	// lista todos los archivos del directorio de trabajo
	CheckError(0 == GetCurrentDirectory(MAX_PATH, rutaDir), 1, "No se pudo obtener el directorio de trabajo");
	listaArchivos(rutaDir);

	printf("\nPulse una tecla para terminar\n");
	getchar();
	return 0;
}

