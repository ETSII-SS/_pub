#define _CRT_SECURE_NO_WARNINGS
#include "pLibSS.h"
#define TIEMPO_ESPERA_MAXIMO_MS (10*1000)

void UsaTuberia(char* NombreP)
{
	HANDLE hPipeC;	// Handle para una instancia del pipe (cliente). 
	//char BEntrada[1000];	// Reserva para buffer de entrada

	char BSalida[1000];		// Reserva para buffer de 
	DWORD Leidos, Escritos, i;
	BOOL ResOperacion;

	// abre tubería...
	hPipeC = CreateFile(NombreP, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	CheckError(hPipeC == INVALID_HANDLE_VALUE, "Error conectando a tubería", 1);

	printf(" ............Cliente conectado\n");

	for (i = 0; i < 3; i++)
	{
		sprintf(BSalida, "Mensaje numero %d\n", i);

		ResOperacion = WriteFile(hPipeC, BSalida, strlen(BSalida) + 1, &Escritos, 0); // Escribe datos
		// Comprueba escritura correcta
		CheckError(FALSE == ResOperacion || Escritos != strlen(BSalida) + 1, "Error escribiendo tubería", 1);

		printf("Enviado mensaje %d\n", i);
	}

	CloseHandle(hPipeC);	// Cierra tubería

	printf("\nCliente desconectado...\n");

}
// Función de entrada del proyecto "T06_20_PipeClient1"
int main(int argc, char* argv[])
{
	int msegs = TIEMPO_ESPERA_MAXIMO_MS;
	setlocale(LC_ALL, "Spanish");	// Indica que debe usar español como idioma por defecto

	printf("\nCliente conectará en %d segs...", msegs / 1000);
	Sleep(TIEMPO_ESPERA_MAXIMO_MS);  // Para dar tiempo a que el servidor arranque  
	//EsperaTuberiaCreada();// Alternativa?

	UsaTuberia("\\\\.\\pipe\\Logger");
	//UsaTuberia("prueba.txt");

	printf("\nPulse tecla RETORNO para terminar\n");
	getchar();
	return 0;
}
//void EsperaTuberiaCreada() {
//
//	HANDLE hEmpieza;
//	DWORD res = 0xff;
//
//	hEmpieza = CreateEventA(NULL, , , "evTuberiaCreada");
//	CheckError(NULL == hEmpieza, "No se pudo crear objeto de sincronización", 1);
//	res = WaitForSingleObject(hEmpieza, 1000);
//	if (res != WAIT_OBJECT_0) {
//		printf("\nNo se ha creado la tubería...\n");
//		ExitProcess(1);
//	}
//}
