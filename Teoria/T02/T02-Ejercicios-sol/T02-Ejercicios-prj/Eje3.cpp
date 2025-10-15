#include <stdio.h>
#include <conio.h>
#include <locale.h>
#include <iostream>
static void EjemploPunteros();

using namespace std;
int main(int argc, char *argv[], char *envp[])
{
	setlocale(LC_ALL, "Spanish");
	
	EjemploPunteros();

	//// Pendiente: Modificar para imprimir argumentos del programa 
	//for (int i = 0; envp[i] != 0; i++)
	//{
	//	cout << envp[i] << endl;
	//}

	printf("\nPor favor, pulse una tecla para terminar ...");
	_getch();
	return 0;
}

// Ejemplo: declaración y uso de punteros a int y punteros a cadenas (char *)
// Ver captura de memoria en https://uses0-my.sharepoint.com/personal/camaya_us_es/_layouts/15/onedrive.aspx?ga=1&id=%2Fpersonal%2Fcamaya_us_es%2FDocuments%2FEscuela%2F_SS%2FSS-PUB%2FMaterial%2Fimgs%2FcapturaMem21%2Epng&parent=%2Fpersonal%2Fcamaya_us_es%2FDocuments%2FEscuela%2F_SS%2FSS-PUB%2FMaterial%2Fimgs

#include <stdio.h>

static void EjemploPunteros(void)
{
    int numero = 42;
    int* pNum = &numero;     // puntero a entero

    // Punteros a cadenas: cada puntero apunta al primer carácter de una cadena literal
    const char* mensaje1 = "Hola mundo";
    const char* mensaje2 = "Software de Sistemas";

    const char* pCadena = mensaje1; // puntero a cadena (char *)

    printf("=== EjemploPunteros() ===\n\n");

    // --- Parte 1: Puntero a int ---
    printf("[Puntero a int]\n");
    printf("Direccion de numero:        %p\n", (void*)&numero);
    printf("Direccion almacenada en pNum: %p\n", (void*)pNum);
    printf("Valor de numero: %d (acceso directo)\n", numero);
    printf("Valor usando *pNum: %d (acceso indirecto)\n", *pNum);

    // Modificar a través del puntero
    *pNum += 10;
    printf("Tras modificar mediante *pNum: numero = %d\n\n", numero);

    // --- Parte 2: Puntero a cadena ---
    printf("[Puntero a cadena]\n");
    printf("mensaje1: \"%s\" (direccion=%p)\n", mensaje1, (void*)mensaje1);
    printf("mensaje2: \"%s\" (direccion=%p)\n", mensaje2, (void*)mensaje2);
    printf("pCadena apunta a: \"%s\" (direccion=%p)\n\n", pCadena, (void*)pCadena);

    // Cambiamos el puntero para que apunte a otra cadena
    pCadena = mensaje2;
    printf("Ahora pCadena apunta a: \"%s\" (direccion=%p)\n\n", pCadena, (void*)pCadena);

    // Recorrer la cadena carácter a carácter usando aritmética de punteros
    printf("Recorriendo la cadena con puntero:\n");
    const char* p = pCadena;
    while (*p != '\0') { // mientras no llegue al fin de cadena
        printf("Caracter '%c' en direccion %p\n", *p, (void*)p);
        p++; // avanzar al siguiente carácter
    }
    printf("\n");

    // --- Parte 3: Matriz de punteros a cadena ---
    const char* mensajes[] = { "uno", "dos", "tres" };
    printf("[Matriz de punteros a cadena]\n");
    for (int i = 0; i < 3; i++) {
        printf("mensajes[%d] -> \"%s\" (puntero=%p)\n",
            i, mensajes[i], (void*)mensajes[i]);
    }
    printf("\n");
}









