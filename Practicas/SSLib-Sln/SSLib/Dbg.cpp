#define _LIB_EXPORTS_

#define _LIB_EXPORTS_
#include "sslib\SSLib.h"
#include <cstdarg>


#define TAM_BUFFER_DbgPrint 1024
// Definiciones asociadas a la clase 
namespace ss {

	bool Dbg::_primerUsoClase = true; // Variable estática de clase

	Dbg::Dbg(bool salidaPorPantalla, bool salidaPorDepuracion)
	{
		_salidaDepuracion = salidaPorDepuracion;
		_salidaPantalla = salidaPorPantalla;
	}

	Dbg::~Dbg()
	{
	}


	bool Dbg::CheckError(bool condicionError, const char* mensajeUsuario, ...)
	{
		bool res = false;
		if (condicionError)
		{
			char* buffer = new char[TAM_BUFFER_DbgPrint];
			buffer[0] = '\0'; // inicializa a cadena vacía
			DWORD errNum = GetLastError();
			MessageBeep(MB_ICONERROR);  // Emite un pitido de error. Puede cambiar el último código de error
			SetLastError(errNum); // Restaura el código de error original
			const int tamMsg = 300, tamCadFormato = tamMsg + 200;
			CHAR memsajeErrorSistema[tamMsg];
			// Pasa código de error de Windows a mensaje y lo pone al principio de la cadena de formato
			if (errNum != 0) {
				DWORD eMsgLen = FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM,
					NULL, errNum, 0, memsajeErrorSistema, sizeof(memsajeErrorSistema), NULL);
				if (eMsgLen > 0)
				{
					snprintf(buffer, TAM_BUFFER_DbgPrint, "\n%s", memsajeErrorSistema);
				}
				SetLastError(ERROR_SUCCESS); // limpia el código de error
			}
			else
			{
				strncpy(buffer, "\n", TAM_BUFFER_DbgPrint);
			}
			auto ultimoCaracter = strlen(buffer);

			// N3: procesa lista variable de argumentos. Es muy simple porque lo único que debe hacer es
			//      pasar los argumentos a otra función con argumentos variables. 

			va_list args;
			va_start(args, mensajeUsuario);
			int ret = vsnprintf(&buffer[ultimoCaracter], TAM_BUFFER_DbgPrint - ultimoCaracter, mensajeUsuario, args);
			va_end(args);
			if (ret >= 0 && ret < TAM_BUFFER_DbgPrint)
			{
				if (_salidaDepuracion)
					OutputDebugStringA(buffer); // Imprime en la ventana de salida de depuración
				if (_salidaPantalla)
					printf("%s", buffer); // Imprime en la consola de salida
			}
			res = true;
			//delete[] buffer;
		}
		return res;
	}

	bool Dbg::CheckError(bool condicionError, int codigoSalida, const char* mensajeUsuario, ...)
	{
		bool res = false;
		if (condicionError)
		{
			char* buffer = new char[TAM_BUFFER_DbgPrint];
			buffer[0] = '\0'; // inicializa a cadena vacía
			DWORD errNum = GetLastError();
			MessageBeep(MB_ICONERROR);  // Emite un pitido de error. Puede cambiar el último código de error
			SetLastError(errNum); // Restaura el código de error original
			const int tamMsg = 300, tamCadFormato = tamMsg + 200;
			CHAR memsajeErrorSistema[tamMsg];
			// Pasa código de error de Windows a mensaje y lo pone al principio de la cadena de formato
			if (errNum != 0) {
				DWORD eMsgLen = FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM,
					NULL, errNum, 0, memsajeErrorSistema, sizeof(memsajeErrorSistema), NULL);
				if (eMsgLen > 0)
				{
					snprintf(buffer, TAM_BUFFER_DbgPrint, "\n%s", memsajeErrorSistema);
				}
				SetLastError(ERROR_SUCCESS); // limpia el código de error
			}
			else
			{
				strncpy(buffer, "\n", TAM_BUFFER_DbgPrint);
			}
			auto ultimoCaracter = strlen(buffer);

			// N3: procesa lista variable de argumentos. Es muy simple porque lo único que debe hacer es
			//      pasar los argumentos a otra función con argumentos variables. 

			va_list args;
			va_start(args, mensajeUsuario);
			int ret = vsnprintf(&buffer[ultimoCaracter], TAM_BUFFER_DbgPrint - ultimoCaracter, mensajeUsuario, args);
			va_end(args);
			if (ret >= 0 && ret < TAM_BUFFER_DbgPrint)
			{
				if (_salidaDepuracion)
					OutputDebugStringA(buffer); // Imprime en la ventana de salida de depuración
				if (_salidaPantalla)
					printf("%s", buffer); // Imprime en la consola de salida
			}
			res = true;
			delete[] buffer;
		}

		if (res && codigoSalida != 0)
		{
			printf("\nPulse la tecla ENTRAR para terminar con código de terminación %d.\n", codigoSalida);
			(void)getchar();  // printf y getchar=> necesitan #include <stdio.h>
			ExitProcess(codigoSalida);
		}
		return res;
	}

	int Dbg::DbgPrint(const char* formatString, ...)
	{
		static char buffer[TAM_BUFFER_DbgPrint] = "";

		va_list args;
		va_start(args, formatString);
		int ret = vsnprintf(buffer, TAM_BUFFER_DbgPrint, formatString, args);
		va_end(args);
		if (ret >= 0 && ret < TAM_BUFFER_DbgPrint)
		{
			BOOL debuggerPresent = FALSE;
			if (CheckRemoteDebuggerPresent(GetCurrentProcess(), &debuggerPresent) && debuggerPresent) {
				OutputDebugStringA(buffer); // Imprime en la ventana de salida de depuración
				if (buffer[ret - 1] != '\n')
					OutputDebugStringA("\n"); // OutputDebugStringA no da salida hasta que se le envia un caracter new line
				//Sleep(1000); // Espera para que termine de conectar depurador.
			}
		}
		return ret;
	}
} // Fin namespace