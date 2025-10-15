#include "Todos-LibInfoCPU.h"
/// Devuelve en CadModelo la cadena de modelo del procesador 
//			El valor devuelto no está definido en el enunciado de la práctica. Lo defino aquí:
//			0  Características extendidas de CPUID no soportadas en este procesador. Devuelve "" en CadModelo
//			1  Devuelve cadena de modelo en CadModelo.
int  LeeIDModelo (char * CadModelo)
{
    int Info[4];
	int * pEntero;
	int i, CodigoEAX, ContadorEnteros;
	
	pEntero= (int *)CadModelo; // pEntero y CadModelo apuntan al mismo bloque de datos.
		// pEntero permite tratarlos como una tabla de enteros.
		// CadModelo permite tratarlos como una tabla de chars. 

    __cpuid(Info,0x80000000);	// Comprueba que el procesador soporta las caracteristicas extendidas de EAX
    if(Info[0] < 0x80000000)
	{
		// No soporta las características extendidas. Aquí no se entra a menos que tengamos 
		// un modelo de procesador de hace + de 10 años
		CadModelo[0]= '\0'; // Pone el caracter de fin de cadena (0) en CadModelo. 
						// La cadena "" en C sólo contiene el caracter de fin de cadena
        return 0;
	}
	ContadorEnteros= 0;
	// Ejecuta __cpuid con los códigos 0x80000002, 0x80000003 y 0x80000004
	for (CodigoEAX= 0x80000002; CodigoEAX <= 0x80000004; CodigoEAX++)
	{
		__cpuid(Info,CodigoEAX);	
		for (i= 0; i<4; i++)	// Copia el bloque de 4x4 caracteres
		{
			pEntero[ContadorEnteros]= Info[i];	// copia 4 caracteres
			ContadorEnteros++;	// Señala dónde estamos en la cadena de destino
		}
	}
	// Pone el carácter de fin de cadena al final
	CadModelo[ContadorEnteros*4]= '\0';
	return 1;
	// Este programa tiene dependencias de la arquitectura del procedsador (aparte de __cpuid). ¿Donde?
}