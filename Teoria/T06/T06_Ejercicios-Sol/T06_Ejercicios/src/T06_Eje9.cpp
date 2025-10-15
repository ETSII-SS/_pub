#define _CRT_SECURE_NO_WARNINGS
#include "pLibSS.h"

HANDLE BloqueaHastaQueSoloHaya1() {
	
	HANDLE hEmpieza;
	DWORD res = 0xff;

	hEmpieza = CreateMutex(NULL, false, "NombreObjetoSinc");
	CheckError(NULL == hEmpieza, "No se pudo crear objeto de sincronización", 1);

	while (res != WAIT_OBJECT_0) {
		res = WaitForSingleObject(hEmpieza, 1000);
		if (WAIT_TIMEOUT == res)
		{
			// LLeva mucho tiempo, hay que hacer algo...
			printf("\nEsperando a que termine el programa en ejecución...\n");
			continue; // Salta el resto del while
		}
		if (WAIT_ABANDONED == res) {
			// Se cerró el mutex
			printf("\nSe cerró el mutex porque el otro proceso terminó..\n");
			hEmpieza = NULL; // no es un handle válido
			break;	// sale del while
		}
		CheckError(res != WAIT_OBJECT_0, "No funciono la espera", -1);
		printf("\nEl otro proceso ejecutó ReleaseMutex...\n");
	}
	return hEmpieza;
}

int main(int argc, char *argv[]) {
	int i = 0;
	setlocale(LC_ALL, "Spanish");	// Indica que debe usar español como idioma por defecto

	HANDLE h= BloqueaHastaQueSoloHaya1();
	printf("\nEntra en proceso....\n");
	for (i = 0; i<20; i++) {
		printf("Ciclo %d\n", i);
		Sleep(1000);
	}

	//Desbloquea...
	ReleaseMutex(h);
	CloseHandle(h);
	printf("\nPulse tecla RETORNO para terminar\n");
	getchar();
	return 0;
}
