#include "Todos-LibInfoCPU.h"

/// Le pasamos un número de característica y nos devuelve una cadena de texto con 
/// el nombre de la característica, o una indicación de que la característica 
//  no existe o no la posee el procesador.
char * LeeCaracteristica (int NumeroBit)
{
	int Info[4];	// Aquí se guarda el resultado de CPUID
	int RegistroAMirar;
	char IDFabricante[13];	// Aquí leemos el ID de fabricante
	Caracteristica_t * pTabla, * pTablaECX, * pTablaEDX;	// Aquí se guarda la tabla de características a usar
	int i;

	LeeIDFabricante(IDFabricante);	// Lee el fabricante

	if (0==strcmp(IDFabricante, "GenuineIntel"))	// Es Intel?
	{
		pTablaECX= TablaCaracteristicasECXIntel;
		pTablaEDX= TablaCaracteristicasEDXIntel;
	}
	else
		if (0==strcmp(IDFabricante, "AMDInstead"))	// Es AMD?
		{
			pTablaECX= TablaCaracteristicasECXAMD;
			pTablaEDX= TablaCaracteristicasEDXAMD;
		}
		else
			return (char *) LEE_CARACTERISTICA_FABRICANTE_NO_SOPORTADO; // Solo soportados estos dos

	__cpuid(Info, 1);	// Ejecuta CPUID con EAX a 1

	// Selecciona una u otra tabla dependiendo del valor de NumeroBit
	if (NumeroBit<32)
	{	// Es ECX el que debemos mirar
		pTabla= pTablaECX;
		RegistroAMirar= Info[2];
	}
	else
	{	// Es EDX el que debemos mirar
		pTabla= pTablaEDX;
		RegistroAMirar= Info[3];
		NumeroBit= NumeroBit -32;	// Se ajusta el valor de NumeroBit al desplazamiento en EDX
	}
	
	// Busca el número de bit dentro de la tabla
	for (i= 0; i< 32; i++)
	{
		if (pTabla[i].NroBit == -1)
			break;
		if (pTabla[i].NroBit == NumeroBit)
		{
			if (((RegistroAMirar>>NumeroBit) & 1)== 0)
				return LEE_CARACTERISTICA_CPU_SIN_CARACTERISTICA;	// Se encuentra la característica, pero el procesador no la tiene
			else
				return pTabla[i].Descripcion;	// devuelve descripción de la caracteristica
		}
	}
	return (char *) LEE_CARACTERISTICA_BIT_CARACTERISTICA_RESERVADO;
}
