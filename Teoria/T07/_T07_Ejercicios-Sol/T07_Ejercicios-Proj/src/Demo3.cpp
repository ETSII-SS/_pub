#include <stdio.h>
#include <locale.h>
#include <Windows.h>
#include "sslib\SSLib.h"	// <- Sustituir por biblioteca privada...

// Demo 3: la tabla de nombres se define como 
char TablaNombres[][20]
// Demo 4
// Define e inicializa una tabla de punteros a caracter
//char * TablaNombres[] 
= { "AAAA_AAAA","ALBERTO","ANGEL","CARLOS","DANIEL","FATIMA","FERNANDO",
"GABRIEL","HASSANE","JAIME","JOSE","JULIO","MANUEL","MARTA",
"PABLO","RAFAEL","RAQUEL","RUBEN","SIMONE","ZZZZ_ZZZZ",
}
;


int main(int argc, char *argv[])
{
	char Resultado[200];
	int nroElementos;
	HANDLE hf;
	DWORD Escritos;
	char * NArchivo = "Tabla1.dat";


	nroElementos = sizeof(TablaNombres) / sizeof(TablaNombres[0]);

	setlocale(LC_ALL, "Spanish");

	hf = CreateFile(NArchivo, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	CheckError(INVALID_HANDLE_VALUE == hf, 1, "No se pudo abrir el archivo para escribir");

	// Alternativas para escribir la tabla en un archivo? 
	//1
	CheckError(FALSE == WriteFile(hf, TablaNombres, sizeof(TablaNombres), &Escritos, NULL)
		|| sizeof(TablaNombres) != Escritos, 1, "No se pudo escribir (1)");

	////2
	for (int i = 0; i < nroElementos; i++)	{
		CheckError(FALSE == WriteFile(hf, TablaNombres[i], sizeof(TablaNombres[i]), &Escritos, NULL)
			|| sizeof(TablaNombres[i]) != Escritos, 1, "No se pudo escribir (2)");
	}

	////3
	for (int i = 0; i < nroElementos; i++) 	{
		CheckError(FALSE == WriteFile(hf, TablaNombres[i], strlen(TablaNombres[i]) + 1, &Escritos, NULL)
			|| (strlen(TablaNombres[i]) + 1) != Escritos, 1, "No se pudo escribir (3)");
	}

	CloseHandle(hf);


	printf("\nPulse una tecla para terminar\n");
	getchar();
	return 0;
}






