#pragma once
#include <intrin.h>

// está definido cuando se compila el proyecto como DLL
#ifdef _WINDLL
#define _EXPORT __declspec (dllexport)
#else
#define _EXPORT
#endif

/////////////////////////////////////////////////////////////////
// Definición de tipos
////////////////////////////////////////////////////////////////

typedef struct 
{
	char NroBit;
	char NombreBit[20];
	char Descripcion[70];
} Caracteristica_t, * pCaracteristica_t;	 
//typedef struct 
//{
//	char NroBit;
//	char * NombreBit;
//	char * Descripcion;
//} Caracteristica_t;	

// Definiciones
#define LEE_CARACTERISTICA_FABRICANTE_NO_SOPORTADO ((char *) -2)
#define LEE_CARACTERISTICA_BIT_CARACTERISTICA_RESERVADO  ((char *) -1)
#define LEE_CARACTERISTICA_CPU_SIN_CARACTERISTICA  ((char *) 0)


// Declaración de variables Weak
_EXPORT Caracteristica_t * TablaCaracteristicasECXIntel;
_EXPORT Caracteristica_t * TablaCaracteristicasEDXIntel;
_EXPORT Caracteristica_t * TablaCaracteristicasECXAMD;
_EXPORT Caracteristica_t * TablaCaracteristicasEDXAMD;


//Prototipos de funciones:
_EXPORT void ImprimeTablasCaracteristicas();
_EXPORT int LeeIDFabricante (char *);
_EXPORT char * LeeCaracteristica (int NumeroBit);
_EXPORT int  LeeIDModelo (char * CadModelo);
_EXPORT void EscribeTablas(void);