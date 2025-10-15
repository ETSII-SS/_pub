#define _CRT_SECURE_NO_WARNINGS
#include "pLibSS.h"

DWORD contador = 0;

int main(int argc, char *argv[])
{


	setlocale(LC_ALL, "Spanish"); // Previo

	bool escPulsado = false;
	printf("\nMantenga pulsada la tecla ESC para terminar\n\n");  //Solo para depurar. Quitar si no es necesario

	while (!escPulsado) {
		printf("Dir: 0x%x, contenido: %d \n", &contador, contador++);
		Sleep(1000);


		DWORD keyst = GetKeyState(VK_ESCAPE); 
		escPulsado = 0 != (0x80000000 & keyst); // Si se pulsa ESC, se pone a 1 el bit más significativo 
	}

	printf("\nPrograma terminado\n\n");  //Solo para depurar. Quitar si no es necesario
	return 0;
}