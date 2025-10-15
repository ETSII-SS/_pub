#pragma once

#include <stdint.h>
#include <vector>
#include <Windows.h>
#include <sslib/SSLib.h>


using namespace std;

namespace ss {
	class LIB_API FileSys
	{
	private:	// Opcional, por defecto todo es privado
		size_t _bytesNombreArchivos;
		int _nroArchivos;
		char** _pListaArchivos;
		char* _pBufferNombres;
		int _idxListaArchivos;
		int _idxBufferNombres;
		Dbg _dbg;
		void CuentaBytesNombreArchivo(const char* nombreDir, bool notificaErroresAcceso);
		void NombreArchivosEnDirectorio(const char* nombreDir, bool notificaErroresAcceso);
		/// <summary>
		/// Si la ruta tiene más de 260 caracteres, coloca el prefijo "\\?\" para que pueda ser procesada
		/// En funciones de la API Win32 de gestión de archivos.
		/// </summary>
		/// <param name="rutaANormalizar"></param>
		/// <param name="tamRuta"></param>
		/// <returns></returns>
		size_t NormalizaRutaGrande(char* rutaANormalizar, size_t tamRuta);
		double _tiempoUltimoMetodo;
	public:
		FileSys();
		~FileSys();
		/// <summary>
		/// Obtiene nombres de archivos en un directorio
		/// </summary>
		/// <param name="encontrados">Número de archivos encontrados</param>
		/// <param name="nombreDir">Nombre del directorio</param>
		/// <param name="notificaErroresAcceso">Da mensajes de error en los directorios sin permiso de acceso</param>
		/// <returns></returns>
		char** ArchivosEnDirectorio(int* encontrados, const char* nombreDir, bool notificaErroresAcceso = true);
		char* BuscaArchivo(const char* nombreDir, const char* nombreArchivo);
		/// <summary>
		/// Devuelve el tiempo de ejecución, en segundos, del último método llamado
		/// </summary>
		/// <returns>Un valor menor que 0.0 indica que no se ha ejecutado ningún metodo</returns>
		double TiempoUltimoMetodo();
		/// <summary>
		/// Busca un bloque de datos en un archivo.
		/// </summary>
		/// <param name="block"></param>
		/// <param name="blockSize"></param>
		/// <param name="fileName"></param>
		/// <param name="bytesPorAhora"></param>
		/// <param name="tamBuffer"></param>
		/// <returns>Devuelve el número de veces que se encuentra el bloque a buscar dentro de un archivo. 
		/// También modifica incrementa bytesPorAhora con el tamaño del archivo. </returns>
		int BuscaDatoEnArchivo(const char* block, size_t blockSize, const char* fileName, size_t* pBytesPorAhora, int tamBuffer=1024);

	};

} // Fin namespace