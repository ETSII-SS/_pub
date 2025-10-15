//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Based on example 2 page 18 of OReilly Learning OpenCV.pdf:
// Basada en práctica 1 de SPD.
// Entorno: PATH=$(AAComm)\bin;%PATH% $(LocalDebuggerEnvironment)
// Directorio de trabajo: $(AAComm)\eje 


#define _CRT_SECURE_NO_WARNINGS 
#include <stdio.h>
#include <locale.h>
#include "opencv/cv.h"
#include "opencv/highgui.h"
#include "pLibSS.h"

void play(char *file_name, char * nombre_ventana, int posX, int posY);

int main(int argc, char *argv[]) {

	CheckError(5 != argc, "Este programa debe ejecutarse con 4 argumentos: nombre_archivo_video nombreVentana posX PosY", 0);
	play(argv[1], argv[2], atoi(argv[3]), atoi(argv[4]));
	return 0;
}

void play(char *file_name, char * nombre_ventana, int posx, int posy)
{
	char key = 0;
	double ahora;
	double antes = 0.0, gap = 0.0;
	CvPoint org = { 20, 40 };
	CvScalar color = { 255.0, 255.0, 255.0, 255.0 };
	CvFont font;
	char texto[200];

	cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 1.0, 1.0, 0.0, 2, 8);
	//HANDLE evComenzar = CreateEvent(NULL, true, false, "evComienzaEjecucion");
	//CheckError(NULL == evComenzar, "Error al crear un evento", 0);

	CvCapture* g_capture = NULL;	// Para gestionar la captura de video
	IplImage* newframe, *oldframe; // para gestionar cada frame
	IplImage img;
	// Crea una ventana OpenCV para mostrar el video
	cvNamedWindow(nombre_ventana, CV_WINDOW_AUTOSIZE);
	cvMoveWindow(nombre_ventana, posx, posy);
	g_capture = cvCreateFileCapture(file_name);	// Abre el archivo de video
	if (NULL == g_capture)
		printf("\n\nERROR: No se pudo abrir el archivo de video\n\n");
	else
	{
		newframe = cvQueryFrame(g_capture); // Lee frame del archivo
		while (newframe && (key != 27)) {
			oldframe = newframe;

			ahora = cvGetCaptureProperty(g_capture, CV_CAP_PROP_POS_MSEC);
			snprintf(texto, sizeof(texto), "%3.1f", ahora / 1000.0);
			cvPutText(oldframe, texto, org, &font, color);
			cvShowImage(nombre_ventana, oldframe);	// Enseña la imagen antigua
			newframe = cvQueryFrame(g_capture);		// Carga la actual
			key = cvWaitKey((int)1); // Espera que se pulse una tecla con un timeout de 1 ms
			//BOOL res = WaitForSingleObject(evComenzar, INFINITE);
			//CheckError(WAIT_OBJECT_0 != res, "Error al bloquear en funcion play", 1);


		}  // end of:   while(newframe && (key != 27 ) ) {
		cvReleaseCapture(&g_capture);
	}
	//CloseHandle(evComenzar);
	cvDestroyWindow(nombre_ventana);
}

