// Test clásico de un proyecto de biblioteca. 
// Consiste símplemente en hacer un proyecto de programa ejecutable que pruebe las funciones/métodos de la biblioteca.
//

#include <stdio.h>
#include <locale.h>
#include "sslib/SSLib.h"

using namespace ss;
static int BuscaTextoEnArchivos(const char* dirBusqueda, const char* textoABuscar);

static Dbg dbg(true); // Salida por pantalla y por depuración
int main()
{
	setlocale(LC_ALL, "Spanish");
	
	FileSys fs;

	dbg.DbgPrint("Versión de la biblioteca: %s\n", dbg.Vesion); // Muestra la versión de la biblioteca por la ventana de depuración

	int encontrados;
	const char *pDir= "..";
	BuscaTextoEnArchivos(pDir, "BuscaTextoEnArchivos");

	char ** plistaArchivos= fs.ArchivosEnDirectorio(&encontrados, "."); // Muestra los archivos de la carpeta System32 por la consola de salida
	dbg.CheckError(plistaArchivos == nullptr, "No se han encontrado archivos en la carpeta %s\n", pDir); // Muestra un mensaje de error si no se han encontrado archivos

	//CHAR dirTest[MAX_PATH];
	//GetEnvironmentVariableA("AAComm", dirTest, MAX_PATH);
	// Prueba rutas largas
	//CHAR dirTest[MAX_PATH] = "c:\\Windows"; // contiene rutas largas
	//CHAR dirTest[MAX_PATH] = "c:\\"; // OJO: este solo sirve para comprobar que no da fallos por rutas largas ni reserva de memoria. 
	// El nro. exacto de archivos no se puede comprobar porque el nro de archivos en el disco varía constantemente debido a los archivos temporales que crean 
	// y destruyen constantemente las distintas aplicaciones. 
	const char* aBuscar = "BuscaDatoEnArchivo";
	size_t bytesAcumulados = 0;
	int ocurrencias= fs.BuscaDatoEnArchivo(aBuscar, strlen(aBuscar), ".\\TestLibClasico.cpp", &bytesAcumulados, 1);

	CHAR dirTest[MAX_PATH] = "c:\\Windows\\System32"; // contiene rutas con directorios protegidos
	plistaArchivos = fs.ArchivosEnDirectorio(&encontrados, dirTest, false);
	dbg.CheckError(plistaArchivos == nullptr, "No se han encontrado archivos en la carpeta %s\n", pDir); // Muestra un mensaje de error si no se han encontrado archivos
	printf("Encontrados %d archivos en %s\n\tEn %f segundos.", encontrados, dirTest, fs.TiempoUltimoMetodo());

	dbg.CronoInicio(); // Inicia med
	Sleep(1500); // Espera 1,5 segundo
	auto segs = dbg.CronoLee(); // lee tiempo transcurrido
	dbg.DbgPrint("Tiempo transcurrido: %f segundos\n", segs); // Muestra el tiempo transcurrido por la ventana de depuración
	dbg.CheckError(false, "Prueba error desde C++ %d\n", 10);  // No debe mostrar nada
	dbg.CheckError(true, "Prueba error desde C++ y continua %d, %d, %d\n", 20, 30, 40); // Debe mostrar un mensaje de error

	HANDLE hfile = CreateFileA("", 0, 0, 0, 0, 0, 0); // Crea un fichero con nombre vacío
	dbg.CheckError(INVALID_HANDLE_VALUE==hfile, 1, "Prueba error forzado en CreateFile y termina %d, %d, %d\n", 50, 60, 70); // Debe mostrar un mensaje de error y terminar el programa

	printf("\nPor favor, pulse la tecla ENTRAR para terminar ...\n");
	(void)getchar();  // printf y getchar=> necesitan #include <stdio.h>
	return 0;
}


static size_t CalculaBytesEnArchivos(char* listaArchivos[], int nroArchivos) {
	size_t total = 0;
	int i = 0;
	WIN32_FILE_ATTRIBUTE_DATA info;
	for (i = 0; i < nroArchivos; i++) {
		if (dbg.CheckError(FALSE ==
			GetFileAttributesExA(listaArchivos[i], GetFileExInfoStandard, &info),
			"Error en GetFileAttributesExA para el archivo %s\n", listaArchivos[i]))
			continue;  // Salta a la siguiente iteración del bucle si hay error
		int64_t tam = ((int64_t)info.nFileSizeHigh >> 32) + info.nFileSizeLow;
		total += tam;
	}
	return total;
}
/// <summary>
/// Muestra el numero de archivos encontrados en el directorio, mostrado en nombre del directorio en forma canónica
///	</summary>
/// <param name="encontrados"></param>
/// <param name="pDir"></param>
static BOOL MuestraNroArchivosEncontradosEnDirectorio(int encontrados, char* pDir) {
	CHAR nombreDir[MAX_PATH];
	BOOL res = FALSE;
	res = GetFullPathNameA(pDir, MAX_PATH, nombreDir, nullptr);
	if (!dbg.CheckError(res == 0, "Error en GetFullPathNameA para el directorio %s\n", pDir)) {
		res = TRUE;
		printf("Se han encontrado %d archivos en la carpeta %s\n", encontrados, nombreDir);
	}
	return res;
}
static int BuscaTextoEnArchivos(const char* dirBusqueda, const char* textoABuscar) {
	Dbg dbg;
	FileSys fs;
	int ocurrencias, encontrados = 0, nroArchivos, idxMarca = 0;
	char** listaArchivos = fs.ArchivosEnDirectorio(&nroArchivos, dirBusqueda, false);
	if (dbg.CheckError(listaArchivos == nullptr, "No se han encontrado archivos en la carpeta %s\n", dirBusqueda)) {
		return -1;
	}
	char marcasAvance[] = "-\\|/"; // Para animación simple
	size_t bytesTotal, bytesAcumulados = 0;
	bytesTotal = CalculaBytesEnArchivos(listaArchivos, nroArchivos);
	if (dbg.CheckError(bytesTotal == 0, "No se han encontrado archivos en la carpeta %s\n", dirBusqueda)) {
		return -1;
	}
	MuestraNroArchivosEncontradosEnDirectorio(nroArchivos, (char*)dirBusqueda);
	printf("\nBuscando texto \"%s\" en %lld bytes\n", textoABuscar, bytesTotal);

	dbg.CronoInicio();
	for (int i = 0; i < nroArchivos; i++) {
		// Busca el texto en el archivo usando un método de FileSys
		ocurrencias = fs.BuscaDatoEnArchivo(textoABuscar, strlen(textoABuscar),
			listaArchivos[i], &bytesAcumulados, 10000);
		if (ocurrencias > 0) {
			encontrados++;
			dbg.DbgPrint("#%d: %d ocurrencias en %s\n", encontrados, ocurrencias, listaArchivos[i]);
		}
		else if (ocurrencias < 0) {
			dbg.DbgPrint("Error %s\n", listaArchivos[i]);
		}
		// Muestra el avance del proceso
		if (i == nroArchivos - 1)
			bytesAcumulados = bytesTotal; // Para que muestre 100% al final, aunque no sea exacto
		printf("\r%c  %.02f %%   %d/%d   ", marcasAvance[idxMarca++ % (sizeof(marcasAvance) - 1)],
			(double)bytesAcumulados * 100 / bytesTotal, i + 1, nroArchivos);
	}
	double segs = dbg.CronoLee();
	printf("\nSe han encontrado %d archivos con el texto\n", encontrados, textoABuscar, dirBusqueda);
	printf("\t\tCalculado en: %f segs por %s\n", segs, __FUNCTION__);
	return encontrados;
}