#define _CRT_SECURE_NO_WARNINGS
#include "pLibSS.h"
#include <thread>
#define NRO_HILOS (100)


// Tema 8. Demo 1: imprime el número de hilo en 
void ImprimeHilo(int nroHilo, int TotalHilos)
{
	Sleep(2000 - nroHilo * 10);  // Esto pone peor las cosas 
	printf("Hola, soy el hilo %d\n", nroHilo);
}
std::thread* pth[NRO_HILOS];
// Función de entrada del proyecto "Demo-P1"
int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "Spanish");
	//Crear N hilos 
	for (int i = 0; i < NRO_HILOS; i++)
	{
		pth[i] = new std::thread(ImprimeHilo, i, NRO_HILOS);
		CheckError(NULL == pth[i]->native_handle(), "ERROR al crear hilo", 1);
	}

	for (int i = 0; i < NRO_HILOS; i++)
	{
		CheckError(0 != WaitForSingleObject(pth[i]->native_handle(), INFINITE), "ERROR al bloquear main", 1);
		// Equivale a pth->join();, aunque sin control de errores
	}

	printf("\nPulse tecla RETORNO para terminar\n");
	getchar();
	return 0;
}

