#define _CRT_SECURE_NO_WARNINGS
#include "pLibSS.h"
#include <thread>
#define NRO_CONEXIONES_SIMULTANEAS 1
// Normalmente NRO_CONEXIONES_SIMULTANEAS y NRO_HILOS_SIRVIENDO son iguales? 
#define NRO_HILOS_SIRVIENDO 1

#define TAM_BUFFER_ENTRADA 1000

void GestionaTuberia(char * pNombreTuberia)
{
	char BEntrada[TAM_BUFFER_ENTRADA];	// Reserva para buffer de entrada
	DWORD Leidos, Escritos;
	BOOL ResOperacion;
	HANDLE hPipe;	// Handle para una instancia del pipe. 

	hPipe = CreateNamedPipe(pNombreTuberia,	//Nomnbre
		PIPE_ACCESS_DUPLEX,				// Modo apertura
		PIPE_TYPE_MESSAGE | PIPE_WAIT,	// Cómo funciona
		NRO_CONEXIONES_SIMULTANEAS,								// Nro. máx. de conexiones
		0,		// Tamańo de buffers salida. 0= tam. por defecto
		0,		// Idem entrada.
		5000,   // Timeout en operacioes ReadFile, WriteFile
		0);		// lpSecurityAttributes.
	CheckError(INVALID_HANDLE_VALUE == hPipe, "Error creando tubería", 1);        //HANDLE hEvento = CreateEvent(NULL,,, "evTuberiaCreada");  CheckError(hEvento == NULL, "No se pudo crear el objeto de sincronización", 1);// Activarlo y listo
	
	// Espera conexión
	printf("Servidor: Esperando conexión entrante...\n");
	CheckError(FALSE == ConnectNamedPipe(hPipe, NULL), "Error esperando conexion tubería", 1);
	printf("Conectada tubería...\n");
	Beep(500, 200); // Aviso de que comienza a leer.
	//Sleep(5000);
	while (TRUE)
	{
		ResOperacion = ReadFile(hPipe, BEntrada, sizeof(BEntrada), &Leidos, 0);
		//Procesa condición cliente cierra tubería
		//if (FALSE == ResOperacion && ERROR_BROKEN_PIPE == GetLastError())
		//		break;
		CheckError(FALSE == ResOperacion, "Error leyendo tubería", 1);
		// Procesamos entrada
		// Forzamos \0 en la cadena de entrada
		BEntrada[Leidos] = '\0';
		printf("%s", BEntrada);

		Beep(1000, 200); // Aviso...
		// Si enviamos respuesta al cliente:
		//ResOperacion = WriteFile(hPipe, BEntrada, Leidos, &Escritos, 0);
		//CheckError(FALSE == ResOperacion || Leidos != Escritos,
		//	"Error escribiendo tubería", 1);
	}
	// Desconecta tubería
	CheckError(FALSE == DisconnectNamedPipe(hPipe), "Error desconectando conexion tubería", -1);
	CloseHandle(hPipe); // Cierra instancia de la tubería (y la tubería)
}


// Función de entrada del proyecto 
int main(int argc, char *argv[])
{
	int i;
	char * pNombreTuberia = "\\\\.\\pipe\\Logger";
	
	setlocale(LC_ALL, "Spanish");	

	GestionaTuberia(pNombreTuberia);

	// Usado hilos (cuando se necesita más de 1 conexión simultanea)
	//std::thread* th[NRO_HILOS_SIRVIENDO];
	//Crear NRO_HILOS_SIRVIENDO hilos 
	//for (i = 0; i< NRO_HILOS_SIRVIENDO; i++)
	//{
	//	th[i] = new std::thread(GestionaTuberia, pNombreTuberia);
	//	CheckError(NULL == th[i]->native_handle(), "ERROR al crear hilo", 1);
	//}

	//for (i = 0; i< NRO_HILOS_SIRVIENDO; i++)
	//{
	//	CheckError(WAIT_OBJECT_0 != WaitForSingleObject(th[i]->native_handle(), INFINITE), 
	//		"ERROR al esperar que el hilo termine", 1);
	//}

	printf("\nPulse tecla RETORNO para terminar\n");
	getchar();
	return 0;
}

