#include "Todos-LibInfoCPU.h"

/// Lee la cadena de identificación del fabricante
//		CadFabricante:	dirección inicial de la cadena donde se almacenará 
//          el mensaje del fabricante. Debe tener al menos 13 caracteres.
//		Devuelve el valor de EAX tras ejecutar CPUID. 

int LeeIDFabricante (char * pCadena)
{
	int Info[4];	// Aquí se guardarña el resultado de ejecutar __cpuid
	int * pEntero;	// Puntero auxiliar. Sirve para tratar la información que 
					// se guarda en pCadena como enteros

	__cpuid(Info,0);

	pEntero= (int *) pCadena; //pEntero y pCadena tienen ahora la misma dirección de memoria

	pEntero[0]= Info[1];	// Primeros 4 bytes.
	pEntero[1]= Info[3];	
	pEntero[2]= Info[2];	// Últimos
	pCadena[12]= '\0';	// Fin de cadena. 
	return Info[0];	// Devuelve el contenido de EAX tras ejecutar __cpuid
}
