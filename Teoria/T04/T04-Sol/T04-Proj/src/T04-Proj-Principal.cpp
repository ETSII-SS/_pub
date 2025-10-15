#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <conio.h>
#include <locale.h>
#include <string.h>
#include <Windows.h>
#include <windns.h>

#include "T04-Proj-Principal.h"

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "Spanish");

	//LanzaChrome();
	//Eje4_2();
	//Demo1_2();


	//printf("Valor de res (segs): %4.3f", res);
	//LanzaChrome();

//	system(path);
	printf("\nPulse una tecla para terminar\n");  //Solo para depurar. Quitar si no es necesario
	_getch();
	return 0;
}


//BOOL WINAPI MessageBeep(_In_ UINT uType);
//int WINAPI MessageBox(
//	_In_opt_ HWND    hWnd,
//	_In_opt_ LPCTSTR lpText,
//	_In_opt_ LPCTSTR lpCaption,
//	_In_     UINT    uType
//);
static void Demo1_2()
{
	// Llamar MessageBeep...
	if (FALSE == Beep(1000, 500)) {
		//printf...;
	    return;
	}

	// Comprobar resultado de la llamada...

	// Llamar MessageBox... ¿Por qué falla?
	MessageBox(NULL, "Este es el contenido del mensaje", "Titulo",
		IDTRYAGAIN);
	// Comprobar resultado de la llamada...
}


static void Eje4_2()
{
	//https://learn.microsoft.com/es-es/windows/win32/api/windns/nf-windns-dnsgetapplicationsettings
	// esta función solo se puede usar en Windows 11. Para que compile y ejecute, debe instalarse el SDK de windows 11
	// Solo ejecutará en un computador con Windows 11.

	//DWORD nroServidores;
	//DNS_CUSTOM_SERVER * pMatrizServidores;
	//DNS_APPLICATION_SETTINGS settingsDNS;
	//DWORD res = DnsGetApplicationSettings(
	//	&nroServidores,
	//	&pMatrizServidores,
	//	&settingsDNS);
}

#include <process.h>
#include <errno.h>
static void LanzaChrome()
{
	//system("dir");
	//intptr_t res = _execl("dir", "dir", NULL);
	// Esta ruta depende de dónde se tenga instalado Chrome
	char* execPath = "\"C:\\Program Files\\Google\\Chrome\\Application\\chrome.exe\"";

	bool usaCLibrary = true;
	if (usaCLibrary)
	{
		int res = system(execPath);
		if (0!=res)
		{
			printf("Fallo al ejecutar system (código de error: %d)\n", res);
		}
	}
	else
	{
		// Estructuras para lanzar un proceso
		STARTUPINFOA si;
		ZeroMemory(&si, sizeof(si));  // Valores por defecto
		// Estructura con información sobre el proceso que se va a lanzar
		PROCESS_INFORMATION pi;
		// valores por defecto
		si.cb = sizeof(si);
		ZeroMemory(&pi, sizeof(pi));

		// Lanza el proceso
		if (!CreateProcessA(NULL, (LPSTR)execPath, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
		{
			printf("Fallo en CreateProcess (código de error: %d)\n", GetLastError());
		}

		// Un ejemplo real del uso de CreateProcces puede verse
		// aquí: https://github.eii.us.es/IC3-SS/_pub/blob/main/Practicas/_TestSln/TestSlnPrj/Dbg.cpp#L162
	}


}

// Usa caracteres genéricos 
#include <tchar.h>

VOID ReportError(LPCTSTR userMessage, DWORD exitCode, BOOL printErrorMessage)

/* General-purpose function for reporting system errors.
	Obtain the error number and convert it to the system error message.
	Display this information and the user-specified message to the standard error device.
	userMessage:		Message to be displayed to standard error device.
	exitCode:		0 - Return.
					> 0 - ExitProcess with this code.
	printErrorMessage:	Display the last system error message if this flag is set. */
{
	DWORD eMsgLen, errNum = GetLastError();
	LPTSTR lpvSysMsg;
	_ftprintf(stderr, _T("%s\n"), userMessage);
	if (printErrorMessage) {
		eMsgLen = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			NULL, errNum, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR)&lpvSysMsg, 0, NULL);
		if (eMsgLen > 0)
		{
			_ftprintf(stderr, _T("%s\n"), lpvSysMsg);
		}
		else
		{
			_ftprintf(stderr, _T("Last Error Number; %d.\n"), errNum);
		}

		if (lpvSysMsg != NULL) LocalFree(lpvSysMsg); /* Explained in Chapter 5. */
	}

	if (exitCode > 0)
		ExitProcess(exitCode);

	return;
}
