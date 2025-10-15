#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <conio.h>
#include <locale.h>
#include <string.h>
#include <Windows.h>

#include "T04-Proj-Principal.h"
#include "pLibSS.h" //añadir $(AAComm)\inc en los directorios de inclusión del proyecto

int main(int argc, char *argv[])
{

	//char * path = "\"C:\\Program Files (x86)\\Google\\Chrome\\Application\\chrome.exe\""
	//	" www.informatica.us.es";
	setlocale(LC_ALL, "Spanish");
	//Eje4_2();
	//Demo1_2();


	Demo3_2018_MideRetraso("");
	Demo3_2018_MideRetraso(0);
	Sleep(500);
	double res = Demo3_2018_MideRetraso("Retraso en segs despues de ejecutar Sleep: ");
	printf("Valor de res (segs): %4.3f", res);


	//LanzaChrome();
	//system(path);
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
	// Comprobar resultado de la llamada...

	// Llamar MessageBox...

	// Comprobar resultado de la llamada...
}
static double Demo3_2018_MideRetraso(char * msg)
{
	double segs;
	static bool primeraLlamada = true;
	static LARGE_INTEGER frecuencia;
	static LARGE_INTEGER anterior;
	LARGE_INTEGER actual;

	if (primeraLlamada)
	{// si es la primera llamada devuelve 0.0
		// calcula la frecuencia con QueryPerformanceFrequency
		CheckError(FALSE == QueryPerformanceFrequency(&frecuencia), "No se pudo ejecutar QueryPerformanceFrequency", 1);
		// guarda el del contador actual con QueryPerformanceCounter	
		CheckError(FALSE == QueryPerformanceCounter(&anterior), "No se pudo ejecutar QueryPerformanceCounter", 2);
		primeraLlamada = false;
		segs = 0.0;
	}
	else
	{// las siguientes llamadas:
		// lee el valor del contador actual 
		CheckError(FALSE == QueryPerformanceCounter(&actual), "No se pudo ejecutar QueryPerformanceCounter", 3);
		LARGE_INTEGER cuenta;
		cuenta.QuadPart = actual.QuadPart - anterior.QuadPart; //y le resta el contador anterior
		// pasa a segundos dividiendo por la frecuencia
		segs = (double)cuenta.QuadPart / (double)frecuencia.QuadPart;
		anterior = actual;
	}
	// trata el mensaje de usuario.
	// PENDIENTE
	// devuelve el resultado
	// si el puntero es 0 (NULL) o apunta a "", no hago nada
	
	if (msg != NULL && (0!=strncmp(msg, "", 1))) //msg[0]!=0
	{ // procesar msg
		//1.00435 s
		char cad[1000];
		snprintf(cad, sizeof(cad), "%s %.3f s", msg, segs);
		OutputDebugString(cad);
	}
	return segs;


}


static void Eje4_2()
{
	//> Intento de usar GetIntegratedDisplaySize. 
	/*
	* Hay que configurar el proyecto para Windows 10: https://msdn.microsoft.com/es-es/library/mt186161.aspx
	* Solo funciona si se ha instalado el SDK de Windows 10 con VS2015. Ver https://social.msdn.microsoft.com/Forums/vstudio/en-US/2f3d5130-f3f9-4771-a6f2-c5883aea0414/installing-windows-10-sdk-into-visual-studio?forum=vssetup

	A fecha de hoy, 7/11/2017, la documentación  en MSDN es incorrecta:
	para que compile, la biblioteca de importación es OneCore.lib, no kernel32.lib.
	*/

	//double TamDisplay;

	//GetIntegratedDisplaySize(&TamDisplay);
	//printf("\nTamaño del display: %f\n", TamDisplay);
}

#include <process.h>
#include <errno.h>
static void LanzaChrome()
{
	//system("dir");
	//intptr_t res = _execl("dir", "dir", NULL);
	char * execPath = "\"C:\\Program Files (x86)\\Google\\Chrome\\Application\\chrome.exe\"";
	system(execPath);
}

// Usa caracteres genéricos 
#include <tchar.h>

//VOID ReportError(LPCTSTR userMessage, DWORD exitCode, BOOL printErrorMessage)
//
///* General-purpose function for reporting system errors.
//	Obtain the error number and convert it to the system error message.
//	Display this information and the user-specified message to the standard error device.
//	userMessage:		Message to be displayed to standard error device.
//	exitCode:		0 - Return.
//					> 0 - ExitProcess with this code.
//	printErrorMessage:	Display the last system error message if this flag is set. */
//{
//	DWORD eMsgLen, errNum = GetLastError();
//	LPTSTR lpvSysMsg;
//	_ftprintf(stderr, _T("%s\n"), userMessage);
//	if (printErrorMessage) {
//		eMsgLen = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
//			NULL, errNum, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
//			(LPTSTR)&lpvSysMsg, 0, NULL);
//		if (eMsgLen > 0)
//		{
//			_ftprintf(stderr, _T("%s\n"), lpvSysMsg);
//		}
//		else
//		{
//			_ftprintf(stderr, _T("Last Error Number; %d.\n"), errNum);
//		}
//
//		if (lpvSysMsg != NULL) LocalFree(lpvSysMsg); /* Explained in Chapter 5. */
//	}
//
//	if (exitCode > 0)
//		ExitProcess(exitCode);
//
//	return;
//}
