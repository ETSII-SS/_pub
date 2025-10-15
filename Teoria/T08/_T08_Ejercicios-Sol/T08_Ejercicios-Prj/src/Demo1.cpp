#define _CRT_SECURE_NO_WARNINGS
#include "pLibSS.h"

int main(int argc, char *argv[])
{
	SYSTEM_INFO SysInfo;
	double TamMemoria;


	setlocale(LC_ALL, "Spanish"); // Previo

	// Espacio de direcciones disponible para una aplicación
	GetSystemInfo(&SysInfo);
#if _WIN64

	printf("\nLa memoria de usuario de esta aplicación está en 0x%llx - 0x%llx \n",
		SysInfo.lpMinimumApplicationAddress, SysInfo.lpMaximumApplicationAddress);
#elif _WIN32
	printf("\nLa memoria de usuario de esta aplicación está en 0x%x - 0x%x \n",
		SysInfo.lpMinimumApplicationAddress, SysInfo.lpMaximumApplicationAddress);
#else
#error "Debe estar definido  _WIN32 o _WIN64"
#endif
	TamMemoria = (LPBYTE)SysInfo.lpMaximumApplicationAddress - (LPBYTE)SysInfo.lpMinimumApplicationAddress;
	printf("Tamaño máximo (en GBytes): %f\n", (float)TamMemoria / (1024.0*1024.0*1024.0));


	printf("\nPulse una tecla para terminar\n");  //Solo para depurar. Quitar si no es necesario
	getchar();
	return 0;
}