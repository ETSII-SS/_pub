#define _PRACTICA5_
#include "sslib/SSLib.h" 
#include "uvus-P5.h"
#include <thread>
#include <chrono>
#include <string>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <windows.h>

static Dbg dbg(true);
static void PrintCiclos(int veces = 20, int retrasoSeg = 1);

int main(int argc, char* argv[], char* envp[])
{
	setlocale(LC_ALL, "Spanish");

	// Comportamiento solicitado:
	// - Si no hay argumentos: ejecutar parte a)
	// - Si hay un argumento n: lanzar n instancias del mismo programa (sin argumentos)
	//   en nuevas ventanas, esperar 1s y luego ejecutar parte a)
	int nroCiclos = 10;
	if (argc == 1) {
		// Parte a)
		PrintCiclos(nroCiclos, 1);
	}
	else {
		// Parte b)
		int n = atoi(argv[1]);
		dbg.CheckError(n <= 0, 1,
			"El argumento debe ser un número entero mayor que 0");
		argv[0];

		for (int i = 0; i < n; i++) {
			STARTUPINFOA si;
			PROCESS_INFORMATION pi;
			ZeroMemory(&si, sizeof(si));
			si.cb = sizeof(si);
			ZeroMemory(&pi, sizeof(pi));
			// Cambio el título de la ventana del proceso hijo
			char titulo[100];
			snprintf(titulo, sizeof(titulo), "Proceso hijo %d de %d",
				i + 1, n);
			si.lpTitle = titulo; // Nuevo título de la ventana de consola
			
			BOOL ok = CreateProcessA(
				NULL,               // lpApplicationName
				argv[0],      // lpCommandLine
				NULL,               // lpProcessAttributes
				NULL,               // lpThreadAttributes
				FALSE,              // bInheritHandles
				CREATE_NEW_CONSOLE, // dwCreationFlags -> nueva ventana de consola
				NULL,               // lpEnvironment
				NULL,               // lpCurrentDirectory
				&si,                // lpStartupInfo
				&pi                 // lpProcessInformation
			);

			if (!ok) {
				dbg.CheckError(true, "No se pudo crear el proceso hijo");
			}
			else {
				// No necesitamos esperar a los hijos; cerramos los handles
				CloseHandle(pi.hProcess);
				CloseHandle(pi.hThread);
			}
		}

		Sleep(1000);
		PrintCiclos(nroCiclos, 1);
	}

	printf("\nPor favor, pulse la tecla ENTRAR para terminar ...\n");
	(void)getchar();  // printf y getchar=> necesitan #include <stdio.h>
	return 0;
}



static void PrintCiclos(int veces, int retrasoSeg) {

	// Sección crítica implementada con semáforo (sustituye al evento)
	// El semáforo se crea con contador máximo 1 e inicial 1 para permitir
	// que una sola instancia entre en la sección crítica a la vez.
	HANDLE hSem = CreateSemaphoreA(
		NULL,               // default security attributes
		3,                  // initial count = 1
		3,                  // maximum count = 1
		"MiSemaforo54634623546435"   // name of the semaphore
	);

	dbg.CheckError(hSem == NULL, 1, "No se pudo crear/abrir el semáforo");

	// Intento entrar en sección crítica sin esperar (timeout 0)
	DWORD res = WaitForSingleObject(hSem, 0);
	if (res == WAIT_TIMEOUT) {
		printf("No se pudo ejecutar el programa: ya hay una instancia en ejecución.");
		// cerramos handle antes de salir
		CloseHandle(hSem);
		return;
	}
	dbg.CheckError(res != WAIT_OBJECT_0, 2, "Error al esperar el objeto de sincronización");

	for (int i = 1; i <= veces; i++) {
		printf("Ciclo %d...\n", i);
		std::this_thread::sleep_for(std::chrono::seconds(retrasoSeg));
	}
	// Fin de sección crítica: liberamos el semáforo
	dbg.CheckError(FALSE == ReleaseSemaphore(hSem, 1, NULL), 3, "Error al liberar el semáforo");

	// Cerramos el handle local
	CloseHandle(hSem);
}


















