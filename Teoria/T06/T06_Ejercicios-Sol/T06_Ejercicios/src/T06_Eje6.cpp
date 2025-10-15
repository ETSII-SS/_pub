#define _CRT_SECURE_NO_WARNINGS
#include "pLibSS.h"

// Cuidado, no podemos poner más de 64 hilos (el límite de la matriz de handle de WaitForMultipleObject)
#define NRO_HILOS 64
//#define NRO_OP 4
// Cada hilo hace NRO_OP operaciones
#define NRO_OP (100)
int Contador = 0;

// Simula una operación que tarda 100 ms.
#define OPERACION Sleep(100)
//#define OPERACION Contador++;

#pragma region Sin bloqueo
DWORD WINAPI OperacionesSinSincronizacion(LPVOID pANroOperaciones) {
	int i;
	for (i = 0; i<NRO_OP; i++)	{
		OPERACION;
		(*((PDWORD)pANroOperaciones))--;	// Decrementa número de operaciones
	}
	return 0;
}

double LanzaHilosSinSincronizacion(char * mensaje) {
	DWORD  NroOperaciones = 0, i;
	PHANDLE phT= (PHANDLE)malloc(sizeof(*phT)*NRO_HILOS); // Reserva un bloque de memoria para la matriz de punteros

	MideRetraso(NULL);
	for (i = 0; i<NRO_HILOS; i++){
		NroOperaciones += NRO_OP;	// Cada hilo hace NRO_OP operaciones
		// Crea el thread. Se puede usar &NroOperaciones como parámetro porque la variable
		// permanece durante la vida de todos los threads
		phT[i] = CreateThread(NULL, 0, OperacionesSinSincronizacion, &NroOperaciones, 0, NULL);
		CheckError(phT[i] == NULL, "Error al crear thread 1", -1);
	}
	// Espera a que los hilos terminen las operaciones
	// WaitForMultipleObjects sincroniza con hasta 64 handles
	CheckError(0 != WaitForMultipleObjects(NRO_HILOS, phT, TRUE, INFINITE), "Error al sincronizar el fin de los hilos", 1);
	printf("\n%s El valor de NroOperaciones es %d (debe ser 0)\n.", mensaje, NroOperaciones);
	return MideRetraso(mensaje);
}
#pragma endregion  

#pragma region   Interlock
DWORD WINAPI OperacionesInterlock(LPVOID pANroOperaciones){
	int i;
	for (i = 0; i<NRO_OP; i++){
		OPERACION;
		InterlockedDecrement((unsigned volatile *)pANroOperaciones);	// Decrementa número de operaciones
	}
	return 0;
}

double LanzaThreadsInterlock(char * mensaje) {
	DWORD  NroOperaciones = 0;
	int i;
	PHANDLE phT = (PHANDLE)malloc(sizeof(*phT)*NRO_HILOS); // Reserva un bloque de memoria para la matriz de punteros

	MideRetraso(NULL);
	for (i = 0; i<NRO_HILOS; i++) {
		InterlockedExchangeAdd(&NroOperaciones, NRO_OP);	// Lanzamos NRO_OP operaciones  *** ESTE ES EL CAMBIO
		phT[i] = CreateThread(NULL, 0, OperacionesInterlock, &NroOperaciones, 0, NULL);
		CheckError(phT[i] == NULL, "Error al crear thread 1", -1);
	}
	// Espera a que los hilos terminen las operaciones. ...MultipleObjects sincroniza con hasta 64 handles
	CheckError(0 != WaitForMultipleObjects(NRO_HILOS, phT, TRUE, INFINITE), "Error al sincronizar el fin de los hilos", 1);
	printf("\n%s El valor de NroOperaciones es %d (debe ser 0)\n.", mensaje, NroOperaciones);
	return MideRetraso(mensaje);
}
#pragma endregion  

#pragma region CriticalSection

static CRITICAL_SECTION cs;
DWORD WINAPI OperacionesCS(LPVOID pANroOperaciones) {
	int i;
	for (i = 0; i<NRO_OP; i++)
	{
		OPERACION;
		EnterCriticalSection(&cs);
		(*((PDWORD)pANroOperaciones))--;	// Decrementa número de operaciones
		LeaveCriticalSection(&cs);
	}
	return 0;
}
double LanzaThreadsCS(char * mensaje) {
	volatile DWORD  NroOperaciones = 0, i;
	PHANDLE phT = (PHANDLE)malloc(sizeof(*phT)*NRO_HILOS); // Reserva un bloque de memoria para la matriz de punteros

	MideRetraso(NULL);
	InitializeCriticalSection(&cs);
	for (i = 0; i<NRO_HILOS; i++) {
		EnterCriticalSection(&cs);
		NroOperaciones += NRO_OP;	// Lanzamos NRO_OP operaciones
		LeaveCriticalSection(&cs);
		phT[i] = CreateThread(NULL, 0, OperacionesCS, (LPVOID) &NroOperaciones, 0, NULL);
		CheckError(phT[i] == NULL, "Error al crear thread 1", -1);
	}
	// Espera a que los hilos terminen las operaciones
	// WaitForMultipleObjects sincroniza con hasta 64 handles
	CheckError(0 != WaitForMultipleObjects(NRO_HILOS, phT, TRUE, INFINITE), "Error al sincronizar el fin de los hilos", 1);
	printf("\n%s El valor de NroOperaciones es %d (debe ser 0)\n.", mensaje, NroOperaciones);
	return MideRetraso(mensaje);
}
#pragma endregion  

#pragma region Mútex
HANDLE hmutex;
//OJO, sin terminar
DWORD WINAPI OperacionesMutex(LPVOID pANroOperaciones) {
	int i;
	for (i = 0; i<NRO_OP; i++)	{
		OPERACION;
		CheckError(WAIT_OBJECT_0 != WaitForSingleObject(hmutex, INFINITE), "No se pudo bloquear por el mútex", 1);
		(*((PDWORD)pANroOperaciones))--;	// Decrementa número de operaciones
		CheckError(FALSE == ReleaseMutex(hmutex), "No se pudo liberar eñ mútex", 2);
	}
	return 0;
}
double LanzaThreadsMutex(char * mensaje) {
	DWORD  NroOperaciones = 0, i;
	PHANDLE phT = (PHANDLE)malloc(sizeof(*phT)*NRO_HILOS); // Reserva un bloque de memoria para la matriz de punteros

	MideRetraso(NULL);
	hmutex = CreateMutex(NULL, false, NULL);
	for (i = 0; i<NRO_HILOS; i++) {
		NroOperaciones += NRO_OP;	// Lanzamos NRO_OP operaciones
		phT[i] = CreateThread(NULL, 0, OperacionesMutex, &NroOperaciones, 0, NULL);
		CheckError(phT[i] == NULL, "Error al crear thread en LanzaThreadsMutex", -1);
	}
	CheckError(0 != WaitForMultipleObjects(NRO_HILOS, phT, TRUE, INFINITE), "Error al sincronizar el fin de los hilos", 1);
	printf("\n%s El valor de NroOperaciones es %d (debe ser 0)\n.", mensaje, NroOperaciones);
	return MideRetraso(mensaje);
}
#pragma endregion 

int  main(void)
{
	HANDLE hm;
	double UltimoRetraso, Retraso;
	setlocale(LC_ALL, "Spanish");	

	// Ejercicio 7: 
	//hm = CreateMutex(NULL, FALSE, "_mutexEje7");
	//DWORD res;
	//do {
	//	res = WaitForSingleObject(hm, 3 * 1000);
	//	if (res == WAIT_TIMEOUT) {
	//		printf("Esperando a que termine otra instancia del programa... \n");
	//		continue;
	//	}
	//	if (res == WAIT_ABANDONED)
	//		break;	// Quien tenía el mutex ha terminado su hilo.
	//	CheckError(res != WAIT_OBJECT_0, "Error iniciando el programa", 1);
	//} while (res != WAIT_OBJECT_0);



	Retraso = LanzaHilosSinSincronizacion("SIN sincronización ---->>>>>");
	printf("\nTiempo sin sincronizacion; %f\n", Retraso);

	Retraso= LanzaThreadsInterlock("Con sincronización Interlock functions -->>>>>");
	printf("\nTiempo con interlocked functions: %f\n", Retraso);

	Retraso= LanzaThreadsCS("Con sincronización sección crítica -->>>>>");
	printf("\nTiempo con LanzaThreadsCS: %f\n", Retraso);

	Retraso= LanzaThreadsMutex("Con sincronización por mútex-->>>>>");
	printf("\nTiempo con LanzaThreadsMutex: %f\n", Retraso);

	printf("\n\nPulsa una tecla para continuar..."); getchar();
}


