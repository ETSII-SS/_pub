#include <stdio.h>

// Código original en tema 3
extern char Saludo[];
char* PasaAMayusculas(char*);

//char* PasaAMayusculas(char*, int);
int main(void)
{
	printf(PasaAMayusculas(Saludo));
	//PasaAMayusculas(Saludo, 1);
}





////Macro que permite usar un símbolo externo C 
//// desde un archivo .c o .cpp
//#if __cplusplus 
//#define EXTERN_C extern "C"
//#else
//#define EXTERN_C extern
//#endif
//
//EXTERN_C char Saludo[];
//EXTERN_C char * PasaAMayusculas(char *);
//
////char* PasaAMayusculas(char*, int);
//
//int main(void)
//{
//	printf(PasaAMayusculas(Saludo));
//	//PasaAMayusculas(Saludo, 1);
//}



