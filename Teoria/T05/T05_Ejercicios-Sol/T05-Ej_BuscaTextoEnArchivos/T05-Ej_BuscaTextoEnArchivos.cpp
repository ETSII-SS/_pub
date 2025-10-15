#include "T05-Ej_BuscaTextoEnArchivos.h"
static Dbg _dbg(true);
static FileSys _fs;

#define SINCRONO
//#define CONCURRENTE_CREATE_THREAD_WORKER
//#define CONCURRENTE_OBJECT_THREAD_WORKER
int main(int argc, char* argv[], char* envp[])
{
	setlocale(LC_ALL, "Spanish");
	
	_dbg.CheckError(argc < 3, 1, "Falta el nombre del directorio y/o el texto a buscar\n");
	BuscaTextoEnArchivos(argv[1], argv[2]);	
	printf("\nPor favor, pulse la tecla ENTRAR para terminar ...\n");
	(void)getchar();  
	return 0;
}

#ifdef SINCRONO
static int BuscaTextoEnArchivos(const char* dirBusqueda, const char* textoABuscar) {
	int ocurrencias, encontrados = 0, nroArchivos, idxMarca = 0;
	char** listaArchivos = _fs.ArchivosEnDirectorio(&nroArchivos, dirBusqueda, false);
	if (_dbg.CheckError(listaArchivos == nullptr, "No se han encontrado archivos en la carpeta %s\n", dirBusqueda)) {
		return -1;
	}
	char marcasAvance[] = "-\\|/"; // Para animación simple
	size_t bytesTotal, bytesAcumulados = 0;
	bytesTotal = CalculaBytesEnArchivos(listaArchivos, nroArchivos);
	if (_dbg.CheckError(bytesTotal == 0, "No se han encontrado archivos en la carpeta %s\n", dirBusqueda)) {
		return -1;
	}
	MuestraNroArchivosEncontradosEnDirectorio(nroArchivos, (char*)dirBusqueda);
	printf("\nBuscando texto \"%s\" en %lld bytes\n", textoABuscar, bytesTotal);

	_dbg.CronoInicio();
	for (int i = 0; i < nroArchivos; i++) {
		// Busca el texto en el archivo usando un método de FileSys
		ocurrencias = _fs.BuscaDatoEnArchivo(textoABuscar, strlen(textoABuscar),
			listaArchivos[i], &bytesAcumulados, 10000);
		if (ocurrencias > 0) {
			encontrados++;
			_dbg.DbgPrint("#%d: %d ocurrencias en %s\n", encontrados, ocurrencias, listaArchivos[i]);
		}
		else if (ocurrencias < 0) {
			_dbg.DbgPrint("Error %s\n", listaArchivos[i]);
		}
		// Muestra el avance del proceso
		if (i == nroArchivos - 1)
			bytesAcumulados = bytesTotal; // Para que muestre 100% al final, aunque no sea exacto
		printf("\r%c  %.02f %%   %d/%d   ", marcasAvance[idxMarca++ % (sizeof(marcasAvance) - 1)],
			(double)bytesAcumulados * 100 / bytesTotal, i + 1, nroArchivos);
	}
	double segs = _dbg.CronoLee();
	printf("\nSe han encontrado %d archivos con el texto\n", encontrados, textoABuscar, dirBusqueda);
	printf("\t\tCalculado en: %f segs por %s\n", segs, __FUNCTION__);
	return encontrados;
}
#endif

#ifdef CONCURRENTE_CREATE_THREAD_WORKER
typedef struct {
	char** listaArchivos;
	int nroArchivos;
	int * pEncontrados;
	char* textoABuscar;
	size_t bytesTotal;
	size_t *bytesAcumulados;
	int* pi;
	
} Di_t, * pDi_t;
static void workerFuncion(int nroArchivos, int *pi, char * textoABuscar, 
	char * listaArchivos[], size_t * pBytesAcumulados, int *pEncontrados) {
	int ocurrencias;
	for (*pi = 0; *pi < nroArchivos; (*pi)++) {
		// Busca el texto en el archivo usando un método de FileSys
		ocurrencias = _fs.BuscaDatoEnArchivo(textoABuscar, strlen(textoABuscar),
			listaArchivos[*pi], pBytesAcumulados);
		if (ocurrencias > 0) {
			*pEncontrados++;
			_dbg.DbgPrint("#%d: %d ocurrencias en %s\n", *pEncontrados, ocurrencias, listaArchivos[*pi]);
		}
		else if (ocurrencias < 0) {
			_dbg.DbgPrint("Error %s\n", listaArchivos[*pi]);
		}
	}
}
static DWORD WINAPI Worker(LPVOID lpParameter) {
	pDi_t pDi = (pDi_t)lpParameter;
	workerFuncion(pDi->nroArchivos, pDi->pi, pDi->textoABuscar, 
		pDi->listaArchivos, pDi->bytesAcumulados, pDi->pEncontrados);
	return 0;
}

static int BuscaTextoEnArchivos(const char* dirBusqueda, const char* textoABuscar) {
	int ocurrencias, encontrados = 0, nroArchivos, idxMarca = 0;
	char** listaArchivos = _fs.ArchivosEnDirectorio(&nroArchivos, dirBusqueda, false);
	if (_dbg.CheckError(listaArchivos == nullptr, "No se han encontrado archivos en la carpeta %s\n", dirBusqueda)) {
		return -1;
	}
	char marcasAvance[] = "-\\|/"; // Para animación simple
	size_t bytesTotal, bytesAcumulados = 0;
	bytesTotal = CalculaBytesEnArchivos(listaArchivos, nroArchivos);
	if (_dbg.CheckError(bytesTotal == 0, "No se han encontrado archivos en la carpeta %s\n", dirBusqueda)) {
		return -1;
	}
	MuestraNroArchivosEncontradosEnDirectorio(nroArchivos, (char*)dirBusqueda);
	printf("\nBuscando texto \"%s\" en %lld bytes\n", textoABuscar, bytesTotal);

	_dbg.CronoInicio();

	Di_t di; // Estructura para pasar los datos de instancia
	int i = 0;
	di.listaArchivos = listaArchivos;
	di.nroArchivos = nroArchivos;
	di.pEncontrados = &encontrados;
	di.textoABuscar = (char*)textoABuscar;
	di.bytesTotal = bytesTotal;
	di.bytesAcumulados = &bytesAcumulados;
	di.pi = &i;

	CreateThread(NULL, 0, Worker, &di, 0, NULL);
	while (i< nroArchivos){
		if (i == nroArchivos - 1)
			bytesAcumulados = bytesTotal; // Para que muestre 100% al final, aunque no sea exacto
		printf("\r%c  %.02f %%   %d/%d   ", marcasAvance[idxMarca++ % (sizeof(marcasAvance) - 1)],
			(double)bytesAcumulados * 100 / bytesTotal, i + 1, nroArchivos);
		Sleep(100);
	}
	double segs = _dbg.CronoLee();
	printf("\nSe han encontrado %d archivos con el texto\n", encontrados, textoABuscar, dirBusqueda);
	printf("\t\tCalculado en: %f segs por %s\n", segs, __FUNCTION__);
	return encontrados;
}
#endif

#ifdef CONCURRENTE_OBJECT_THREAD_WORKER
void workerFuncion(int nroArchivos, int *pi, char * textoABuscar, 
	char ** listaArchivos, size_t * pBytesAcumulados, int *pEncontrados) {
	int ocurrencias;
	for (*pi = 0; *pi < nroArchivos; (*pi)++) {
		// Busca el texto en el archivo usando un método de FileSys
		ocurrencias = _fs.BuscaDatoEnArchivo(textoABuscar, strlen(textoABuscar),
			listaArchivos[*pi], pBytesAcumulados);
		if (ocurrencias > 0) {
			*pEncontrados++;
			_dbg.DbgPrint("#%d: %d ocurrencias en %s\n", *pEncontrados, ocurrencias, listaArchivos[*pi]);
		}
		else if (ocurrencias < 0) {
			_dbg.DbgPrint("Error %s\n", listaArchivos[*pi]);
		}
	}
}
#include <thread>
static int BuscaTextoEnArchivos(const char* dirBusqueda, const char* textoABuscar) {
	int encontrados = 0, nroArchivos, idxMarca = 0;
	char** listaArchivos = _fs.ArchivosEnDirectorio(&nroArchivos, dirBusqueda, false);
	if (_dbg.CheckError(listaArchivos == nullptr, "No se han encontrado archivos en la carpeta %s\n", dirBusqueda)) {
		return -1;
	}
	char marcasAvance[] = "-\\|/"; // Para animación simple
	size_t bytesTotal, bytesAcumulados = 0;
	bytesTotal = CalculaBytesEnArchivos(listaArchivos, nroArchivos);
	if (_dbg.CheckError(bytesTotal == 0, "No se han encontrado archivos en la carpeta %s\n", dirBusqueda)) {
		return -1;
	}
	MuestraNroArchivosEncontradosEnDirectorio(nroArchivos, (char*)dirBusqueda);
	printf("\nBuscando texto \"%s\" en %lld bytes\n", textoABuscar, bytesTotal);

	_dbg.CronoInicio();
	int i = 0;

	thread hilo = thread(workerFuncion, nroArchivos, &i, (char *)textoABuscar, 
		listaArchivos, &bytesAcumulados, &encontrados);
	while (i< nroArchivos){
		if (i == nroArchivos - 1)
			bytesAcumulados = bytesTotal; // Para que muestre 100% al final, aunque no sea exacto
		printf("\r%c  %.02f %%   %d/%d   ", marcasAvance[idxMarca++ % (sizeof(marcasAvance) - 1)],
			(double)bytesAcumulados * 100 / bytesTotal, i + 1, nroArchivos);
		Sleep(100);
	}
	double segs = _dbg.CronoLee();
	printf("\nSe han encontrado %d archivos con el texto\n", encontrados, textoABuscar, dirBusqueda);
	printf("\t\tCalculado en: %f segs por %s\n", segs, __FUNCTION__);
	return encontrados;
}
#endif


static size_t CalculaBytesEnArchivos(char* listaArchivos[]
	, int nroArchivos) {
	size_t total = 0;
	WIN32_FILE_ATTRIBUTE_DATA info;
	for (int i = 0; i < nroArchivos; i++) {
		if (_dbg.CheckError(FALSE ==
			GetFileAttributesExA(listaArchivos[i], GetFileExInfoStandard, &info),
			"Error en GetFileAttributesExA para el archivo %s\n", listaArchivos[i]))
			continue;
		int64_t tam = ((int64_t)info.nFileSizeHigh << 32)
			+ info.nFileSizeLow;
		total += tam;
	}
	return total;
}


/// <summary>
/// Muestra el numero de archivos encontrados en el directorio, mostrado en nombre del directorio en forma canónica
///	</summary>
/// <param name="encontrados"></param>
/// <param name="pDir"></param>
BOOL MuestraNroArchivosEncontradosEnDirectorio(int encontrados, char* pDir) {
	CHAR nombreDir[MAX_PATH];
	BOOL res = FALSE;
	res = GetFullPathNameA(pDir, MAX_PATH, nombreDir, nullptr);
	if (!_dbg.CheckError(res == 0, "Error en GetFullPathNameA para el directorio %s\n", pDir)) {
		res = TRUE;
		printf("Se han encontrado %d archivos en la carpeta %s\n", encontrados, nombreDir);
	}
	return res;
}


