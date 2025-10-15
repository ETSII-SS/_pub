#include <stdio.h>
#include <locale.h>
#include <conio.h>
#include <Windows.h>

// Este programa falla en procesadores a más de 1,073,741,823.5 hz.
int main(int argc, char *argv[], char * envp[])
{
	int antes, ahora;
	setlocale(LC_ALL, "Spanish");
	// Leo el valor del contador RD TSC y lo guardo
	antes = __rdtsc();	//--> Aquí hay warning
	// Espero 2 segundos (fuerza fallo en ordenadores a 1,... Ghz o más)
	Sleep(2000);
	// Leo el valor del contador ahora
	ahora = __rdtsc();	//--> Aquí hay warning
	// Resto el valor e imprimo el resultado. Es la velocidad en Hz.	
	printf("La frecuencia en Hz es: %d", (ahora-antes)/2); 
	printf("\nPor favor, pulse una tecla para terminar ...");
	_getch();
	return 0;
}
