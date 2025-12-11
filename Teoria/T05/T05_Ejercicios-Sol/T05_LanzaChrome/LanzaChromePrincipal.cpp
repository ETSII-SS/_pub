#define _CRT_SECURE_NO_WARNINGS
#include "T05_LanzaChrome.h"
static Dbg _dbg(true, true);

// Función de entrada del proyecto 
int main(int argc, char* argv[])
{
    BOOL Resultado;
    PROCESS_INFORMATION ProcI;
    STARTUPINFOA ProcStartInfo;
    CHAR LineaComando[255];

    setlocale(LC_ALL, "Spanish");
    _snprintf(LineaComando, sizeof(LineaComando), "%s %s"
        , "\"C:\\Program Files\\Google\\Chrome\\Application\\chrome.exe\""
        , "https://www.informatica.us.es");

	// La ruta puede variar según la instalación. Asegurarse de que es correcta
	// También se puede usar otro navegador, cambiando la ruta y el nombre del ejecutable
    memset(&ProcStartInfo, 0, sizeof(ProcStartInfo));
    ProcStartInfo.cb = sizeof(STARTUPINFO);
    Resultado = CreateProcessA(NULL,
        LineaComando,
        NULL, NULL, FALSE, 0, NULL, NULL,
        &ProcStartInfo,
        &ProcI);
    CheckError(0 == Resultado, 1, "ERROR: No se pudo crear el proceso");
	Beep(1000, 500); // Sonido para indicar que se ha hemos llegado a este punto
	WaitForSingleObject(ProcI.hProcess, INFINITE); // Espera a que termine el proceso
    printf("\nPulse una tecla para terminar\n");  //Solo para depurar. Quitar si no es necesario
    getchar();
    return 0;
}

