#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <locale.h>
#include <Windows.h>

/// IMPORTANTE: Compilar este programa en x64

#define TAM_BLOQUE (1024*1024)

int main(int argc, char *argv[])
{
	int MBytes, i, NroTest=0;
	char * pPrueba;
	double Retraso;

	setlocale(LC_ALL, "Spanish"); // Previo


	MBytes = 2*512; // reserva en bloques de 512 MBytes
	for (int j = 0; j < 10; j++)
	{
 		pPrueba = (char *) malloc(MBytes*TAM_BLOQUE);
		if (NULL == pPrueba)
		{
			printf("\nERROR: No se pudo reservar memoria\n", 1);
			exit(1);
		}
		for (i = 0; i < MBytes*TAM_BLOQUE; i++)
			pPrueba[i] += 1;
		printf("\rTest #%d: Modificados %d MBytes",
			NroTest, MBytes);
		Sleep(100); // Asegura un tiempo mínimo entre llamadas
		NroTest++;
	}

	printf("\nPulse una tecla para terminar\n");  //Solo para depurar. Quitar si no es necesario
	getchar();
	return 0;
}