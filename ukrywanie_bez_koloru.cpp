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
long long int liczba_na_binarna(int liczba) {

	int kod_ASCII = liczba;
	int zapis_binarny[15];
	int i = 0;
	long long int wynik = 0;
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
void konwertowanie(string zd, Mat img) {
	int dlugosc_zdania = zd.length();
	int ilosc_znakow_do_zaszyfrowania = dlugosc_zdania*8;
	int** zdanie_binarnie = new int*[dlugosc_zdania*8];
	int* zdanie_do_podstawienia = new int[dlugosc_zdania*8];		// ci¹g zer i jedynek do podstawienia do obrazu
	for (int i = 0; i < dlugosc_zdania; ++i)
		zdanie_binarnie[i] = new int[8];
	int ciag_binarny;												// zapis liczby binarnej jako inta, póŸniej wklejany do tablicy
	char znak;
	int tablica_binarna[8] = { 0,0,0,0,0,0,0,0 };

	for (int i = 0; i < dlugosc_zdania; i++) {						// ca³e zdanie jest konwertowane literka po literce na binarny zapis
		znak = zd[i];
		ciag_binarny = znak_na_binarna(znak);
		for (int j = 7; j >= 0; j--) {
			zdanie_binarnie[i][j] = ciag_binarny % 10;
			ciag_binarny /= 10;
		}
	}
	for (int i = 0; i < dlugosc_zdania; i++) {
		for (int j = 0; j < 8; j++) {
			zdanie_do_podstawienia[i * 8 + j] = zdanie_binarnie[i][j];
		}
	}

	//////////////////////////////////
	//	MIEJSCE NA PODMIANÊ BITÓW	//
	//////////////////////////////////
	Mat img_do_zapisu = img.clone();							// kopia obrazu - na niej wszystkie operacje
	Scalar intensity;											// zmienna posiadaj¹ce pole .val z wartoœciami rgb
	uchar newval = 0;
	int licznik = 0;
	int pomocnicza_do_ustawienia_poczatku_wiadomosci;
	long long int pom = liczba_na_binarna(dlugosc_zdania);
	int tab_z_dlugoscia_zdania[15];
	for (int i = 14; i >= 0; i--) {
		tab_z_dlugoscia_zdania[i] = pom % 10;
		pom /= 10;
	}
	int pom1 = 0, pom2 = 0;
	bool wyjscie = false;
	for (int j = 0; j < img.cols; j++) {
		if (j < 15) {
			intensity = img.at<uchar>(0, j);
			uchar grey = intensity.val[0];
			newval = podmiana(tab_z_dlugoscia_zdania[j], grey);
			img_do_zapisu.at<uchar>(0, j) = newval;
		}
		else { break; }
	}
	for (int i = 0; i < img.rows; i++) {
		if (i == 0) {
			pomocnicza_do_ustawienia_poczatku_wiadomosci = 15;
		}
		else { 
			pomocnicza_do_ustawienia_poczatku_wiadomosci=0;
		}
		for (int j = pomocnicza_do_ustawienia_poczatku_wiadomosci; j < img.cols; j++) {

			intensity = img.at<uchar>(i, j);				
			uchar grey = intensity.val[0];

			if (zdanie_do_podstawienia[licznik] != 0 ) {
				if (zdanie_do_podstawienia[licznik] != 1) {
					wyjscie = true;
					break;
				}
			}

			newval = podmiana(zdanie_do_podstawienia[licznik], grey);
			licznik++;
			if (licznik >= dlugosc_zdania*8+1) {
				wyjscie = true;
				break;
			}
			img_do_zapisu.at<uchar>(i, j) = newval;


		}
		if (wyjscie == true)
			break;
	}
	imwrite("zaszyfrowany.bmp", img_do_zapisu);

	delete(zdanie_binarnie);
}
int main() {
	srand(time(NULL));
	Mat img = imread("image.bmp");	

	long long int maksymalna_ilosc_znakow = (((img.cols * img.rows)-15))/8;
	int max = 32768;														//maksymalna d³ugoœæ zapisu 
	if (!img.data)															//Sprawdzenie poprawnosci odczytu 
	{
		cout << "Nie znaleziono pliku .\n";
		system("pause");
		return 0;

	}
	if (img.rows < 512 && img.cols < 512) {
		cout << "Obrazek jest za maly. WYmagany wymiar conajmniej 512x512";
		system("pause");
		return 0;
	}
	Mat im_gray;
	
	cvtColor(img, im_gray, CV_RGB2GRAY);
	if(max> maksymalna_ilosc_znakow)
		cout << "Podaj zdanie do zaszyfrowania. Maksymalnie: "<< max<< " znakow"<<endl;
	else
		cout << "Podaj zdanie do zaszyfrowania. Maksymalnie: " << maksymalna_ilosc_znakow << " znakow" << endl;
	string zdanie;
	getline(cin, zdanie);

 	konwertowanie(zdanie, im_gray);


	system("pause");
	return 0;
}
