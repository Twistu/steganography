#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include "opencv2\opencv.hpp"


using namespace std;
using namespace cv;



int binarnie_na_liczbe(int x) {
	int wynik = 0;
	int tab[8];
	for (int i = 0; i < 8; i++) {
		tab[i] = x % 10;
		x /= 10;
	}
	for (int i = 0; i < 8; i++) {
		wynik += tab[i] * pow(2, i);
	}
	return wynik;
}
int uchar_na_binarke(uchar znak) {
	int kod_ASCII = (int)znak;
	int zapis_binarny[8], i = 0, wynik = 0;
	while (kod_ASCII) {
		zapis_binarny[i++] = kod_ASCII % 2;
		kod_ASCII /= 2;
	}
	for (int j = i - 1; j >= 0; j--) {
		wynik += zapis_binarny[j];
		if (j>0)
			wynik *= 10;
	}
	return wynik;
}
uchar podmiana(uchar z) {			//  x- ostatni bit do zamiany; z - znak w którym ma byæ zamiana 
	int znak = uchar_na_binarke(z);
	int wynik_int, pom = znak % 10;
	if (pom == 1)
		znak = 255;
	else
		znak = 0;
	uchar wynik_uchar = (uchar)znak;
	return wynik_uchar;
} 
void konwertowanie(Mat img) {
	Scalar intensity;		
	Mat img_do_zapisu = img.clone();

	uchar newval;
	uchar ij = (uchar)255;
	for (int i = 0; i < img.rows; i++) {
		for (int j = 0; j < img.cols; j++) {

			intensity = img.at<uchar>(i, j);
			uchar gray1 = intensity.val[0];
			newval = podmiana(gray1);
			img_do_zapisu.at<uchar>(i, j) = newval;

		}
	}
	
	imwrite("atak.bmp", img_do_zapisu);
}
char losuj() {
	int liczba = std::rand() % 26 + 97;
	char wynik = (char)(liczba );
	return wynik;
}
int main() {
	srand(time(NULL));
	Mat img = imread("image.bmp", CV_LOAD_IMAGE_GRAYSCALE);
	if (!img.data)												
	{
		cout << "Nie znaleziono pliku .\n";
		system("pause");
		return 0;
	}

	konwertowanie(img);

	system("pause");
	return 0;
}
