#include "sslib/SSLib.h"

#define PASO 2


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
#include <thread>

void TocaAsync()
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
	std::thread* philo = new std::thread(TocaAsync);
	CheckError(NULL == philo->native_handle(), 1, "ERROR: No se pudo crear el hilo\n"); // Comprueba Creación
	//// El hilo principal sigue con sus cosas...
	printf("\nPulse tecla RETORNO para terminar\n");
	getchar();
	printf("\nTermino de pitar\n");

	return 0;
}

#endif // PASO==2

#pragma endregion
