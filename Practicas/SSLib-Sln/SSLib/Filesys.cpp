
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#define _LIB_EXPORTS_
#include "sslib/SSLib.h"

using namespace ss;

#include <shlwapi.h> // Para PathFindFileNameA
#include <PathCch.h> // Para PathCchRemoveBackslash

// Si no encuentra el archivo, devuelve nullptr
char* FileSys::BuscaArchivo(const char* directorioDondeSeBusca, const char* nombreArchivo)
{
	char* res = nullptr;
	_tiempoUltimoMetodo = 0.0;
	// Comprueba los parámetros usando CheckError
	if (_dbg.CheckError(directorioDondeSeBusca == nullptr, __FUNCTION__ ": directorioDondeSeBusca es null.\n")
		|| _dbg.CheckError(nombreArchivo == nullptr, __FUNCTION__ ": nombreArchivo es null.\n"))
		return nullptr;
	char ruta[SSLIB_MAX_RUTA];
	memcpy(ruta, directorioDondeSeBusca, strlen(directorioDondeSeBusca) + 1);
	int encontrados;
	char** archivos = ArchivosEnDirectorio(&encontrados, directorioDondeSeBusca, false);
	if (_dbg.CheckError(archivos == nullptr, __FUNCTION__ ": No se han encontrado archivos.\n"))
		return nullptr;
	for (int i = 0; i < encontrados; i++) {
		// Extrae el nombre del archivo de la ruta
		char* pArchivo = PathFindFileNameA(archivos[i]);
		if (pArchivo == nullptr)
			continue;

		if (CompareStringA(LOCALE_SYSTEM_DEFAULT, LINGUISTIC_IGNORECASE,
			pArchivo, (int)strlen(pArchivo), nombreArchivo, (int)strlen(pArchivo)) == CSTR_EQUAL) {
			res = archivos[i];
			break;
		}
	}
	_tiempoUltimoMetodo = _dbg.CronoLee();
	return res;
}
double ss::FileSys::TiempoUltimoMetodo()
{
	return _tiempoUltimoMetodo;
}
void FileSys::CuentaBytesNombreArchivo(const char* nombreDir, bool notificaErroresAcceso)
{
	char ruta[SSLIB_MAX_RUTA];

	snprintf(ruta, sizeof(ruta), "%s\\*", nombreDir); // Añade el patrón de búsqueda para FindFirstFile

	// busca archivos en la ruta que cumplan el patrón usando API de Windows
	WIN32_FIND_DATAA findFileData;
	HANDLE hFind = FindFirstFileA(ruta, &findFileData);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		if (notificaErroresAcceso)
			_dbg.CheckError(true,
				__FUNCTION__ ": No se ha podido abrir la ruta..\n%s\n", nombreDir);
		return;
	}
	// Acumula el número y tamaño de los nombres de archivos encontrados
	size_t bytesNombreDirectorio = strlen(nombreDir) + 1; // Suma 1 para el caracter '\\'
	do
	{
		if (strcmp(findFileData.cFileName, ".") == 0 || strcmp(findFileData.cFileName, "..") == 0)
			continue; // Salta "." y ".."

		snprintf(ruta, sizeof(ruta), "%s\\%s", nombreDir, findFileData.cFileName);
		size_t bytesNombreArchivo = NormalizaRutaGrande(ruta, sizeof(ruta)) + 1;
		if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) // Si es un directorio
		{
			CuentaBytesNombreArchivo(ruta, notificaErroresAcceso); // Llama recursivamente
		}
		else
		{
			_nroArchivos++;
			_bytesNombreArchivos += bytesNombreArchivo;
			//_dbg.DbgPrint("%d::(%d) %s\\%s\n", _nroArchivos, bytesNombreArchivo, nombreDir, findFileData.cFileName);
		}
	} while (FindNextFileA(hFind, &findFileData));
	FindClose(hFind); // Cierra la búsqueda
}

void FileSys::NombreArchivosEnDirectorio(const char* nombreDir, bool notificaErroresAcceso)
{

	char ruta[SSLIB_MAX_RUTA];
	snprintf(ruta, sizeof(ruta), "%s\\*", nombreDir); // Añade el patrón de búsqueda para FindFirstFile

	// busca archivos en la ruta que cumplan el patrón usando API de Windows
	WIN32_FIND_DATAA findFileData;
	HANDLE hFind = FindFirstFileA(ruta, &findFileData);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		if (notificaErroresAcceso)
			_dbg.CheckError(true,
				__FUNCTION__ ": No se ha podido abrir la ruta.\n%s\n", nombreDir);
		return;
	}
	do
	{
		if (strcmp(findFileData.cFileName, ".") == 0 || strcmp(findFileData.cFileName, "..") == 0)
			continue; // Salta "." y ".."
		snprintf(ruta, sizeof(ruta), "%s\\%s", nombreDir, findFileData.cFileName);
		size_t bytesNombreArchivo = NormalizaRutaGrande(ruta, sizeof(ruta)) + 1;// +1 porque snprintf no cuenta el caracter fin cadena
		if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) // Si es un directorio
		{
			NombreArchivosEnDirectorio(ruta, notificaErroresAcceso); // Llama recursivamente
		}
		else
		{
			// Copia el nombre del archivo en el buffer de nombres
			if (!_dbg.CheckError(bytesNombreArchivo > _bytesNombreArchivos - _idxBufferNombres,
				3, __FUNCTION__ ": Buffer de nombres insuficiente.\n")
				&& !_dbg.CheckError(_idxListaArchivos > _nroArchivos, 4, __FUNCTION__ ": Número de archivos excede el esperado.\n")
				) {
				strncpy(&_pBufferNombres[_idxBufferNombres],
					ruta, bytesNombreArchivo);
				_pListaArchivos[_idxListaArchivos] = &_pBufferNombres[_idxBufferNombres];
				//_dbg.DbgPrint("%d==(%d) %s\n", _idxListaArchivos, strlen(_pListaArchivos[_idxListaArchivos]) + 1, _pListaArchivos[_idxListaArchivos]);
				_idxListaArchivos++;
				_pListaArchivos[_idxListaArchivos] = nullptr;
				_idxBufferNombres += bytesNombreArchivo;
			}

		}
	} while (FindNextFileA(hFind, &findFileData));
	FindClose(hFind); // Cierra la búsqueda
}
size_t FileSys::NormalizaRutaGrande(char* rutaANormalizar, size_t tamRuta)
{
	size_t tam = 0;
	if (rutaANormalizar != nullptr && tamRuta > 0
		&& (tam = strnlen(rutaANormalizar, tamRuta)) < tamRuta) {
		if (tam >= MAX_PATH)
		{
			// Solo hace esta transformación si el nombre de archivo es mayor que MAX_PATH. 
			// En tal caso, algunas funciones Win32 de gestión del sistema de archivos no funcionan
			// correctamente si no se pasa al formato para nombres de archivo largos.
			if (0 != strncmp(rutaANormalizar, "\\\\?\\", 4)) {  // no está ya normalizada
				// Aquí no se puede usar una matriz de char porque tamRuta no es constante
				char* pCadTmp = new char[tamRuta];
				if (pCadTmp != nullptr) {
					tam = snprintf(pCadTmp, tamRuta, "\\\\?\\%s", rutaANormalizar); // Añade el prefijo "\\?\" para nombres de archivo largos
					pCadTmp[tamRuta - 1] = '\0'; // Asegura que la cadena esté terminada
					if (tam <= 0 || tam >= tamRuta) {
						tam = 0;
					}
					else {
						strncpy(rutaANormalizar, pCadTmp, tamRuta); // Copia la cadena normalizada
					}
					delete[] pCadTmp; // Libera la memoria
				}
				else
					tam = 0;
			}
		}
	}
	if (tam == 0)
		return 0;
	return tam;
}
char** FileSys::ArchivosEnDirectorio(int* encontrados, const char* nombreDir, bool notificaErroresAcceso)
{
	*encontrados = 0;
	_tiempoUltimoMetodo = 0.0;
	_dbg.CronoInicio();
	// Comprueba los parámetros usando CheckError
	if (_dbg.CheckError(encontrados == nullptr, __FUNCTION__ ": encontrados es null.\n")
		|| _dbg.CheckError(nombreDir == nullptr, __FUNCTION__ ": ruta es null.\n"))
		return nullptr;
	// Pasa el nombre del directorio a Unicode. 
	// Se tiene que pasar a Unicode para usar la función GetFullPathNameW. 
	// GetFullPathNameA no funciona con rutas de más de 260 caracteres. GetFullPathNameW sí.
	wchar_t nombreDirUnicodecode[SSLIB_MAX_RUTA];
	wchar_t nombreDirUnicodecodeFullPath[SSLIB_MAX_RUTA];
	if (_dbg.CheckError(MultiByteToWideChar(CP_ACP, 0, nombreDir, -1, nombreDirUnicodecode,
		sizeof(nombreDirUnicodecode) / sizeof(nombreDirUnicodecode[0]))
		== 0, "Error en MultiByteToWideChar %s\n", nombreDir)) {
		return nullptr;
	}
	if (_dbg.CheckError(GetFullPathNameW(nombreDirUnicodecode, SSLIB_MAX_RUTA, nombreDirUnicodecodeFullPath, nullptr) == 0,
		"Error en GetFullPathNameA %s\n", nombreDir))
		return nullptr;
	// Elimina la barra invertida final si la hay
	HRESULT  resAux = PathCchRemoveBackslash(nombreDirUnicodecodeFullPath, SSLIB_MAX_RUTA);
	if (_dbg.CheckError(resAux != S_OK && resAux != S_FALSE,  // Condición de error para esta función
		"Error en PathCchRemoveBackslash %s\n", nombreDir))
		return nullptr;

	char rutaDir[SSLIB_MAX_RUTA];
	// Pasa el nombre del directorio a ANSI
	if (_dbg.CheckError(WideCharToMultiByte(CP_ACP, 0, nombreDirUnicodecodeFullPath, -1, rutaDir, SSLIB_MAX_RUTA,
		nullptr, nullptr) == 0, "Error en WideCharToMultiByte %s\n", nombreDir))
		return nullptr;

	// Normaliza la ruta para evitar problemas con rutas mayores a 260 caracteres
	NormalizaRutaGrande(rutaDir, sizeof(rutaDir));

	// Comprueba que nombreDir es un directorio
	DWORD atributos = GetFileAttributesA(rutaDir);
	if (_dbg.CheckError(atributos == INVALID_FILE_ATTRIBUTES, __FUNCTION__ ": No se ha podido obtener los atributos del directorio.\n")
		|| _dbg.CheckError((atributos & FILE_ATTRIBUTE_DIRECTORY) == 0, "%s no es un directorio.\n", rutaDir)) // Si no es un directorio, FILE_ATTRIBUTE_DIRECTORY es 0
	{
		return nullptr;
	}

	// libera la memoria de nombres de archivos si ya se ha usado
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

	// Fin de comprobacibes previas. Enpieza el trabajo efectivo
	CuentaBytesNombreArchivo(rutaDir, notificaErroresAcceso);
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
	NombreArchivosEnDirectorio(rutaDir, notificaErroresAcceso);

	*encontrados = _nroArchivos;
	if (_pListaArchivos != nullptr)
		SetLastError(ERROR_SUCCESS);
	_tiempoUltimoMetodo = _dbg.CronoLee();
	return _pListaArchivos;
}



FileSys::FileSys()
{
	// Inicializa los miembros de la clase
	_bytesNombreArchivos = _nroArchivos = _idxBufferNombres = _idxListaArchivos = 0;
	_pListaArchivos = nullptr;
	_pBufferNombres = nullptr;
	_tiempoUltimoMetodo = -1.0;
}

FileSys::~FileSys()
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


// Método BuscaDatoEnArchivo, de la clase Dbg:
// devuelve el número de veces que se encuentra el bloque a buscar
// dentro de un archivo. También modifica incrementa bytesPorAhora con 
// el tamaño del archivo. 
int FileSys::BuscaDatoEnArchivo(const char* block, size_t blockSize, const char* fileName,
	size_t* bytesPorAhora, int tamBuffer) {
	// Abre archivo
	FILE* file_ptr;
	file_ptr = fopen(fileName, "rb");
	int res = 0;
	if (_dbg.CheckError(tamBuffer == 0, __FUNCTION__ ": Tamaño del buffer debe ser mayor que 0\n")) {
		return -1;
	}
	if (_dbg.CheckError(file_ptr == NULL, __FUNCTION__ ": No se pudo abrir el archivo %s\n", fileName)) {
		// comprueba si el archivo está en uso
		WIN32_FILE_ATTRIBUTE_DATA info;
		if (GetFileAttributesExA(fileName, GetFileExInfoStandard, &info)) {
			// Si no se puede abrir, añade a los bytes procesados el tamaño del archivo.
			*bytesPorAhora += ((int64_t)info.nFileSizeHigh >> 32) + info.nFileSizeLow;
		}
		return -1;
	}
	char* buffer = new char[tamBuffer];
	int idxBusqueda = 0;
	size_t bytesRead = fread(buffer, 1, tamBuffer, file_ptr);
	while (bytesRead > 0) {
		*bytesPorAhora += bytesRead;
		for (size_t i = 0; i < bytesRead; i++) {
			if (buffer[i] == block[idxBusqueda]) {
				idxBusqueda++;
				if (idxBusqueda == blockSize) {
					res++;
					idxBusqueda = 0;
				}
			}
			else {
				idxBusqueda = 0;
			}
		}
		bytesRead = fread(buffer, 1, tamBuffer, file_ptr);
	}
	fclose(file_ptr);
	delete[] buffer;  // delete aplicado a matriz.

	return res;
}


// El formato de cada linea será:
// 0000000000;0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ
#define FORMMATO_LINEA_TEST        "0000000000;0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ\n"
#define FORMATO_LINEA_TEST_PARA_PRINTF "%010d;0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ\n"
bool FileSys::CreaArchivoTest(const char* nombre, uint64_t tamArchivo, size_t tamBuffer, int64_t* pCuantoLLeva)
{
	bool res = false;
	if (tamBuffer < 1)
		tamBuffer = 1;
	try {
		// Crear el archivo de prueba
		HANDLE hFile = CreateFileA(nombre, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (!_dbg.CheckError(hFile == INVALID_HANDLE_VALUE, "No se pudo crear el archivo %s\n", nombre))
		{
			unsigned int nroLinea = 0;
			// Cadena ASCII para escribir en el archivo
			char lineaFormateada[sizeof(FORMMATO_LINEA_TEST)] = FORMMATO_LINEA_TEST;

			unsigned int idx = 0;
			uint64_t nroEscrituras = 0; // todas las variables relacionadas con el tamaño del archivo son int64_t
			// ya que los archivos pueden ser mayores de 4 GB

			// Crear un búfer para escribir en el archivo
			uint8_t* pbuffer = new uint8_t[tamBuffer];
			snprintf(lineaFormateada, sizeof(lineaFormateada), FORMATO_LINEA_TEST_PARA_PRINTF, nroLinea++);
			for (uint64_t i = 0; i < tamArchivo; i += tamBuffer)
			{
				uint64_t j;
				for (j = 0; j < tamBuffer; j++) {
					pbuffer[j] = lineaFormateada[idx++];
					// Cuando idx sea igual que el tamaño de la cadena, se pone a 0
					if (lineaFormateada[idx] == '\0') {
						idx = 0;
						snprintf(lineaFormateada, sizeof(lineaFormateada), FORMATO_LINEA_TEST_PARA_PRINTF, nroLinea++);
					}
				}
				// Escribir el búfer en el archivo
				int64_t tamAEscribir = min(tamBuffer, tamArchivo - i);
				DWORD escritos;
				BOOL ok = WriteFile(hFile, pbuffer, (DWORD)tamAEscribir, &escritos, NULL);
				if (!_dbg.CheckError(ok == FALSE || escritos != (DWORD)tamAEscribir,
					"Error escribiendo en el archivo de test %s\n", nombre))
				{
					nroEscrituras += escritos;
					if (pCuantoLLeva != nullptr)
						*pCuantoLLeva = nroEscrituras;
				}
				else
					break;
			}
			CloseHandle(hFile);
			res = nroEscrituras == (int64_t)tamArchivo;
			_dbg.CheckError(!res,
				"Error: el tamaño escrito en el archivo de test %s es incorrecto. Esperado: %llu bytes, escritos: %llu bytes\n",
				nombre, tamArchivo, nroEscrituras);
			if (res)
				_dbg.DbgPrint("Creado archivo de test (%d bytes, buffer: %d):\n%s\n",
					tamArchivo, tamBuffer, nombre);
			delete[] pbuffer;
			res = true;
		}
	}
	catch (const std::exception& e) {
		_dbg.DbgPrint("Excepción al crear archivo de test %s: %s\n", nombre, e.what());
	}
	return res;
}

bool FileSys::AbreArchivoParaLectura(const char* nombreArchivo)
{
	CierraArchivoLectura();
	_hArchivoLectura = CreateFileA(nombreArchivo, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	bool res = _hArchivoLectura != INVALID_HANDLE_VALUE;
	if (res)
	{
		strncpy_s(_ultimoNombreArchivoLectura, MAX_PATH, nombreArchivo, _TRUNCATE);
	}
	_dbg.CheckError(!res, "No se pudo abrir el archivo para lectura: %s\n", nombreArchivo);
	return res;
}
bool FileSys::LeeBloque(const char* nombreArchivo, uint8_t* pBuffer, size_t tamBuffer, size_t* pLeidos)
{
	*pLeidos = 0;
	DWORD leidos = 0;
	BOOL res = _hArchivoLectura != INVALID_HANDLE_VALUE && ReadFile(_hArchivoLectura, pBuffer, (DWORD)tamBuffer, &leidos, NULL);
	if (!_dbg.CheckError(res == FALSE, "Error leyendo bloque del archivo %s\n", nombreArchivo))
	{
		*pLeidos = (size_t)leidos;
	}
	return res;
}
bool FileSys::CierraArchivoLectura()
{
	BOOL res = FALSE;
	if (_hArchivoLectura != INVALID_HANDLE_VALUE)
	{
		res = CloseHandle(_hArchivoLectura);
		_dbg.CheckError(res == FALSE, "Error cerrando el archivo %s\n", _ultimoNombreArchivoLectura);
		_hArchivoLectura = INVALID_HANDLE_VALUE;
	}
	_ultimoNombreArchivoLectura[0] = '\0';
	return res;
}
bool FileSys::AbreArchivoParaEscritura(const char* nombreArchivo)
{
	CierraArchivoEscritura();
	_hArchivoEscritura = CreateFileA(nombreArchivo, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	bool res = _hArchivoLectura != INVALID_HANDLE_VALUE;
	if (res)
	{
		strncpy_s(_ultimoNombreArchivoEscritura, MAX_PATH, nombreArchivo, _TRUNCATE);
	}
	_dbg.CheckError(!res, "No se pudo abrir el archivo para escritura: %s\n", nombreArchivo);
	return res;
}
bool FileSys::CierraArchivoEscritura()
{
	BOOL res = FALSE;
	if (_hArchivoEscritura != INVALID_HANDLE_VALUE)
	{
		res = CloseHandle(_hArchivoEscritura);
		_dbg.CheckError(res == FALSE, "Error cerrando el archivo %s\n", _ultimoNombreArchivoEscritura);
		_hArchivoEscritura = INVALID_HANDLE_VALUE;
	}
	_ultimoNombreArchivoEscritura[0] = '\0';
	return res;
}
bool FileSys::EscribeBloque(const char* nombreArchivo, const uint8_t* pBuffer, size_t tamBuffer)
{
	DWORD escritos = 0;
	BOOL res = _hArchivoEscritura != INVALID_HANDLE_VALUE && WriteFile(_hArchivoEscritura, pBuffer, (DWORD)tamBuffer, &escritos, NULL);
	if (!_dbg.CheckError(res == FALSE, "Error escribiendo bloque del archivo %s\n", nombreArchivo))
	{
		if (escritos != tamBuffer)
		{
			res = false;
			_dbg.CheckError(true, "Error escribiendo bloque del archivo %s: escritos %d bytes, se querían escribir %d bytes\n",
				nombreArchivo, escritos, tamBuffer);
		}
	}
	return res;
}

