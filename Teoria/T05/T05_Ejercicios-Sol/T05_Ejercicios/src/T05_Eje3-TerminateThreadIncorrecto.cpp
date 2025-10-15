#include "sslib/SSLib.h"

// para _getch
#include <conio.h>
// Thread simple: Crear un thread, suspenderlo y activarlo.

// Ejercicio 3
DWORD WINAPI Beeper(PDWORD pTiempoEntreBips)
{
	DWORD Ciclo = 125; // ciclo de llamda a Sleep
	DWORD Contador, Frecuencia = 500, Duracion = 100;

	Contador = *pTiempoEntreBips;
	while (TRUE)
	{
		if (Contador >= *pTiempoEntreBips)
		{
			Beep(Frecuencia, Duracion); // Toca un beep a la frecuenca y duración que se le pasa
			Contador = 0;
		}
		Sleep(Ciclo);
		Contador += Ciclo;
	}


	return 0;    // En este ejemplo, aquí nunca llega
}



int  main(void)
{
	double Retraso;
	DWORD TiempoEntreBips = 1000;
	DWORD IdThread;
	HANDLE hThread;
	int Cmd = ' ';
	setlocale(LC_ALL, "Spanish");  // Indica que debe usar español como idioma por defecto

	hThread = CreateThread(NULL, 0,
		(LPTHREAD_START_ROUTINE)Beeper, &TiempoEntreBips,
		0, // También se suele usar CREATE_SUSPENDED
		&IdThread);
	CheckError(NULL == hThread, 1, "Error al crear thread 1\n");


	while (TRUE){
		system("CLS");    // Ejecuta el comando CLS (clear screen) en el shell
		printf("Ultimo comando: %c\nY ahora?\n\n", Cmd);
		printf("0.- Salir\n1.- Silenciar\n2.- Volver a sonar\n3.- Aumentar tiempo entre bips: %d\n4.- Disminuir tiempo entre bips\n\n",
			TiempoEntreBips);
		Cmd = _getch();
		switch (Cmd) {
		case '0':
			ExitProcess(0); // Termina el programa
			break;
		case '1':  // CUIDADO: esta forma no es correcta ¿Por qué?
			CheckError(FALSE == TerminateThread(hThread, 0), 1, "Error al terminar thread 1\n");
			//AQUI ha terminado el hilo... o no.
			hThread = NULL;
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






		//case '1':  // Corregido. De todas formas, evitar uso de TerminateThread...
		//	if (NULL != hThread)	// Solo entra si se ha creado.
		//	{
		//		CheckError(FALSE == TerminateThread(hThread, 0), 1, "Error al terminar thread 1\n");
		//		WaitForSingleObject(hThread, INFINITE);
		//		hThread = NULL;
		//	}
		//	break;
