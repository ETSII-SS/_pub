#include "TestLibClasico.h"
static Dbg dbg(true); // Salida por pantalla y por depuración
static FileSys fs;

bool ProcesaBloque(uint8_t* pBuffer, size_t tamBuffer)
{
	int res= LCMapStringA(
		LOCALE_SYSTEM_DEFAULT, // LCID Locale,
		LCMAP_UPPERCASE,      // DWORD dwMapFlags,
		(LPCSTR)pBuffer,      // LPCSTR lpSrcStr,
		(int)tamBuffer,       // int cchSrc,
		(LPSTR)pBuffer,       // LPSTR lpDestStr,
		(int)tamBuffer        // int cchDest
	);
	if (res == 0)
	{
		dbg.CheckError(true, "Error convirtiendo a mayúsculas el bloque de datos\n");
		// Error
	}
	return true;
}
int TestP5_2025_main(int argc, char* argv[], char* envp[])
{
	setlocale(LC_ALL, "Spanish");

	bool res;

	size_t tamBufferEscritura = 4*1024;
	int tamArchivoMB = 500;
	auto nombreArchivo = "c:\\tmp\\p5.txt";
	auto nombreArchivoSalida = "c:\\tmp\\p5Salida.txt";
	//for (int i = 1; i < 4096; i++)
	//{
	//	NuevoArchivoTest(nombreArchivo, tamArchivoMB, i);
	//}
	res =NuevoArchivoTest(nombreArchivo, tamArchivoMB, tamBufferEscritura);
	//NuevoArchivoTest(res, "d:\\p5.txt", tamArchivoMB, tamBuffer);
	//NuevoArchivoTest(res, "F:\\p5.txt", tamArchivoMB, tamBuffer);

	// Pasa a mayusculas el archivo creado
	
	size_t tamBuffer = 4 * 1024, leidos= 0;
	uint8_t* pBuffer = new uint8_t[tamBuffer];
	uint32_t procesados = 0;
	printf("Procesando el archivo %s ...\n", nombreArchivo);
	dbg.CronoInicio();
	res = fs.AbreArchivoParaLectura(nombreArchivo);
	res = res && fs.AbreArchivoParaEscritura(nombreArchivoSalida);
	if (res)
	{
		while (fs.LeeBloque(nombreArchivo, pBuffer, tamBuffer, &leidos)
			&& leidos > 0)
		{
			if (!ProcesaBloque(pBuffer, leidos))
			{
				break;
			}
			if (!fs.EscribeBloque(nombreArchivoSalida, pBuffer, leidos))
			{
				break;
			}
			procesados += leidos;
		}
	}
	fs.CierraArchivoLectura();
	fs.CierraArchivoEscritura();
	delete[] pBuffer;
	dbg.CheckError(procesados != (tamArchivoMB * 1024 * 1024),
		"Error procesando el archivo %s: procesados %d bytes, se esperaban %d bytes\n",
		nombreArchivo, procesados, tamArchivoMB * 1024 * 1024);
	double segsLectura = dbg.CronoLee();
	printf("Archivo %s procesado en %f segundos (%f MB/s, buffer %d)\n",
		nombreArchivo, segsLectura, (tamArchivoMB) / (segsLectura), tamBuffer);
	printf("\nPor favor, pulse la tecla ENTRAR para terminar ...\n");
	(void)getchar();  
	return 0;
}

bool NuevoArchivoTest(const char* nombreArchivo, uint64_t tamArchivoMB, size_t tamBuffer)
{
	dbg.CronoInicio();
	bool res = fs.CreaArchivoTest(nombreArchivo, tamArchivoMB * 1024 * 1024, tamBuffer); // Crea un archivo de prueba de 10 MB con buffer de 4 KB
	dbg.CheckError(!res, 1, "No se pudo crear archivo de test");
	double segsEscritura = dbg.CronoLee();
	segsEscritura = segsEscritura > 0.0 ? segsEscritura : 0.0001; // Evita división por cero
	printf("%s (%llu MB) creado en %f segundos (%f MB/s, buffer %llu)\n", nombreArchivo, tamArchivoMB, segsEscritura, (10.0f) / (segsEscritura), tamBuffer);
	return res;
}
