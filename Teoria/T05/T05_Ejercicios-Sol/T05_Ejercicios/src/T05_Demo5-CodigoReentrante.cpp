#include "sslib/SSLib.h"

#include <conio.h>

typedef struct {
	DWORD Frecuencia;
	DWORD Duracion;
	DWORD TiempoEntreBips;

	DWORD IdThread;    // Comun en todos los threads
	HANDLE hThread;
} DIBeeper_t, *pDIBeeper_t;    // Estructura de instancia
//
// Entrada a thread que hace bip
DWORD WINAPI Beeper(pDIBeeper_t pDI)
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

#define NRO_BEEPERS 2
int  main(void)
{
	setlocale(LC_ALL, "Spanish");  // Indica que debe usar español como idioma por defecto

	DWORD i;
	pDIBeeper_t TablaBeepers[NRO_BEEPERS];

	for (i = 0; i < NRO_BEEPERS; i++)
	{
		pDIBeeper_t pDIBeeper = new DIBeeper_t;    // Esta memoria permanece hasta el fin del proceso.
		CheckError(pDIBeeper == NULL, 1, "Error al reservar memoria\n");
		pDIBeeper->Frecuencia = 750 + 250 * i; // Hz
		pDIBeeper->Duracion = 105; //ms
		pDIBeeper->TiempoEntreBips = 1000 + 250 * i;

		pDIBeeper->hThread = CreateThread(NULL, 0,
			(LPTHREAD_START_ROUTINE)Beeper, pDIBeeper,
			0, &pDIBeeper->IdThread);  // Tanto el HANDLE como el ID del thread se guarda en su
		// “bloque” de datos de instancia. Se pueden usar tanto desde el propio thread como
		// desde el thread que lo ha creado.
		CheckError(NULL == pDIBeeper->hThread, 1, "Error al crear thread Beeper\n");
		TablaBeepers[i] = pDIBeeper;
	}

	i = 0;
	while (TRUE)
	{
		DWORD Resultado;
		int Cmd = ' ';

		pDIBeeper_t pDIBeeper = TablaBeepers[i];
		system("CLS");    // Ejecuta el comando CLS (clear screen) en el shell
		printf("Último comando: %c\nY ahora?\n\n", Cmd);
		printf("0.- Salir\n1.- Silenciar\n2.- Volver a sonar\n3.- Aumentar tiempo entre bips: %d\n4.- Disminuir tiempo entre bips\n5.- Aumentar frecuencia: %d\n6.- Disminuir frecuencia\n7.- Cambia Beeper %d/%d\n",
			pDIBeeper->TiempoEntreBips, pDIBeeper->Frecuencia, i + 1, NRO_BEEPERS);
		Cmd = _getch();
		switch (Cmd)
		{
		case '0':
			ExitProcess(0);
			break;
		case '1':
			do {
				CheckError((DWORD)-1 == SuspendThread(pDIBeeper->hThread), 1, "Error al suspender hilo\n");
				break;
			} while (Resultado != 0);
		case '2':
			do {
				Resultado = ResumeThread(pDIBeeper->hThread);
				CheckError((DWORD)-1 == Resultado, 1, "Error al reactivar thread 1\n");
			} while (Resultado != 0);
			break;
		case '3':
			if (pDIBeeper->TiempoEntreBips < 10 * 1000)    // No pasa de 10 segundos.
				pDIBeeper->TiempoEntreBips += 250;
			break;
		case '4':
			if (pDIBeeper->TiempoEntreBips > 250)    // No pasa de 0,25 segundos.
				pDIBeeper->TiempoEntreBips -= 250;
			break;
		case '5':    // Cambia la frecuencia
			if (pDIBeeper->Frecuencia < 5000)    // check límites
				pDIBeeper->Frecuencia += 150;
			break;
		case '6':    // Cambia la frecuencia
			if (pDIBeeper->Frecuencia > 200)    // check límites
				pDIBeeper->Frecuencia -= 150;
			break;
		case '7':    // Cambiar beeper actual
			i++;
			if (NRO_BEEPERS == i)
				i = 0;
			break;
		default:
			break;
		}

	}
	return 0;
}
