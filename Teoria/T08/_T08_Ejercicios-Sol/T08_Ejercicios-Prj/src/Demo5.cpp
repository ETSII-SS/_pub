#define _CRT_SECURE_NO_WARNINGS
#include "pLibSS.h"

#define TAM_BLOQUE (1024*1024)

int main(int argc, char *argv[])
{
	HANDLE hFile;
	HANDLE hMap;
	DWORD TamArchivo;
	char * pView;

	setlocale(LC_ALL, "Spanish"); // Previo


	hFile = CreateFile("in.BMP", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	CheckError(hFile == INVALID_HANDLE_VALUE, "ERROR abriendo archivo para escribir", 1);
	TamArchivo = GetFileSize(hFile, NULL);

	//> Crea el mapa
	hMap = CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, TamArchivo, NULL);
	CheckError(NULL == hMap, "ERROR ejecutando CreateFileMapping", 1);
	//> Crea vista del archivo (= reserva páginas en memoria)
	pView = (char *)MapViewOfFile(hMap, FILE_MAP_ALL_ACCESS, 0, 0, TamArchivo);
	CheckError(NULL == pView, "ERROR ejecutando MapViewOfFile", 1);
	// A partir de aquí ya puedo usar el puntero directamente...

		DWORD MaxX, MaxY, *pDWORD;
		pDWORD = (DWORD *) &pView[18];				// pDWORD lo uso para cargar direcciones de datos DWORD
		MaxX = *pDWORD; //*((DWORD*)(&pView[18]))	// alternativa, usar un casting sobre pView
		pDWORD = (DWORD *) &pView[18+4];
		MaxY = *pDWORD; //*((DWORD*)(&pView[18]))

		//		// Leer MaxX y MaxY
//		// ver campos biWidth y biHeight en https://msdn.microsoft.com/en-us/library/windows/desktop/dd183376%28v=vs.85%29.aspx
//		Movido = SetFilePointer(hArchivo, 18, NULL, FILE_BEGIN);
//		CheckError(INVALID_SET_FILE_POINTER == Movido, "Error al ejecutar SetFilePointer)", 1);
//		CheckError(FALSE == ReadFile(hArchivo, &MaxX, sizeof(MaxX), &Movido, NULL),
//			"Error: No se ha podido leer MaxX", 1);
//		CheckError(FALSE == ReadFile(hArchivo, &MaxY, sizeof(MaxY), &Movido, NULL),
//			"Error: No se ha podido leer MaxY", 1);
//		// ir al inicio de la matriz de pixel.
#define TAM_CABECERAS (sizeof( BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER))
		//pView[TAM_CABECERAS] = 0;
		//pView[TAM_CABECERAS+1] = 0;
		//pView[TAM_CABECERAS+2] = 0;
		memset(&pView[TAM_CABECERAS], 0, 3);
		//;
//		Movido = SetFilePointer(hArchivo, TAM_CABECERAS, NULL, FILE_BEGIN);
//		CheckError(INVALID_SET_FILE_POINTER == Movido, "Error al ejecutar SetFilePointer)", 1);
//
//		// escribir el punto negro
//		CheckError(FALSE == WriteFile(hArchivo, &cero, 3, &Movido, NULL) || Movido != 3,
//			"Error al escribir pixel", 1);
//		// ir al final

		int tamImagen;
		if (MaxX * 3 % 4 == 0)
			tamImagen = MaxX * 3 * MaxY;
		else
			tamImagen = (MaxX * 3 + 4 - (MaxX * 3 % 4)) *MaxY;

		memset(&pView[TAM_CABECERAS + tamImagen -3], 0, 3);

	//>Libera vista y mapa
	UnmapViewOfFile(pView);
	CloseHandle(hMap);
	CloseHandle(hFile);



	printf("\nPulse una tecla para terminar\n");  //Solo para depurar. Quitar si no es necesario
	getchar();
	return 0;
}



int main_ejercio9_Tema5(int argc, char *argv[])
{
	HANDLE hArchivo;
	DWORD Movido;
	DWORD MaxX, MaxY;
	DWORD cero = 0;
	setlocale(LC_ALL, "Spanish"); // Previo

	CheckError(argc != 2, "Es necesario poner el nombre del archivo en la línea de comandos", 1);


	// Comprueba que es bitmap de 24 bits
	// PENDIENTE: si no es BMP da un mensaje y cierra el programa


	// Abre el archivo	
	hArchivo = CreateFile(argv[1], GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	CheckError(INVALID_HANDLE_VALUE == hArchivo, "ERROR: No se pudo abrir el archivo", 1);


	// Dos métodos para leer la info de cabecera: 
	// 1) Leer las cabeceras completas.
	// 2) leer sólo los campos que interesan.

	// Usaremos el método 2) para mostrar cómo se usa SetFilePointer...


	// Leer MaxX y MaxY
	// ver campos biWidth y biHeight en https://msdn.microsoft.com/en-us/library/windows/desktop/dd183376%28v=vs.85%29.aspx
	Movido = SetFilePointer(hArchivo, 18, NULL, FILE_BEGIN);
	CheckError(INVALID_SET_FILE_POINTER == Movido, "Error al ejecutar SetFilePointer)", 1);
	CheckError(FALSE == ReadFile(hArchivo, &MaxX, sizeof(MaxX), &Movido, NULL),
		"Error: No se ha podido leer MaxX", 1);
	CheckError(FALSE == ReadFile(hArchivo, &MaxY, sizeof(MaxY), &Movido, NULL),
		"Error: No se ha podido leer MaxY", 1);
	// ir al inicio de la matriz de pixel.
#define TAM_CABECERAS (sizeof( BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER))

	Movido = SetFilePointer(hArchivo, TAM_CABECERAS, NULL, FILE_BEGIN);
	CheckError(INVALID_SET_FILE_POINTER == Movido, "Error al ejecutar SetFilePointer)", 1);

	// escribir el punto negro
	CheckError(FALSE == WriteFile(hArchivo, &cero, 3, &Movido, NULL) || Movido != 3,
		"Error al escribir pixel", 1);
	// ir al final
	int tamImagen;
	if (MaxX * 3 % 4 == 0)
		tamImagen = MaxX * 3 * MaxY;
	else
		tamImagen = (MaxX * 3 + 4 - (MaxX * 3 % 4)) *MaxY;

	Movido = SetFilePointer(hArchivo, TAM_CABECERAS + tamImagen - 1, NULL, FILE_BEGIN);
	CheckError(INVALID_SET_FILE_POINTER == Movido, "Error al ejecutar SetFilePointer)", 1);
	// escribir punto negro
	CheckError(FALSE == WriteFile(hArchivo, &cero, 3, &Movido, NULL) || Movido != 3,
		"Error al escribir pixel", 1);

	CloseHandle(hArchivo);



	printf("\nPulse una tecla para terminar\n");  //Solo para depurar. Quitar si no es necesario
	getchar();
	return 0;
}