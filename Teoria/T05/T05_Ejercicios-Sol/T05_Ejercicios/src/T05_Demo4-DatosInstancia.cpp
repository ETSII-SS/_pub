#include "sslib/SSLib.h"
#include <conio.h>

typedef struct{
	DWORD IdThread;    // Comun en todos los hilos
	HANDLE hThread;

	DWORD Frecuencia; 
	DWORD Duracion;
	DWORD TiempoEntreBips;
} DIBeeper_t, *pDIBeeper_t;    // Estructura de instancia
//
DWORD WINAPI Beeper(pDIBeeper_t pDI)    // pDI: Puntero a datos de instancia
{
	DWORD Contador;
	DWORD Ciclo = 125; // ciclo de llamda a Sleep
	Contador = pDI->TiempoEntreBips;
	while (TRUE) {
		if (Contador >= pDI->TiempoEntreBips) {
			Beep(pDI->Frecuencia, pDI->Duracion);
			Contador = 0;
		}
		Sleep(Ciclo);
		Contador += Ciclo;
	}
	return 0;
}
//
int main(void)
{
	setlocale(LC_ALL, "Spanish");	// Indica que debe usar español como idioma por defecto

	pDIBeeper_t pDIBeeper;

	pDIBeeper = new DIBeeper_t;    // Esta memoria permanece hasta el fin del proceso.
	CheckError(pDIBeeper == NULL, 1, "Error al reservar memoria\n");
	
	pDIBeeper->Frecuencia = 750; // Hz
	pDIBeeper->Duracion = 105; //ms
	pDIBeeper->TiempoEntreBips = 1000;

	pDIBeeper->hThread = CreateThread(NULL, 0,
		(LPTHREAD_START_ROUTINE)Beeper, pDIBeeper,
		0, &pDIBeeper->IdThread);
	CheckError(NULL == pDIBeeper->hThread, 1, "Error al crear hilo Beeper\n");

	// Muestra el menú para modificar frecuencia, etc...11
	while (TRUE)
	{
		DWORD Resultado;
		int Cmd = ' ';

		system("CLS");    // Ejecuta el comando CLS (clear screen) en el shell
		printf("Último comando: %c\nY ahora?\n\n", Cmd);
		printf("0.- Salir\n1.- Silenciar\n2.- Volver a sonar\n3.- Aumentar tiempo entre bips: %d\n4.- Disminuir tiempo entre bips\n5.- Aumentar frecuencia: %d\n6.- Disminuir frecuencia\n",
			pDIBeeper->TiempoEntreBips, pDIBeeper->Frecuencia, pDIBeeper->Duracion);
		Cmd = _getch();
		switch (Cmd)
		{
		case '0':
			ExitProcess(0);
			break;
		case '1':
			CheckError((DWORD)-1 == SuspendThread(pDIBeeper->hThread), 1, "Error al suspender hilo\n");
			break;
		case '2':
			do {
				Resultado = ResumeThread(pDIBeeper->hThread);
				CheckError((DWORD)-1 == Resultado, 1, "Error al reactivar hilo\n");
			} while (Resultado != 0);
			break;
		case '3':
			if (pDIBeeper->TiempoEntreBips<10 * 1000)    // No pasa de 10 segundos.
				pDIBeeper->TiempoEntreBips += 250;
			break;
		case '4':
			if (pDIBeeper->TiempoEntreBips>250)    // No pasa de 0,25 segundos.
				pDIBeeper->TiempoEntreBips -= 250;
			break;
		case '5':    // Cambia la frecuencia
			if (pDIBeeper->Frecuencia<5000)    // check límites
				pDIBeeper->Frecuencia += 150;
			break;
		case '6':    // Cambia la frecuencia
			if (pDIBeeper->Frecuencia>200)    // check límites
				pDIBeeper->Frecuencia -= 150;
			break;
		default:
			break;
		}

	}
	return 0;
}