#define _CRT_SECURE_NO_WARNINGS
#include "pLibSS.h"
#define NRO_HILOS (100)
typedef struct {
	HANDLE h; DWORD id; int nroHilo, totalHilos;                                                                        // HANDLE anterior, posterior;
} di_imprimeHilo_t, * pdi_imprimeHilo_t;

pdi_imprimeHilo_t threadInfo[NRO_HILOS];

// Tema 8. Demo 1: imprime el número de hilo en 
DWORD WINAPI ImprimeHilo(LPVOID parametro)
{
	pdi_imprimeHilo_t p = (pdi_imprimeHilo_t)parametro;

	Sleep(2000 - p->nroHilo * 10);  // Esto pone peor las cosas 
	//Necesitamos comprobar turno aquí->                                                                                                                           CheckError(0 != WaitForSingleObject(p->anterior, INFINITE), "Error en el bloqueo", 2);
	printf("Hola, soy el hilo %d\n", p->nroHilo);                                                                                                           //CheckError(FALSE == SetEvent(p->posterior), "Error en Activación", 3);

	return 0;  // No se usa
}
// Función de entrada del proyecto "Demo-P1"
int main(int argc, char* argv[]){
	setlocale(LC_ALL, "Spanish"); 
	// Hacer sistema de turnos aquí ->                                                                                                                                                            HANDLE ultimoCreado = CreateEventA(NULL, , , FALSE);HANDLE empiezaPor= ultimoCreado;
	//Crear N hilos 
	for (int i = 0; i < NRO_HILOS; i++)	{
		pdi_imprimeHilo_t p = new di_imprimeHilo_t;							                                                                            //p->anterior = ultimoCreado;p->posterior= CreateEventA(NULL, FALSE, FALSE, FALSE);
		p->nroHilo = i; p->totalHilos = NRO_HILOS;
		p->h = CreateThread(NULL, 0, ImprimeHilo, p, 0, NULL);
		CheckError(NULL == p->h, "ERROR al crear hilo", 1);
		threadInfo[i] = p;                                                                                                                         //ultimoCreado = p->posterior;
	}
	for (int i = 0; i < NRO_HILOS; i++)	{
		CheckError(0 != WaitForSingleObject(threadInfo[i]->h, INFINITE), "ERROR al bloquear main", 1);
	}

	printf("\nPulse tecla RETORNO para terminar\n");
	getchar();
	return 0;
}

