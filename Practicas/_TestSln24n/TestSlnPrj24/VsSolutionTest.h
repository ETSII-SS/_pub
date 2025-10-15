#pragma once
#include <stdint.h>
#include <vector>
#include <Windows.h>


using namespace std;


namespace ss {
	class VsSolutionTest
	{
	private:	// Opcional, por defecto todo es privado
		/// <summary>
		/// Lanza un proceso con el comando que se le pasa
		/// </summary>
		/// <param name="comando"></param>
		/// <param name="hProceso">Ejemplo de parámetro por referencia (Solo C++)</param>
		/// <returns></returns>
		bool LanzaProceso(const char* comando, HANDLE &hProceso);

	public:
		VsSolutionTest();
		~VsSolutionTest();
		int CompruebaSolucion(int nroPractica, const char* uvus, const char* visualStudioExe, 
			bool modoTest= false);
	};

} // Fin namespace
