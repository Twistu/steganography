#include <iostream>
#include <string>
#include "opencv2\opencv.hpp"


using namespace std;
using namespace cv;



int binarnie_na_liczbe(long long int x) {
	int wynik = 0;
	int tab[15];
	for (int i = 0; i < 15; i++) {
		tab[i] = x % 10;
		x /= 10;
	}
	for (int i = 0; i < 15; i++) {
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
int znak_na_binarna(char znak) {

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

void konwertowanie() {

	//////////////////////////////////
	//	MIEJSCE NA PODMIANÊ BITÓW	//
	//////////////////////////////////
	Mat img = imread("zaszyfrowany.bmp", CV_LOAD_IMAGE_GRAYSCALE);						//(domyœlnie 3 kana³y: BGR)
	Scalar intensity;											// zmienna posiadaj¹ce pole .val z wartoœciami rgb
	int tab_z_dlugoscia_zdania[15];
	long long int licznik = 0;
	int pomocnicza2, dlugosc_zdania;
	long long int pom;
	bool wyjscie = false;
	int *zdanie = new int[10];
	if (!img.data)												//Sprawdzenie poprawnosci odczytu 
	{
		cout << "Nie znaleziono pliku .\n";
	}
	else {
		for (int j = 0; j < img.cols; j++) {
			if (j < 15) {
				intensity = img.at<uchar>(0, j);			

				uchar grey = intensity.val[0];
				pom = uchar_na_binarke(grey);
				tab_z_dlugoscia_zdania[j] = pom % 10;
			}
			else { break; }
		}
		pom = 0;
		for (int k = 0; k <15; k++) {
			pom += tab_z_dlugoscia_zdania[k];
			if(k<14)
				pom *= 10;
		}
		dlugosc_zdania = binarnie_na_liczbe(pom);
		zdanie = new int[dlugosc_zdania * 8];
		licznik = 0;
		int pom123 = dlugosc_zdania * 8;
		for (int i = 0; i < img.rows; i++) {
			if (i == 0) {
				pomocnicza2 = 15;
			}
			else {
				pomocnicza2 = 0;
			}
			for (int j = pomocnicza2; j < img.cols; j++) {
					
				if (i * 200 + j == 39592) {
					pom = 0;
				}
					intensity = img.at<uchar>(i, j);
					uchar grey = intensity.val[0];
					pom = uchar_na_binarke(grey);
					if (licznik >= dlugosc_zdania * 8) {
						wyjscie = true;
						break;
					}
					zdanie[licznik] = pom % 10;
					licznik++;
			}
			if (wyjscie == true)
				break;
		}
	}
	pom = 0;
	char literka;
	int counter=0;
	int p1 = sizeof(zdanie);
	cout << "Zaszyfrowana wiadomosc to: \n";
	for (int i = 0; i < dlugosc_zdania*8; i++) {

		pom += zdanie[i];
		if(i % 8 != 7)
			pom *= 10;
		if (i % 8 == 7 && i!=0) {
			literka = binarnie_na_liczbe(pom);
			cout << literka;
			counter++;
			pom = 0;
		}
	}
	cout << endl;
	delete(zdanie);
}

int main() {
	konwertowanie();

	system("pause");
	return 0;
}