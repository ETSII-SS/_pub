#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <locale.h>
#include <Windows.h>
#include <crtdbg.h>

/// IMPORTANTE: Compilar este programa en x64
const int	MaxX = 1024, // Cada fila ocupa una página de 4 K (1024*4)
			MaxY = 4*1024, // Cada imagen son 16 Mbytes 
			FramesXSec= 25, // Cada segundo son 400 MBytes
			Duracion=1*10; // 2,4 GBytes cada 10 segundos

typedef RGBQUAD frame_t[MaxY][MaxX];
//frame_t secuencia[FramesXSec*Duracion];  // problema: el compilador solo admite matrices
										 // estáticas de menos de 2 G

const SIZE_T tamSecuencia = sizeof(frame_t)*FramesXSec*Duracion;
const SIZE_T tamSegundo = sizeof(frame_t)*FramesXSec;

int main(int argc, char *argv[])
{
	frame_t * secuencia; 

	setlocale(LC_ALL, "Spanish"); // Previo

	// Reserva  memoria para la secuencia
	secuencia = (frame_t *) VirtualAlloc(NULL, tamSecuencia, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	if (secuencia != NULL) {
		for (int seg = 0; seg < Duracion; seg++) // Proceso cada segundo
		{
			//...
			for (int nroFrame = 0; nroFrame < FramesXSec; nroFrame++) // Proceso cada frame del segundo
			{
				//...
				for (int filaImagen = 0; filaImagen < MaxY; filaImagen++) // Proceso cada fila de la imagen
				{
					//...
					// Escribe primer pixel de la fila. Provoca fallo de página.
					secuencia[seg*FramesXSec + nroFrame][filaImagen][0].rgbRed = 0xff;
				}

			}
			//VirtualUnlock(&secuencia[seg*FramesXSec][0][0], tamSegundo); // saca de memoria física el segundo actual
		}
		VirtualFree(secuencia, 0, MEM_RELEASE);
	}
	else
		printf("\nERROR: No se pudo reservar la memoria\n\n");  
    printf("\nPulse una tecla para terminar\n");  //Solo para depurar. Quitar si no es necesario
    getchar();
    return 0;
}