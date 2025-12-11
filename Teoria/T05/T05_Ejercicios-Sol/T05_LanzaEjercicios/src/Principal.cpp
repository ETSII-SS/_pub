#include "sslib/SSLib.h"
#include <conio.h>
// Este proyecto está configurado con la opción 
// multibyte character set (Propiedades de proyecto-->Avanzado-->Juego de caracteres).
// Hace que todas las funciones de la API de Windows que tienen versión ANSI (ASCII) y Unicode
// usen la versión ANSI.
int main(void)
{
	BOOL Resultado;
	PROCESS_INFORMATION ProcI;
	STARTUPINFO ProcStartInfo;
	CHAR LineaComando[255];

	setlocale(LC_ALL, "Spanish");
	
	_snprintf(LineaComando, sizeof(LineaComando),"%s %s", 
		"T05_Ejercicios.exe"
		, " ");
	printf("Lanzando aplicacion %s desde LanzaEjercicios.... \n\n", LineaComando);
	/* MUY IMPORTANTE: si no se da un path absoluto al nombre de archivo, el ejecutable
		debe estar en el mismo directorio que este programa o en alguno de los directorios
		de la variable de entorno PATH
	*/

	memset(&ProcStartInfo, 0, sizeof(ProcStartInfo));
	ProcStartInfo.cb = sizeof(ProcStartInfo);
	ProcStartInfo.lpTitle = "Proceso Ejercicio T05";
	// Crear el proceso en una nueva ventana de consola
	Resultado = CreateProcess(NULL,
		LineaComando,
		NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL,
		&ProcStartInfo,
		&ProcI);
	CheckError(FALSE == Resultado, 1, "ERROR: No se pudo crear el proceso\n");

	printf("\nPulse una tecla para terminar\n");  //Solo para depurar. Quitar si no es necesario
	getchar();
	return 0;
}