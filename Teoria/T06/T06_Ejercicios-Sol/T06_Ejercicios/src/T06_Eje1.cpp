#define _CRT_SECURE_NO_WARNINGS
#include "pLibSS.h"

int Contador = 0;
// Este valor se calcula correctamente: no da tiempo a cambios de hilo en mitad de los hilos
//#define NRO_INCREMENTOS (10*100) 
#define NRO_INCREMENTOS (10*1000)
#define NRO_HILOS (1000)
HANDLE hThreads[NRO_HILOS];

//Cada hilo la incrementa en 1 NRO_INCREMENTOS veces 
DWORD WINAPI Incrementa(LPVOID NoSeUsa)
{
	int i;
	for (i = 0; i < NRO_INCREMENTOS; i++)
	{
		//printf("Hilo %d- Contador %d\n", (int)NoSeUsa, i);
		//printf("Hilo %d- ", (int)NoSeUsa);
		//printf("Contador %d\n", i);
		Contador++;
	}
	return 0; // no se usa el valor devuelto
}
int main(int argc, char *argv[])
{
	int i;
	setlocale(LC_ALL, "Spanish");	// Indica que debe usar español como idioma por defecto

	//Crear N hilos que incrementen una variable global
	for (i = 0; i < NRO_HILOS; i++)
	{
		hThreads[i] = CreateThread(NULL, 0, Incrementa, 0, 0, NULL);
		CheckError(NULL == hThreads[i], "ERROR al crear hilo", 1);
	}
	// Espera a que terminen los NRO_HILOS hilos
	for (i = 0; i < NRO_HILOS; i++)
	{
		CheckError(0 != WaitForSingleObject(hThreads[i], INFINITE), "ERROR al bloquear main", 1);

		//// Alternativa: espera usando WaitForMultipleObjects. El numero de handles que se
		//// puede usar son MAXIMUM_WAIT_OBJECTS (64)
		//int nroHilos = NRO_HILOS - i * MAXIMUM_WAIT_OBJECTS;
		//if (nroHilos < 0)
		//	break;
		//if (nroHilos > MAXIMUM_WAIT_OBJECTS)
		//	nroHilos = MAXIMUM_WAIT_OBJECTS;
		//CheckError(nroHilos <= WaitForMultipleObjects(nroHilos, hThreads, TRUE, INFINITE)
		//	, "ERROR al bloquear main", 1);
	}
	printf("\n El valor del contador es %d", Contador);
	printf("\n El valor esperado es     %d", NRO_HILOS*NRO_INCREMENTOS);
	printf("\n La diferencia en los valores es %d", NRO_HILOS*NRO_INCREMENTOS - Contador);

	printf("\nPulse tecla RETORNO para terminar\n");
	getchar();
	return 0;
}