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
	Vec3b intensity;											// zmienna posiadaj¹ce pole .val z wartoœciami rgb
	uchar newval = 0;
	int pomocnicza_do_ustawienia_poczatku_wiadomosci;
	long long int pom = liczba_na_binarna(dlugosc_zdania);
	int tab_z_dlugoscia_zdania[15];
	for (int i = 14; i >= 0; i--) {
		tab_z_dlugoscia_zdania[i] = pom % 10;
		pom /= 10;
	}
	int licznik = 0;
	bool wyjscie = false;

	for (int j = 0; j < img.cols; j++) {
		if (licznik < 15) {
			intensity = img.at<Vec3b>(0, j);				// do sprawdzenia
			uchar blue = intensity.val[0];
			newval = podmiana(tab_z_dlugoscia_zdania[licznik], blue);
			licznik++;
			img_do_zapisu.at<cv::Vec3b>(0, j)[0] = newval;

			uchar green = intensity.val[1];
			newval = podmiana(tab_z_dlugoscia_zdania[licznik], green);
			licznik++;
			img_do_zapisu.at<cv::Vec3b>(0, j)[1] = newval;

			uchar red = intensity.val[2];
			newval = podmiana(tab_z_dlugoscia_zdania[licznik], red);
			licznik++;
			img_do_zapisu.at<cv::Vec3b>(0, j)[2] = newval;
		}
		else { break; }
	}
	licznik = 0;
	for (int i = 0; i < img.rows; i++) {
		if (i == 0) {
			pomocnicza_do_ustawienia_poczatku_wiadomosci = 5;
		}
		else { 
			pomocnicza_do_ustawienia_poczatku_wiadomosci=0;
		}
		for (int j = pomocnicza_do_ustawienia_poczatku_wiadomosci; j < img.cols; j++) {

			intensity = img.at<Vec3b>(i, j);				
			uchar blue = intensity.val[0];
			if (zdanie_do_podstawienia[licznik] != 0 ) {
				if (zdanie_do_podstawienia[licznik] != 1) {
					wyjscie = true;
					break;
				}
			}
			newval = podmiana(zdanie_do_podstawienia[licznik], blue);
			licznik++;
			if (licznik > dlugosc_zdania*8) {
				wyjscie = true;
				break;
			}
			img_do_zapisu.at<cv::Vec3b>(i, j)[0] = newval;

			uchar green = intensity.val[1];
			if (zdanie_do_podstawienia[licznik] != 0) {
				if (zdanie_do_podstawienia[licznik] != 1) {
					wyjscie = true;
					break;
				}
			}
			newval = podmiana(zdanie_do_podstawienia[licznik], green);
			licznik++;
			if (licznik > dlugosc_zdania*8) {
				wyjscie = true;
				break;
			}
			img_do_zapisu.at<cv::Vec3b>(i, j)[1] = newval;

			uchar red = intensity.val[2];
			if (zdanie_do_podstawienia[licznik] != 0) {
				if (zdanie_do_podstawienia[licznik] != 1) {
					wyjscie = true;
					break;
				}
			}
			newval = podmiana(zdanie_do_podstawienia[licznik], red);
			licznik++;
			if (licznik > dlugosc_zdania*8) {
				wyjscie = true;
				break;
			}
			img_do_zapisu.at<cv::Vec3b>(i, j)[2] = newval;
		}
		if (wyjscie == true)
			break;
	}
	imwrite("zaszyfrowany.bmp", img_do_zapisu);
	delete(zdanie_binarnie);
}
char losuj() {	
	int liczba = std::rand() % 26 + 97;
	char wynik = (char)(liczba );
	return wynik;
} // funkcja losuj¹ca literê
int main() {
	//  dlugosc wiadomosci , ilosc znakow max 32767, 15 cyfr w binarnie razem 5 pixeli;
	srand(time(NULL));
	int M[4] = { 0,1,1,0 };
	int nM[4] = { 1,0,0,1 };
	Mat img = imread("image.bmp");								//(domyœlnie 3 kana³y: BGR)

	long long int maksymalna_ilosc_znakow = (((img.cols * img.rows)-5)*3)/8;
	int max = 32768;												//maksymalna d³ugoœæ zapisu wynikaj¹ca z 9 bitów zapisu binarnego
	if (!img.data)												//Sprawdzenie poprawnosci odczytu 
	{
		cout << "Nie znaleziono pliku .\n";
		system("pause");
		return 0;
	}
	if(max > maksymalna_ilosc_znakow)
		cout << "Podaj zdanie do zaszyfrowania. Maksymalnie: "<< max<< " znakow"<<endl;
	else
		cout << "Podaj zdanie do zaszyfrowania. Maksymalnie: " << maksymalna_ilosc_znakow << " znakow" << endl;
	string zdanie;
	long long int granica = 250 *19;

	getline(cin, zdanie);
	konwertowanie(zdanie, img);

	system("pause");
	return 0;
}
