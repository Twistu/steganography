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
uchar podmiana(int x, uchar z) {			//  x- ostatni bit do zamiany; z - znak w którym ma byæ zamiana 
	int znak = uchar_na_binarke(z);
	znak /= 10;
	znak *= 10;
	znak += x;
	int wynik_int = binarnie_na_liczbe(znak);
	uchar wynik_uchar = (uchar)wynik_int;
	return wynik_uchar;
} 
void konwertowanie() {

	//////////////////////////////////
	//	MIEJSCE NA PODMIANÊ BITÓW	//
	//////////////////////////////////
	Mat img = imread("zaszyfrowany.bmp");						//(domyœlnie 3 kana³y: BGR)
	Vec3b intensity;											// zmienna posiadaj¹ce pole .val z wartoœciami rgb
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
			if (licznik < 15) {
				intensity = img.at<Vec3b>(0, j);

				uchar blue = intensity.val[0];
				pom = uchar_na_binarke(blue);
				tab_z_dlugoscia_zdania[licznik] = pom % 10;
				licznik++;


				uchar green = intensity.val[1];
				pom = uchar_na_binarke(green);
				tab_z_dlugoscia_zdania[licznik] = pom % 10;
				licznik++;

				uchar red = intensity.val[2];
				pom = uchar_na_binarke(red);
				tab_z_dlugoscia_zdania[licznik] = pom % 10;
				licznik++;
				pom = 0;
			}
			else { break; }
		}

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
				pomocnicza2 = 5;
			}
			else {
				pomocnicza2 = 0;
			}
			for (int j = pomocnicza2; j < img.cols; j++) {

					intensity = img.at<Vec3b>(i, j);		
					uchar blue = intensity.val[0];
					pom = uchar_na_binarke(blue);
					if (licznik >= dlugosc_zdania * 8) {
						wyjscie = true;
						break;
					}
					zdanie[licznik] = pom % 10;
					licznik++;

					uchar green = intensity.val[1];
					pom = uchar_na_binarke(green);
					if (licznik >= dlugosc_zdania * 8) {
						wyjscie = true;
						break;
					}
					zdanie[licznik] = pom % 10;
					licznik++;

					uchar red = intensity.val[2];
					pom = uchar_na_binarke(red);
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
	int p1 = sizeof(zdanie);
	cout << "Zaszyfrowana wiadomosc to: \n";
	for (int i = 0; i < dlugosc_zdania*8; i++) {

		pom += zdanie[i];
		if(i % 8 != 7)
			pom *= 10;
		if (i % 8 == 7 && i!=0) {
			literka = binarnie_na_liczbe(pom);
			if (literka != 'b') {
				pom = 0;
			}
			cout << literka;
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