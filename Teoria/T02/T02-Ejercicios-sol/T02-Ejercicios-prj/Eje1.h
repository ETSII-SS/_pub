#pragma once
// Prototipo de la función 
void CalculaUVUS(int NroElementos, InfoAlumnos_t tabla[]);

typedef struct
{
	char Nombre[55];
	char Apellido1[55];
	char Apellido2[55];
	char UVUS[55];
	double NMedia;
	int Matriculadas;
	int Superadas;
	char EnCurso;
} InfoAlumnos_t, *pInforAlumno_t;

