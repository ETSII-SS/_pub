#include "Todos-LibInfoCPU.h"

// static: este símbolo no es "visible" fuera del archivo .c (símbolo "privado" al archivo .c)

/// Imprime en pantalla una tabla
static void ImprimeUnaTabla(Caracteristica_t * pTabla, char * Cabecera)
{
	int i=0;
	// Imprime la cabecera
	printf("\n********************************************************************\n");
	printf("** TABLA:  %s \n", Cabecera);
	printf("********************************************************************\n");

	// Recorre toda la tabla de características
	do
	{
		printf("bit #: %d, => % s:  %s\n",
			pTabla[i].NroBit, pTabla[i].NombreBit, pTabla[i].Descripcion);
		i++;	// Siguiente elemento
	} while (pTabla[i].NroBit != -1);	// Sale si encuentra la marca de fin de tabla
}

/// Imprime todas las tablas de características. Esta función sí es publica
void ImprimeTablasCaracteristicas()
{
	int i=0;
	ImprimeUnaTabla(TablaCaracteristicasECXIntel, "Tabla Caracteristicas ECX Intel");
	ImprimeUnaTabla(TablaCaracteristicasEDXIntel, "Tabla Caracteristicas EDX Intel");
	ImprimeUnaTabla(TablaCaracteristicasECXAMD, "Tabla Caracteristicas ECX AMD");
	ImprimeUnaTabla(TablaCaracteristicasEDXAMD, "Tabla Caracteristicas EDX AMD");
}