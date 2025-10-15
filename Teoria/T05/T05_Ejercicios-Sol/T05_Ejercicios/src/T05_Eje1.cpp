#include "sslib/SSLib.h"

#define PASO 0


#pragma region 0-Inicial

#if PASO==0

int main(int argc, char *argv[])
{
	setlocale(LC_ALL, "Spanish");  // Indica que debe usar español como idioma por defecto

	DWORD Ciclo = 1000; // Ciclo entre pitidos (en ms)
	DWORD Frecuencia = 500;
	DWORD Duracion = 100;

	while (TRUE) {
		Beep(Frecuencia, Duracion); // Toca un beep a la frecuenca y duración que se le pasa
		Sleep(Ciclo);
	}

	printf("\nPulse tecla RETORNO para terminar\n");
	getchar();
	return 0;
}
#endif
#pragma endregion


#pragma region 1-Agrupar codigo a paralelizar en una funcion

#if PASO==1

// Definición de un ThreadProc:
//DWORD WINAPI ThreadProc(LPVOID lpParameter);   
DWORD WINAPI TocaAsync(LPVOID noSeUsaEnEsteEjemplo)
{
	DWORD Ciclo = 1000;
	DWORD Frecuencia = 500;
	DWORD Duracion = 100;
	while (TRUE) {
		Beep(Frecuencia, Duracion);
		Sleep(Ciclo);
	}
}

int main(int argc, char *argv[])
{

	setlocale(LC_ALL, "Spanish");
	TocaAsync(0);
	printf("\nPulse tecla RETORNO para terminar\n");
	getchar();
	printf("\nTermino de pitar\n");

	return 0;
}

#endif // PASO==1

#pragma endregion


#pragma region 2-Ejecución asíncrona

#if PASO==2
//DWORD WINAPI ThreadProc(LPVOID lpParameter);   // Definición de un ThreadProc

DWORD WINAPI TocaAsync(LPVOID noSeUsaEnEsteEjemplo)
{
	DWORD Ciclo = 1000;
	DWORD Frecuencia = 500;
	DWORD Duracion = 100;
	while (TRUE) {
		Beep(Frecuencia, Duracion);
		Sleep(Ciclo);
	}
}

int main(int argc, char *argv[])
{

	setlocale(LC_ALL, "Spanish");
	HANDLE hT= CreateThread(NULL, 0, TocaAsync, NULL, 0, NULL);	// Crea
	CheckError(NULL == hT, 1, "ERROR: No se pudo crear el hilo\n"); // Comprueba Creación
	CloseHandle(hT); // Si no voy a usar el handle, puedo cerrarlo (no afecta al hilo)
	//// El hilo principal sigue con sus cosas...
	printf("\nPulse tecla RETORNO para terminar\n");
	getchar();
	printf("\nTermino de pitar\n");

	return 0;
}

#endif // PASO==2

#if PASO==3
//Hacer el paso 2 con la clase thread

DWORD WINAPI TocaAsync(LPVOID noSeUsaEnEsteEjemplo)
{
	DWORD Ciclo = 1000;
	DWORD Frecuencia = 500;
	DWORD Duracion = 100;
	while (TRUE) {
		Beep(Frecuencia, Duracion);
		Sleep(Ciclo);
	}
}

int main(int argc, char* argv[])
{

	setlocale(LC_ALL, "Spanish");
	HANDLE hT = CreateThread(NULL, 0, TocaAsync, NULL, 0, NULL);	// Crea
	CheckError(NULL == hT, 1, "ERROR: No se pudo crear el hilo\n"); // Comprueba Creación
	CloseHandle(hT); // Si no voy a usar el handle, puedo cerrarlo (no afecta al hilo)
	//// El hilo principal sigue con sus cosas...
	printf("\nPulse tecla RETORNO para terminar\n");
	getchar();
	printf("\nTermino de pitar\n");

	return 0;
}

#endif // PASO==2
#pragma endregion
