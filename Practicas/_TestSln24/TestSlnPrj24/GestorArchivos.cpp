
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "GestorArchivos.h"

using namespace ss;

#include <shlwapi.h> // Para PathFindFileNameA

// Si no encuentra el archivo, devuelve nullptr
char* GestorArchivos::BuscaArchivo(const char* directorioDondeSeBusca, const char * nombreArchivo)
{
	char * res = nullptr; 

	if (_dbg.CheckError(directorioDondeSeBusca == nullptr, __FUNCTION__ ": directorioDondeSeBusca es null.\n")
		|| _dbg.CheckError(nombreArchivo == nullptr, __FUNCTION__ ": nombreArchivo es null.\n"))
		return nullptr;
	char ruta[MAX_PATH];
	memcpy(ruta, directorioDondeSeBusca, strlen(directorioDondeSeBusca) + 1);
	int encontrados;
	char** archivos = ArchivosEnDirectorio(&encontrados, directorioDondeSeBusca, false);
	if (_dbg.CheckError(archivos == nullptr, __FUNCTION__ ": No se han encontrado archivos.\n"))
		return nullptr;
	for (int i = 0; i < encontrados; i++){

		char * pArchivo = PathFindFileNameA(archivos[i]);
		if (pArchivo == nullptr)
			continue;

		if (CompareStringA(LOCALE_SYSTEM_DEFAULT, LINGUISTIC_IGNORECASE,
			pArchivo, sizeof(pArchivo), nombreArchivo, sizeof(pArchivo))) {
			res = archivos[i];
			break;
		}
	}
	return res;
}
void GestorArchivos::CuentaBytesNombreArchivo(const char* nombreDir)
{
	char ruta[MAX_PATH];
	snprintf(ruta, sizeof(ruta), "%s\\*", nombreDir); // Añade el patrón de búsqueda para FindFirstFile

	// busca archivos en la ruta que cumplan el patrón usando API de Windows
	WIN32_FIND_DATAA findFileData;
	HANDLE hFind = FindFirstFileA(ruta, &findFileData);
	if (_dbg.CheckError(hFind == INVALID_HANDLE_VALUE, __FUNCTION__ ": No se ha podido abrir la ruta.\n"))
	{
		return;
	}
	// Acumula el número y tamaño de los nombres de archivos encontrados
	size_t bytesNombreDirectorio = strlen(nombreDir) + 1; // Suma 1 para el caracter '\\'
	do
	{
		if (strcmp(findFileData.cFileName, ".") == 0 || strcmp(findFileData.cFileName, "..") == 0)
			continue; // Salta "." y ".."

		if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) // Si es un directorio
		{
			char rutaDir[MAX_PATH];
			snprintf(rutaDir, sizeof(rutaDir), "%s\\%s", nombreDir, findFileData.cFileName);
			CuentaBytesNombreArchivo(rutaDir); // Llama recursivamente
		}
		else 
		{
			_nroArchivos++;
			_bytesNombreArchivos += bytesNombreDirectorio + strlen(findFileData.cFileName) + 1; // +1 para el caracter fin cadena
		}
	} while (FindNextFileA(hFind, &findFileData));
	FindClose(hFind); // Cierra la búsqueda
}

void GestorArchivos::NombreArchivosEnDirectorio(const char* nombreDir)
{
	char ruta[MAX_PATH];
	snprintf(ruta, sizeof(ruta), "%s\\*", nombreDir); // Añade el patrón de búsqueda para FindFirstFile

	// busca archivos en la ruta que cumplan el patrón usando API de Windows
	WIN32_FIND_DATAA findFileData;
	HANDLE hFind = FindFirstFileA(ruta, &findFileData);
	if (_dbg.CheckError(hFind == INVALID_HANDLE_VALUE, __FUNCTION__ ": No se ha podido abrir la ruta.\n"))
	{
		return;
	}
	do
	{
		if (strcmp(findFileData.cFileName, ".") == 0 || strcmp(findFileData.cFileName, "..") == 0)
			continue; // Salta "." y ".."

		if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) // Si es un directorio
		{
			char rutaDir[MAX_PATH];
			snprintf(rutaDir, sizeof(rutaDir), "%s\\%s", nombreDir, findFileData.cFileName);
			NombreArchivosEnDirectorio(rutaDir); // Llama recursivamente
		}
		else
		{
			// Copia el nombre del archivo en el buffer de nombres
			int nuevos= snprintf(&_pBufferNombres[_idxBufferNombres], _bytesNombreArchivos - _idxBufferNombres,
				"%s\\%s", nombreDir, findFileData.cFileName);
			if (_dbg.CheckError(nuevos < 0, __FUNCTION__ ": Error al copiar el nombre del archivo.\n"))
			{
				return;
			}
			_dbg.CheckError(nuevos >= _bytesNombreArchivos - _idxBufferNombres,3, __FUNCTION__ ": Buffer de nombres insuficiente.\n");
			_pListaArchivos[_idxListaArchivos] = &_pBufferNombres[_idxBufferNombres];
			_idxListaArchivos++;
			_dbg.CheckError(_idxListaArchivos > _nroArchivos, 4, __FUNCTION__ ": Número de archivos excede el esperado.\n");
			_pListaArchivos[_idxListaArchivos] = nullptr;
			_idxBufferNombres += nuevos + 1; // +1 porque snprintf no cuenta el caracter fin cadena
		}
	} while (FindNextFileA(hFind, &findFileData));
	FindClose(hFind); // Cierra la búsqueda
}

char** GestorArchivos::ArchivosEnDirectorio(int* encontrados, const char* nombreDir, bool imprimir)
{
	*encontrados = 0;
	// Comprueba los parámetros usando CheckError
	if (_dbg.CheckError(encontrados == nullptr, __FUNCTION__ ": encontrados es null.\n")
		|| _dbg.CheckError(nombreDir == nullptr, __FUNCTION__ ": ruta es null.\n"))
		return nullptr;
	// Comprueba que nombreDir es un directorio
	DWORD atributos = GetFileAttributesA(nombreDir);
	if (_dbg.CheckError(atributos == INVALID_FILE_ATTRIBUTES, __FUNCTION__ ": No se ha podido obtener los atributos del directorio.\n")
		|| _dbg.CheckError((atributos & FILE_ATTRIBUTE_DIRECTORY) == 0, "%s no es un directorio.\n", nombreDir)) // Si no es un directorio, FILE_ATTRIBUTE_DIRECTORY es 0
	{
		return nullptr;
	}
	// Compureba libera la memoria de nombres de archivos si ya se ha usado
	if (_pListaArchivos != nullptr)
	{
		delete[] _pListaArchivos;
	}
	if (_pBufferNombres != nullptr)
	{
		delete[] _pBufferNombres;
	}
	_bytesNombreArchivos = _nroArchivos = _idxBufferNombres = _idxListaArchivos = 0;
	_pListaArchivos = _pListaArchivos = nullptr;

	CuentaBytesNombreArchivo(nombreDir);
	if (_dbg.CheckError(_nroArchivos == 0, __FUNCTION__ ": No se han encontrado archivos en el directorio.\n"))
	{
		return nullptr;
	}
	// Reserva memoria para la matriz de punteros a cadenas usando new
	_pListaArchivos = new char* [_nroArchivos + 1];	// +1 para el puntero nulo al final
	if (_dbg.CheckError(_pListaArchivos == nullptr, __FUNCTION__ ": No se ha podido reservar memoria para _pListaArchivos.\n"))
	{
		return nullptr;
	}
	// Reserva memoria para los nombres de los archivos
	_pBufferNombres = new char[_bytesNombreArchivos];
	if (_dbg.CheckError(_pBufferNombres == nullptr, __FUNCTION__ ": No se ha podido reservar memoria para _pBufferNombres"))
	{
		delete _pListaArchivos;
		_pListaArchivos = nullptr;
		return nullptr;
	}
	NombreArchivosEnDirectorio(nombreDir);

	*encontrados = _nroArchivos;
	return _pListaArchivos;
}


GestorArchivos::GestorArchivos()
{
	// Inicializa los miembros de la clase
	_bytesNombreArchivos = _nroArchivos = _idxBufferNombres= _idxListaArchivos= 0;
	_pListaArchivos = _pListaArchivos = nullptr;
}

GestorArchivos::~GestorArchivos()
{
	// Importante: PENDIENTE liberar memoria reservada!!!
	if (_pListaArchivos != nullptr)
	{
		delete[] _pListaArchivos;
	}
	if (_pBufferNombres != nullptr)
	{
		delete[] _pBufferNombres;
	}
}

