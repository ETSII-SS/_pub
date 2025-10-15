#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <locale.h>
#include <conio.h>
#define _CRT_SECURE_DEPRECATE_MEMORY
#include <memory.h>
#include <string.h>
#include "Eje1.h"


InfoAlumnos_t TablaInfoAlumnos[] =
{
	{
		"Nombre1", "Apellido1", "Apellido2", "SinRellenar",
		7.2, 36, 24, 8,
	},
	{
		"asdf", "qwer", "zxcv", "SinRellenar", 0.0, 0, 0, 0,
	},
	{
		"Otro", "Appal", "Dragw", "SinRellenar",
		0.0, 0, 0, 0
	},
	{
		"Otres", "Appal", "Dragw", "SinRellenar",
		0.0, 0, 0, 0
	}, 
};


int main(int argc, char *argv[])
{
	int nroElementos;


	nroElementos = sizeof(TablaInfoAlumnos) /
		sizeof(TablaInfoAlumnos[0]);

	setlocale(LC_ALL, "Spanish");

	CalculaUVUS(nroElementos, TablaInfoAlumnos);
	// Imprimir resultados
	for (int i = 0; i < nroElementos; i++)
	{
		printf("%s %s, %s  ###> %s \n",
			TablaInfoAlumnos[i].Apellido1,
			TablaInfoAlumnos[i].Apellido2,
			TablaInfoAlumnos[i].Nombre,
			TablaInfoAlumnos[i].UVUS
			);

	}



	printf("\nPulse una tecla para terminar\n");
	_getch();
	return 0;
}

void CalculaUVUS(int NroElementos,
	InfoAlumnos_t tabla[])
{

	for (int i = 0; i < NroElementos; i++)
	{
		int contadorUVUS = 0;
		memcpy(&tabla[i].UVUS[0], &tabla[i].Nombre[0], 3);
		memcpy(&tabla[i].UVUS[3], &tabla[i].Apellido1[0], 3);
		memcpy(&tabla[i].UVUS[6], &tabla[i].Apellido2[0], 3);
		tabla[i].UVUS[9] = 0;

		// Buscar los uvus básicos que sean iguales en 
		// la parte anterior de la tabla
		for (int j = i - 1; j >= 0; j--)
		{
			int tamUVUS;
			// Si uvus básico i y j son iguales, 
			//incremento un contador
			tamUVUS = strlen(tabla[i].UVUS);
			if (0 == strncmp(tabla[i].UVUS, tabla[j].UVUS, tamUVUS))
				contadorUVUS++;
		}
		// Si contadorUVUS es mayor que 0, le pego contadorUVUS al uvus
		if (contadorUVUS>0)
			_snprintf(tabla[i].UVUS, sizeof(tabla[i].UVUS),
			"%s%d", tabla[i].UVUS, contadorUVUS);
	}

}