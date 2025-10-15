#pragma once
#include "sslib/c/CUtils.h"
#include <stdint.h>

// Namespace: agrupación de clases, funciones y variables globales
namespace ss {

	/// <summary>
	/// Sirve para medir tiempos de ejecución
	/// </summary>
	class  LIB_API Crono {	// Declaración de la clase Crono

	private:	//opcional. por defecto todo es privad
		int64_t _ticsAntes = 0;
		int64_t _ticsPorSegundoDelTimerDelPC = -1;
	public:
		Crono();  // Constructor (opcional)
		~Crono(); // Destructor (opcional)

		/// <summary>
		/// Inicia medición de tiempos
		/// </summary>
		/// <returns>false: fallo en el inicio de la medición</returns>
		bool Inicio();

		/// <summary>
		/// Lee segundos transcurridos desde la última llamada a Inicio.
		/// </summary>
		/// <returns>-1.0 indica error</returns>
		double Lee();
	};
} // namespace ss