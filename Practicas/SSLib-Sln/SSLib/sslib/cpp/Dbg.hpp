#pragma once

#include <vector>
#include <string>
#include <Windows.h>

#include "Crono.hpp"

using namespace std;


namespace ss {
	class LIB_API Dbg
	{
	private:	// Opcional, por defecto todo es privado
		const char* cabeceraError = "ERROR";
		bool _salidaDepuracion; // Si es true, se envía la salida a la ventana de depuración
		bool _salidaPantalla; // Si es true, se envía la salida a la consola de salida
		Crono _crono;
		static bool _primerUsoClase; // Variable estática de clase
	public:
		const char* Vesion = "2025.12.03_00";
		Dbg(bool salidaPorPantalla = false, bool salidaPorDepuracion = true);
		~Dbg();

		/// <summary>
		/// Comprueba la condición de error que se le pasa en el primer argumento y actua en consecuencia
		/// </summary>
		/// <param name="condicionError"> Condición de error a comprobar. Si false no hace nada. Si true, USA los parámetros que sigue...</param>
		/// <param name="codigoSalida"> Si condicionError es true, termina el programa si este parámetro es distinto de 0</param>
		/// <param name="mensajeUsuario"> Si condicionError es true, imprime un mensaje de error como lo haría un printf: 
		/// mensajeUsuario es la cadena de formato, y los siguientes parámetros se usan como la lista opcional de parámetros de un printf</param>
		/// <param name="">Lista de parámetros opcionales</param>
		/// <returns>devuelve el valor que se le pasa en condicionError</returns>
		bool CheckError(bool condicionError, int codigoSalida, const char* mensajeUsuario, ...);

		/// <summary>
		/// Comprueba la condición de error que se le pasa en el primer argumento y actua en consecuencia
		/// </summary>
		/// <param name="condicionError"> Condición de error a comprobar. Si false no hace nada. Si true, USA los parámetros que sigue...</param>
		/// <param name="mensajeUsuario"> Si condicionError es true, imprime un mensaje de error como lo haría un printf: 
		/// mensajeUsuario es la cadena de formato, y los siguientes parámetros se usan como la lista opcional de parámetros de un printf</param>
		/// <param name="">Lista de parámetros opcionales</param>
		/// <returns>devuelve el valor que se le pasa en condicionError</returns>
		bool CheckError(bool condicionError, const char* mensajeUsuario, ...);

		/// <summary>
		/// Inicia medición de tiempos.
		/// </summary>
		/// <returns></returns>
		bool CronoInicio() {	// Ejemplo de método interno (definido en el propio archivo .h)
			return _crono.Inicio();
		}
		/// <summary>
		/// Lee segundos transcurridos desde la última llamada a CronoInicio.
		/// </summary>
		/// <returns></returns>
		double CronoLee() {	// Ejemplo de método interno (definido en el propio archivo .h)
			return _crono.Lee();
		}
		/// <summary>
		/// Funciona igual que printf, pero muestra la salida solo por la ventana de salida (depuración) y por consola (opcional)
		/// </summary>
		int DbgPrint(const char* formatString, ...);
	};

} // Fin namespace
