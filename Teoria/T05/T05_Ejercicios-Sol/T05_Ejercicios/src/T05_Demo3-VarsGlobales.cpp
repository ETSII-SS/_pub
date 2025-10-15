#include "sslib/SSLib.h"

#include <conio.h>
// Demo 3. Compartiendo datos por variables globales/estáticas

// Variables globales compartidas
volatile DWORD Frecuencia;
volatile DWORD Duracion;
DWORD WINAPI Beeper(PDWORD pTiempoEntreBips)
{
	DWORD Contador;
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
	return 0;
}

int main(void)
{
	DWORD TiempoEntreBips;
	HANDLE hThread;
	DWORD Resultado;
	int Cmd = ' ';

	setlocale(LC_ALL, "Spanish");

	Frecuencia = 750; // Hz
	// Duración está fija en este ejemplo, pero también se podría usar para comunicanos entre threads.
	Duracion = 105; //ms    

	TiempoEntreBips = 1000;

	hThread = CreateThread(NULL, 0,	(LPTHREAD_START_ROUTINE)Beeper, &TiempoEntreBips,
		0, NULL);
	CheckError(NULL == hThread, 1, "Error al crear thread Beeper\n");
	while (TRUE)
	{

		system("CLS");    // Ejecuta el comando CLS (clear screen) en el shell
		printf("Ultimo comando: %c\nY ahora?\n\n", Cmd);
		printf("0.- Salir\n1.- Silenciar\n2.- Volver a sonar\n3.- Aumentar tiempo entre bips: %d\n4.- Disminuir tiempo entre bips\n5.- Aumentar frecuencia: %d\n6.- Disminuir frecuencia\n",
			TiempoEntreBips, Frecuencia, Duracion);
		Cmd = _getch(); 
		switch (Cmd)
		{
		case '0':
			ExitProcess(0);
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
				hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Beeper, &TiempoEntreBips, 0, NULL);
				CheckError(NULL == hThread, 1, "Error al crear thread 1\n");
			}
			break;
		case '3':
			if (TiempoEntreBips<10 * 1000)    // No pasa de 10 segundos.
				TiempoEntreBips += 250;
			break;
		case '4':
			if (TiempoEntreBips>250)    // No pasa de 0,25 segundos.
				TiempoEntreBips -= 250;
			break;
		case '5':    // Cambia la frecuencia
			if (Frecuencia<5000)    // check límites
				Frecuencia += 150;
			break;
		case '6':    // Cambia la frecuencia
			if (Frecuencia>200)    // check límites
				Frecuencia -= 150;
			break;
		default:
			break;
		}
	}
	return 0;
}
