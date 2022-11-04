
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <cmath>
#include <iomanip>
#define PI 3.141592653589

using namespace cv;
using namespace std;


Mat obtenerImagen(char NombreImagen[]) {

	Mat imagen; // Matriz que contiene nuestra imagen sin importar el formato


	/*********Lectura de la imagen*********/
	imagen = imread(NombreImagen);

	if (!imagen.data)
	{
		cout << "Error al cargar la imagen: " << NombreImagen << endl;
		exit(1);
	}
	return imagen;
}
void mostrarImagen(Mat imagen) {
	/************Procesos*********/
	int fila_original = imagen.rows;
	int columna_original = imagen.cols;

	cout << "filas: " << fila_original << endl;
	cout << "columnas: " << columna_original << endl;

	namedWindow("Lena Moderna", WINDOW_AUTOSIZE);//Creacion de una ventana
	imshow("Lena Moderna", imagen);

	/************************/

	waitKey(0); //Funcion para esperar

}

double** generacionKernel(int sigma, int kernelS) {
	//Mat matriz; 
	Mat matriz(kernelS, kernelS, CV_8UC1);
	int centro = (kernelS - 1) / 2;

	double** matrizprueba = new double* [kernelS];


	for (int i = 0; i < kernelS; i++) {
		matrizprueba[i] = new double[kernelS];
		for (int j = 0; j < kernelS; j++) {
			int posx = i - centro;
			int posy = (j - centro) * -1;

			double valor = 1 / (2 * PI * sigma * sigma);
			valor = valor * exp(-(pow(posx, 2) + pow(posy, 2)) / 2 * pow(sigma, 2));
			matrizprueba[i][j] = valor;

			cout << valor << "\t";

		}
		cout << "\n";
	}

	return matrizprueba;
}

void procesarMatriz(Mat imagen, int kernel, int sigma) {
	int rows = imagen.rows;
	int cols = imagen.cols;
	int exceso = (kernel - 1) / 2;

	Mat grises(rows + exceso, cols + exceso, CV_8UC1);
	Mat grande(rows + exceso, cols + exceso, CV_8UC1);

	cvtColor(imagen, grises, COLOR_BGR2GRAY);

	double rojo, azul, verde, gris_p;

	for (int i = 0; i < rows + exceso; i++) {
		for (int j = 0; j < cols + exceso; j++) {

			if (i >= rows || i < exceso) { // >=
				grande.at<uchar>(Point(j, i)) = uchar(0);
				//cout << "entra\n";


			}
			else if (j >= cols || j < exceso) { //nadamas le cambie por >=, ya que toma en cuenta el 0
				grande.at<uchar>(Point(j, i)) = uchar(0);
				//cout << "entra\n";
			}
			else {
				azul = imagen.at<Vec3b>(Point(j - exceso, i - exceso)).val[0];
				verde = imagen.at<Vec3b>(Point(j - exceso, i - exceso)).val[1];
				rojo = imagen.at<Vec3b>(Point(j - exceso, i - exceso)).val[2];

				gris_p = (azul + verde + rojo) / 3;

				grande.at<uchar>(Point(j, i)) = uchar(gris_p);
			}
			

		}
	}
	mostrarImagen(grande);



}

int main() {

	char NombreImagen[] = "lenaModerna.png";
	Mat imagen = obtenerImagen(NombreImagen);

	int sigma;

	cout << "Elige un valor de sigma: ";
	cin >> sigma;

	int kernel;
	cout << "elige un tamaño para kernel (debe ser impar): ";

	cin >> kernel;
	while (kernel < 1 || kernel % 2 == 0) {
		cout << "Error! Elige in kernel de tama�o impar ";

		cin >> kernel;
	}


	double** matriz_kernel = generacionKernel(sigma, kernel);

	procesarMatriz(imagen, kernel, sigma);


	return 1;
}

