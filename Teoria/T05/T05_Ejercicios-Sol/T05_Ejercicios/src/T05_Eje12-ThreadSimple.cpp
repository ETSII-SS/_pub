#include "sslib/SSLib.h"

// para _getch
#include <conio.h>
// Thread simple: Crear un thread, suspenderlo y activarlo.

DWORD WINAPI Beeper(LPVOID pACualquierCosa)
{
	PDWORD pTiempoEntreBips; 
	DWORD Ciclo = 125; // ciclo de llamda a Sleep
	DWORD Contador, Frecuencia = 500, Duracion = 100;


	// El puntero pACualquierCosa se puede usar sin problemas,
	// aunque habría que poner un casting al tipo de datos al que
	// apunta. Para evitar eso, se define un puntero con el tipo y
	// nombre que nos sea más cómodo, y hacemos esta asignación al principio.
	pTiempoEntreBips = (PDWORD)pACualquierCosa;

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
		Beeper, &TiempoEntreBips,
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
		case '1':  
			CheckError((DWORD)-1 == SuspendThread(hThread), 1, "Error al suspender hilo 1\n");
			break;
		case '2':
			CheckError((DWORD)-1 == ResumeThread(hThread), 1, "Error al reactivar hilo 1\n");
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






/*
Sustituir contenido de case '2' por esto

{	// ResumeThread y SuspendThread funcionan con contador de suspensión. El hilo ejecuta si el contador vale 0.
// Cada llamada a SuspendThread incrementa el contador. Para reanudar el hilo, hay que hacer tantas llamadas a ResumeThread
// como sean necesarias para que el contador de suspensiones sea 0
DWORD res = 1;
while (res != 0)
{
res = ResumeThread(hThread);	// devuelve valor de contador de suspensiones del hilo.
CheckError((DWORD)-1 == res, 1, "Error al reactivar thread 1\n");
}
}
*/