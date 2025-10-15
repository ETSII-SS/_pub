#pragma once

#include <stdint.h>
#include <vector>
#include <Windows.h>
#include <sslib/SSLib.h>


using namespace std;

namespace ss {
	class GestorArchivos
	{
	private:	// Opcional, por defecto todo es privado
		size_t _bytesNombreArchivos;
		int _nroArchivos;
		char** _pListaArchivos;
		char* _pBufferNombres;
		int _idxListaArchivos;
		int _idxBufferNombres;
		Dbg _dbg;
		void CuentaBytesNombreArchivo(const char* nombreDir);
		void NombreArchivosEnDirectorio(const char* nombreDir);
	public:
		GestorArchivos();
		~GestorArchivos();
		char ** ArchivosEnDirectorio(int* encontrados, const char* nombreDir, bool imprimir= false);
		char * BuscaArchivo(const char* nombreDir, const char* nombreArchivo);
	};

} // Fin namespace