#include <stdio.h>
#include <conio.h>
#include <locale.h>

int main(int argc, char *argv[], char *envp[])
{
	setlocale(LC_ALL, "Spanish");

	for (int i = 0; envp[i] != 0; i++)
	{
	}

	printf("\nPor favor, pulse una tecla para terminar ...");
	_getch();
	return 0;
}