//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Based on example 2 page 18 of OReilly Learning OpenCV.pdf:
// Basada en práctica 1 de SPD.

#define _CRT_SECURE_NO_WARNINGS 
#include <stdio.h>
#include <locale.h>
#include "opencv/cv.h"
#include "opencv/highgui.h"

static void play(const char * file_name, const char * nombre_ventana);

// Pegar los nombres de archivos en las opciones de depuración:
// P10Video1.mp4 P10Video2.mp4 P10Video3.mp4 P10Eje1_corto.mp4 P10Eje2_Corto.mp4

int main(int argc, char *argv[]) {
	char nombreVentana[12];
	

	// los archivos .mp4 están en _pub/_comm/eje del repositorio público
	setlocale(LC_ALL, "Spanish");  // hace que printf "entienda" tildes, ñ, etc. 
	printf("\nPulse tecla ESC para cerrar la ventana\n");

	for (int i = 1; i < argc ; i++) {
		sprintf(nombreVentana, "Reproduciendo: %s", argv[i]);
		play(argv[i], nombreVentana);
	}

	printf("\nPulse tecla RETORNO para terminar\n");
	getchar();
	return 0;
}

static void play(const char *file_name, const char * nombre_ventana)
{
	char key = 0;

	// Estructuras definidas en OpenCV
	// OJO: las coordenadas en OpenCV son distintas que en los archivos .BMP. 
	//       El punto 0,0 está en la esquina superior izquierda.
	CvCapture* g_capture = NULL;	// Para gestionar la captura de video
	IplImage* newframe, *oldframe; // para gestionar cada frame

								   // Crea una ventana OpenCV para mostrar el video
	cvNamedWindow(nombre_ventana, CV_WINDOW_AUTOSIZE);
	g_capture = cvCreateFileCapture(file_name);	// Abre el archivo de video
	if (NULL == g_capture)
		printf("\n\nERROR: No se pudo abrir el archivo de video\n\n");
	else
	{
		// Bucle de proceso de frames
		newframe = cvQueryFrame(g_capture); // Lee frame del archivo
		while (newframe && (key != 27)) {
			oldframe = newframe;
			cvShowImage(nombre_ventana, oldframe);	// Enseña la imagen antigua
			newframe = cvQueryFrame(g_capture);		// Carga la actual
													// Lee la posición de tiempo del frame
			key = cvWaitKey(1); // Espera que se pulse una tecla con un timeout de 1 ms
								// Aquí podemos decodificar la tecla
		}  // end of:   while(newframe && (key != 27 ) ) {
		   // Cerrar archivo de video
		cvReleaseCapture(&g_capture);
	}
	cvDestroyWindow(nombre_ventana);
}
