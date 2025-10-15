#include "sslib/SSLib.h"
#include <conio.h>

// Demo 2. Fallos en un puntero pasado a un hilo

//DWORD WINAPI ThreadProc(LPVOID lpParameter)
DWORD WINAPI Beeper(PDWORD pTiempoEntreBips)
{
	DWORD Contador, Frecuencia = 500, Duracion = 100;
	DWORD Ciclo = 125; // ciclo de llamda a Sleep

	Contador = *pTiempoEntreBips;
	while (TRUE)
	{
		if (Contador >= *pTiempoEntreBips)
		{
			Beep(Frecuencia, Duracion);
			Contador = 0;
		}
		Sleep(Ciclo);
		Contador += Ciclo;
	}


	return 0;    // En este ejemplo, aquí nunca llega
}

DWORD WINAPI ControlBeeper(LPVOID pACualquierCosa)
{
	DWORD TiempoEntreBips = 1000;
	DWORD IdThread;
	HANDLE hThread;
	int Cmd = ' ';

	hThread = CreateThread(NULL, 0,
		(LPTHREAD_START_ROUTINE)Beeper, // Necesita casting
		&TiempoEntreBips,
		0, // También se suele usar CREATE_SUSPENDED
		&IdThread);
	CheckError(NULL == hThread, 1, "Error al crear hilo Beeper\n");

	while (TRUE) {
		system("CLS");    // Ejecuta el comando CLS (clear screen) en el shell
		printf("\nEn este ejemplo, el hilo de control dura 10 segundos y el programa termina.\nO eso espero...\n\n");

		printf("Ultimo comando: %c\nY ahora?\n\n", Cmd);
		printf("0.- Salir\n1.- Silenciar\n2.- Volver a sonar\n3.- Aumentar tiempo entre bips: %d\n4.- Disminuir tiempo entre bips\n\n",
			TiempoEntreBips);
		Cmd = _getch();
		switch (Cmd) {
		case '0':
			ExitProcess(0); // Termina el programa
			break;
		case '1':
			if (NULL != hThread)	// Solo entra si se ha creado.
			{
				CheckError(FALSE == TerminateThread(hThread, 0), 1, "Error al terminar thread 1\n");
				WaitForSingleObject(hThread, INFINITE);
				hThread = NULL;
			}
			break;
		case '2':
			if (NULL == hThread) {
				hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Beeper, &TiempoEntreBips, 0, &IdThread);
				CheckError(NULL == hThread, 1, "Error al crear thread 1\n");
			}
		break;
		case '3':
			if (TiempoEntreBips < 10 * 1000)    // No pasa de 10 segundos.
				TiempoEntreBips += 250;
			break;
		case '4':
			if (TiempoEntreBips > 250)    // No pasa de 0,25 segundos.
				TiempoEntreBips -= 250;
			break;
		default:
			break;
		}
	}
	return 0;

}
//
int  main(void)
{
	HANDLE hThreadControlBeeper;

	setlocale(LC_ALL, "Spanish");	// Indica que debe usar español como idioma por defecto

	hThreadControlBeeper = CreateThread(NULL, 0, ControlBeeper, NULL, 0, NULL);
	CheckError(NULL == hThreadControlBeeper, 1, "Error al crear hilo ControlBeeper\n");

	// Lo que quiero hacer: el usuario tiene 10 segundos para modificar periodo.
	//            A partir de entonces, se para el thread y muestra "Hilo terminado..."
	Sleep(10 * 1000); // Espera 10 segundos
	CheckError(FALSE == TerminateThread(hThreadControlBeeper, 0), 1, "Error al terminar ControlBeeper\n");
	WaitForSingleObject(hThreadControlBeeper, INFINITE);
	CloseHandle(hThreadControlBeeper);
	printf("\nHilo terminado.\nPulse tecla RETORNO para terminar\n");
	getchar();

	return 0;
}
