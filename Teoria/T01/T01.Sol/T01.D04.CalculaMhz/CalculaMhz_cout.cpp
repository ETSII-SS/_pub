// Cambiado para usar std.cout  (flujo de salida estandar de C++)
//     Este archivo no se compila en el proyecto
#include <iostream>
#include <conio.h>

#include <intrin.h>
#include <windows.h>
#include <stdint.h> // necesario para los enteros "standard" C/C++
// Muestro en pantalla la velocidad de reloj del procesador en hercios.
// 	Para ello uso una instrucción del procesador RDTSC (Read Time Stamp Counter, x86/x64)
using namespace std;
int main(void)
{
	int64_t antes, ahora;
	int64_t VelocidadCPU;


	// Leo el valor del contador RDTSC y lo guardo
	antes = __rdtsc();

	// Espero un segundo
	Sleep(1000);
	// Leo el valor del contador ahora
	ahora = __rdtsc();

	// Resto el valor e imprimo el resultado. Es la velocidad en Hz.
	VelocidadCPU = ahora - antes;
	cout << "La frecuencia en Hz es:" << VelocidadCPU ;
	
	cout << endl << "Por favor, pulse una tecla para terminar ...";
	_getch();	// lo más parecido es cin.get(), pero no funciona igual
	return 0;
}