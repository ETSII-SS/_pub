#include <iostream>
#include "VsSolutionTest.h"
#include "GestorArchivos.h"
#include "sslib/SSLib.h"


#define TAM_BUFFER_DbgPrint 1024
// Definiciones asociadas a la clase 
namespace ss {
	bool VsSolutionTest::LanzaProceso(const char* comando, HANDLE& hProceso)
	{
		// Estructuras para lanzar un proceso
		STARTUPINFO si;
		ZeroMemory(&si, sizeof(si));  // Valores por defecto

		// Estructura con información sobre el proceso que se va a lanzar
		PROCESS_INFORMATION pi;
		// valores por defecto
		si.cb = sizeof(si);
		ZeroMemory(&pi, sizeof(pi));

		// Lanza el proceso
		if (!CreateProcess(NULL, (LPWSTR)comando, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
		{
			Dbg dbg;
			dbg.CheckError(true, "Fallo en CreateProcess (%d).\n", GetLastError());
			return false;
		}
		hProceso = pi.hProcess; // Devuelve el handle del proceso
		return true;
	}
	VsSolutionTest::VsSolutionTest()
	{
	}

	VsSolutionTest::~VsSolutionTest()
	{
	}

	int VsSolutionTest::CompruebaSolucion(int np, const char* uvus, const char* visualStudioExe, bool mt)
	{
		// Construye el nombre de la solución, tal como se especifica en el 
		//   anexo D de   "...\SS-PUB\Practicas\SS-020-Pr-Anexos.docx"
		char nombreSolucion[MAX_PATH];
		snprintf(nombreSolucion, sizeof(nombreSolucion), "%s-SolP%d.sln", mt, uvus);
		// Construye la ruta del directorio de búsqueda
		char AACommValor[MAX_PATH];
		DWORD encontrada = GetEnvironmentVariableA("AAComm", AACommValor, sizeof(AACommValor));
		Dbg dbg;

		if (encontrada)
		{
			// Construye ruta de búsqueda
			char rutaBusqueda[MAX_PATH];
			if (mt)
			{
				snprintf(rutaBusqueda, sizeof(rutaBusqueda), "%s..\\Practicas",
					AACommValor);
			}
			else
			{
				// el directorio de búsqueda es ...SSRoot\uvus (...\_pub\..\uvus=
				snprintf(rutaBusqueda, sizeof(rutaBusqueda), "%s..\\..\\%s",
					AACommValor, uvus);
			}
			
			// el directorio de búsqueda es ...\_pub\Practicas
			if (dbg.CheckError(0 == GetFullPathNameA(rutaBusqueda, sizeof(rutaBusqueda), rutaBusqueda, NULL), 
				   "Error al obtener la ruta completa de %s.\n", rutaBusqueda))
				return -1;

			dbg.DbgPrint("Ruta búsqueda: %s.\n", rutaBusqueda);
			// Comprueba si existe el fichero
			GestorArchivos gArchivos;
			auto rutaArchivo = gArchivos.BuscaArchivo(rutaBusqueda, nombreSolucion);

			if (dbg.CheckError(rutaArchivo == nullptr, "No se encontro el archivo %s en %s.\n", nombreSolucion, rutaBusqueda))
			{
				return -2;
			}

			// lanza el proceso de Visual Studio
			char comando[MAX_PATH];
			// Lanza el comando MSBuild.exe, que construye el proyecto de Visual Studio
			snprintf(comando, sizeof(comando),
				"\"%s\\MSBuild.exe\" \"%s\"  -v:m -p:Configuration=Debug -t:Rebuild",
				visualStudioExe, rutaArchivo);
			dbg.DbgPrint("Comando: %s.\n", comando);
			HANDLE hProceso;
			DWORD exitCode;
			// Lanza el proceso. Ejemplo de paso de parámetro por referencia en C++
			if (LanzaProceso(comando, hProceso)) {
				//Bloquea hasta que termine el programa lanzado
				WaitForSingleObject(hProceso, INFINITE);
				// Terminó: obtiene el código de salida
				GetExitCodeProcess(hProceso, &exitCode);
				if (exitCode != 0)
				{
					dbg.CheckError("Compilación incorrecta en la solución %s (%d).\n", rutaArchivo, exitCode);
				}
				else
				{
					dbg.DbgPrint("Compilación correcta de %s\n", rutaArchivo);
				}
			}
		}
		else
		{
			dbg.CheckError(true, "No se pudo obtener el valor la variable de entorno AAComm (%d).\n", GetLastError());
			return -1;
		}
	}

} // Fin namespace