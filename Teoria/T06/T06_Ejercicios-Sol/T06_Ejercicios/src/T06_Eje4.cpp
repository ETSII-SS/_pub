#define _CRT_SECURE_NO_WARNINGS
#include "pLibSS.h"

int Contador = 0;
// Este valor se calcula correctamente: no da tiempo a cambios de hilo en mitad de los hilos
//#define NRO_INCREMENTOS (10*100) 
#define NRO_INCREMENTOS (10*1000)
#define NRO_HILOS (10)
HANDLE hThreads[NRO_HILOS];

HANDLE hEvento;
//Cada hilo la incrementa en 1 NRO_INCREMENTOS veces 
DWORD WINAPI Incrementa(LPVOID NoSeUsa)
{
	int i;
	for (i = 0; i<NRO_INCREMENTOS; i++)
	{
		// Espera a que el acceso al recurso esté libre + indica que este hilo tiene la propiedad del recurso
		CheckError(0 != WaitForSingleObject(hEvento, INFINITE), "Error en el bloqueo", 1);
		Contador++;
		// Indica que el recurso está libre
		CheckError(FALSE == SetEvent(hEvento), "No se pudo desbloquear el objeto", 1);
	}
	return 0; // no se usa el valor devuelto
}
int main(int argc, char *argv[])
{
	int i;
	setlocale(LC_ALL, "Spanish");	// Indica que debe usar español como idioma por defecto
	// Inicializa objeto
	hEvento = CreateEvent(NULL, FALSE, TRUE, NULL); // Reset automatico, inicialmente abierto
	CheckError(hEvento == NULL, "No se pudo crear el objeto de sincronización", 1);
	//Crear N hilos que incrementen una variable global
	for (i = 0; i< NRO_HILOS; i++)
	{
		hThreads[i] = CreateThread(NULL, 0, Incrementa, 0, 0, NULL);
		CheckError(NULL == hThreads[i], "ERROR al crear hilo", 1);
	}
	// Espera a que terminen los NRO_HILOS hilos
	// No vale para más de 64 handles WaitForMultipleObjects(NRO_HILOS, hThreads, TRUE, INFINITE);
	for (i = 0; i< NRO_HILOS; i++)
	{
		CheckError(0 != WaitForSingleObject(hThreads[i], INFINITE), "ERROR al bloquear main", 1);
	}
	printf("\n El valor del contador es %d", Contador);
	printf("\n El valor esperado es     %d", NRO_HILOS*NRO_INCREMENTOS);
	printf("\n La diferencia en los valores es %d  (protegido con mutex)", NRO_HILOS*NRO_INCREMENTOS - Contador);

	printf("\nPulse tecla RETORNO para terminar\n");
	getchar();
	return 0;
}

