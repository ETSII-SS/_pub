#include <stdio.h>
#include <Windows.h>

#define _LIB_EXPORTS_
#include "sslib\SSLib.h"
#define TAM_CADS_DBG 1000

namespace ss {  // Namespace: agrupación de clases, funciones y variables globales

	Crono::Crono()	// constructor
	{
		// Paso 1: Obtiene la frecuencia del timer del PC en tics/segundo usando QueryPerformanceFrequency
		//   Esta frecuencia depende del hardware del PC y no cambia durante la ejecución del programa
		if (_ticsPorSegundoDelTimerDelPC < 0) {
			// No se ha calculado la frecuencia aún. Se calcula ahora
			LARGE_INTEGER frequencia;
			if (QueryPerformanceFrequency(&frequencia)) {
				_ticsPorSegundoDelTimerDelPC = frequencia.QuadPart;
			}
			else {
				OutputDebugStringA("Error al ejecutar QueryPerformanceCounter en " __FUNCTION__);
			}
		}
	}
	Crono::~Crono()	// Destructor
	{
	}

	// Inicia la medición de tiempos
	bool Crono::Inicio()
	{
		bool res = false;
		if (_ticsPorSegundoDelTimerDelPC > 0) {
			// Paso 2: guarda el valor actual del timer del PC en tics usando QueryPerformanceCounter 
			LARGE_INTEGER ticks;
			if (QueryPerformanceCounter(&ticks)) {
				_ticsAntes = ticks.QuadPart;
				res = true;
			}
			else {
				OutputDebugStringA("Error al ejecutar QueryPerformanceCounter en " __FUNCTION__);
			}
		}
		return res;
	}

	// Devuelve los segundos transcurridos desde la última llamada a Inicio
	double Crono::Lee()
	{
			double segundos= -1.0;
			// Paso 3: Obtiene el valor actual del timer del PC en tics usando QueryPerformanceCounter
			// y le resta el valor guardado en el paso 2 para obtener el número de tics transcurridos.
			// Divide este número por la frecuencia del timer del PC para obtener el número de segundos transcurridos.
			int64_t ahora;
			LARGE_INTEGER ticks;
			if (QueryPerformanceCounter(&ticks)) {
				ahora = ticks.QuadPart;
				segundos = (double)(ahora - _ticsAntes) / _ticsPorSegundoDelTimerDelPC;
			}
			else {
				printf("Error al ejecutar QueryPerformanceCounter en " __FUNCTION__);
			}
			return segundos;
	}

} // fin namespace ss

