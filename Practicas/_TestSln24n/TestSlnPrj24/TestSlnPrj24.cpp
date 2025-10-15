#include "TestSlnPrj24.h"
using namespace ss;

int main(int argc, char* argv[], char* envp[])
{
	setlocale(LC_ALL, "Spanish");

	Dbg dbg(true);
	// Comprueba si se han pasado los argumentos necesarios
	dbg.CheckError(argc < 2, ErrorFaltanArgumentos,
		"Faltan argumentos: debe indicar uvus, nroDePractica, y test (opcional).\n");
	dbg.CheckError(argc > 3, ErrorSobranArgumentos,
		"Faltan argumentos: debe indicar uvus, nroDePractica, y test (opcional).\n");

	VsSolutionTest solTest;

	bool modoTest = false;

	if (argc == 4)
	{
		modoTest = CompareStringA(LOCALE_SYSTEM_DEFAULT, LINGUISTIC_IGNORECASE,
			argv[3], sizeof(argv[3]), "test", sizeof("test")) == CSTR_EQUAL;
	}

	int nroPractica = (int)argv[2];

	/* Obtiene de las variables de entorno, la ruta de Visual Studio

	* Para probar esta parte, definir el valor de la variable una variable de entorno MSDOS en
	* las opciones del proyecto, añadiendo una línea como esta:

   __DirMSBUILD__=$(MSBuildToolsPath)
	*/

	char rutaVS[MAX_PATH];
	const char* nombreVar = "__DirMSBUILD__";
	DWORD encontrada = GetEnvironmentVariableA(nombreVar, rutaVS, sizeof(rutaVS));
	dbg.CheckError(encontrada == 0, 2, "Error: No se ha encontrado la variable de entorno %s.\n", nombreVar);

	int res = solTest.CompruebaSolucion(nroPractica, rutaVS, argv[1], modoTest);

	// CompruebaSolucion devolvió 0
	dbg.CheckError(res != 0, 1, "Solución incorrecta.\n");
	printf("Solución compilada correctamente.\n");

	printf("\nPor favor, pulse la tecla ENTRAR para terminar ...\n");
	(void)getchar();  // printf y getchar=> necesitan #include <stdio.h>
	return res;
}











//// Un experimento con estructuras y uniones
//struct {
//	int entero;
//	char car;
//} vEstructura = { 256, 1 }; // Define + inicializa la estructura
//union {
//	int entero;
//	char car;
//}  vUnion = { 256 }; // Define + inicializa la unión

//vUnion.car = 1;
//printf("Estructura: %d Tam: %d\n", 
//	vEstructura.entero, sizeof(vEstructura));
//printf("Union: %d Tam: %d\n", 
//	vUnion.entero, sizeof(vUnion));

//ss::Dbg dbg(true);
//dbg.CronoInicio();
//Sleep(1200); // Espera 1.2 segundos
//dbg.CronoFin();
//dbg.Print("Tiempo transcurrido: %f segundos\n", dbg.CronoSegs);
//
//path ruta(argv[0]);  // NO comentar esta línea