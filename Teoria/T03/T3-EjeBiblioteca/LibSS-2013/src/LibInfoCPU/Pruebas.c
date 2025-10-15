//#include "Todos-LibInfoCPU.h"
//#include <assert.h>
//
//static void __PruebaLeeIDCaracteristica();
//static void __PruebaLeeIDModelo();
//
//int main()
//{
//	__PruebaLeeIDModelo();
//	__PruebaLeeIDCaracteristica();
//	return 0;
//}
//
//
//static void __PruebaLeeIDModelo()
//{
//	char Resultado[49];
//	
//	assert(0!=LeeIDModelo(Resultado));
//	printf("Cadena de modelo: #%s# \n\n Pulsar una tecla para salir de __PruebaLeeIDModelo...", Resultado);
//	printf("\n\n Pulsar una tecla para salir de __PruebaLeeIDModelo...");
//	getchar();
//}
//static void __PruebaLeeIDCaracteristica()
//{
//	int i;
//	char * Cadena;
//	
//	// Test de LeeIDCaracterística
//	for (i= 0; i<64; i++)
//	{
//		Cadena= LeeCaracteristica(i);
//		if (Cadena==LEE_CARACTERISTICA_FABRICANTE_NO_SOPORTADO)
//		{
//			LeeIDFabricante(Cadena);
//			printf("El procesador con ID de fabricante \"%s\" no está soportado por LeeCaracteristica. \n", Cadena);
//			break;
//		}
//		else if (Cadena==LEE_CARACTERISTICA_BIT_CARACTERISTICA_RESERVADO)
//			printf("Bit #%d: **** RESERVADO.\n", i, Cadena);
//		else if (Cadena==LEE_CARACTERISTICA_CPU_SIN_CARACTERISTICA)
//			printf("Bit #%d: ** SIN IMPLEMENTAR en la CPU.\n", i, Cadena);
//		else
//			printf("Bit #%d:  %s\n", i, Cadena);
//	}
//	printf("\n\n Pulsar una tecla para salir de __PruebaLeeIDCaracteristica...");
//	getchar();
//
//}