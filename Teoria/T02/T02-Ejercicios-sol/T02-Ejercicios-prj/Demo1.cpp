#include <conio.h>
#include <memory.h>
#include <string.h>
#include <stdio.h>


void rellena(char[], int, char);

int main()
{
	char cad[40];// = "cadena Ejemplo ASCII"
	printf("\nCadena original: %s\n", cad);
	rellena(cad, 50, 'A');
	// Error: rellena más datos de los debe. Lo correcto es: 
	//rellena(cad, sizeof(cad), 'A');

	printf("\nCadena resultado: %s\n", cad);
	printf("\nPor favor, pulse una tecla para terminar (y ver el error :)...");
	_getch();
	return 0;
}
void rellena(char s[], int tam, char car) {
	int c = 0;
	while (c < tam - 1) {
		s[c] = car;
		c++;
	}
	s[tam - 1] = 0;
}
