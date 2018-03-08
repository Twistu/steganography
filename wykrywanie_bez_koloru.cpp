#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <cmath>
#include "opencv2\opencv.hpp"


using namespace std;
using namespace cv;

int  R=0;
int  S=0;

int Rm=0;
int Sm=0;
// vectory do zapisu po zmianie LSB
int  R2 = 0;
int  S2 = 0;

int Rm2 = 0;
int Sm2 = 0;

double funkcja_x(float d0, float d1, float d_0, float d_1) {
	double delta= (d_0 - d1 - d_1- 3 * d0)*(d_0 - d1 - d_1- 3 * d0) - 4 * (2 * (d1 + d0)*(d0 - d_0));
	double x1 = ((-d_0 + d1 + d_1 + 3 * d0) + sqrt(delta)) / (2 * (2 * (d1 + d0)));
	double x2 = ((-d_0 + d1 + d_1 + 3 * d0) - sqrt(delta)) / (2 * (2 * (d1 + d0)));
	double abs_x1 = abs(x1);
	double abs_x2 = abs(x2);
	if (abs_x1 < abs_x2)
		return x1;
	else
		return x2;
}
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
}//dzia³a
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
}//dzia³a
int zamiana_LSB(int x) {
	int wynik = x / 10;
	int pom = x % 10;
	if (pom == 1)
		wynik *= 10;
	else {
		wynik *= 10;
		wynik++;
	}
	return wynik;
}
void zastosowanie_maski(int M[4],int tab[4],int tab2[4]) {
	int pom = 0;
	bool wyjscie = false;
	for (int i = 0; i < 4; i++) {
		if (M[i] == 1) {
			if (tab[i] % 2 == 0)
				tab2[i] = tab[i] + 1;
			else
				tab2[i] = tab[i] - 1;
		}
		else if (M[i] == -1) {
			if (tab[i] % 2 == 0)
				tab2[i] = tab[i] - 1;
			else
				tab2[i] = tab[i] + 1;
		}
		else if (M[i] == 0) {
			tab2[i] = tab[i];
		}
	}
}//dzia³a
void zamiana_bitu(Mat img) {
	Scalar intensity;											// zmienna posiadaj¹ce pole .val z wartoœciami rgb
	int pom1=0,pom2=0;
	for (int i = 0; i < img.rows; i++) {
		for (int j = 0; j < img.cols; j++) {

			intensity = img.at<uchar>(i, j);

			uchar blue = intensity.val[0];
			pom1 = uchar_na_binarke(blue);
			pom2 = zamiana_LSB(pom1);
			pom1 = binarnie_na_liczbe(pom2);
			img.at<uchar>(i, j) = (uchar)pom1;

		}
	}
}
int funkcja_glowna(Mat img, int parametr) {
	bool wyjscie = false;
	int u = 0;
	int ilosc_iteracji = 0;
	//int M[4] = { 1,0,0,1 };
	//int nM[4] = { -1,0,0,-1 };

	int M[4] = { 1, 1, 1, -1 };
	int nM[4] = { -1,-1,-1,1 };

	//int M[4] = { 0, 1, 1, 0 };							// maska
	//int nM[4] = { 0, -1, -1, 0 };						// negacja maski
	Scalar intensity;
	int tab[4], tab2[4],tab3[4], licznik=0, pom=0;		// tab - tabela z kanalami 4 pikselow / tab2 - tabela z kanalami 4 pikselow po zastosowaniu maski M/ tab3 - tabela z kanalami 4 pikselow po zastosowaniu maski nM

	for (int i = 0; i < img.rows; i++) {

		for (int j = 0; j < img.cols-3; j+=4) {
			intensity = img.at<uchar>(i, j);				

			uchar blue = intensity.val[0];
			tab[0] = (int)blue;

			intensity = img.at<uchar>(i, j+1);

			blue = intensity.val[0];
			tab[1] = (int)blue;


			intensity = img.at<uchar>(i, j+2);

			blue = intensity.val[0];
			tab[2] = (int)blue;

			intensity = img.at<uchar>(i, j+3);

			blue = intensity.val[0];
			tab[3] = (int)blue;


			//obliczanie funkcji
			int wynik_funkcji=0;
			for (int k = 0; k < 3; k++)
				wynik_funkcji += abs(tab[k + 1] - tab[k]);
			// koniec

			// zastosowanie maski M i -M
			zastosowanie_maski(M, tab, tab2);
			zastosowanie_maski(nM, tab, tab3);
			// koniec maski

			//obliczanie funkcji po zastosowaniu maski
			int wynik_funkcji_po_masce_M = 0, wynik_funkcji_po_masce_nM = 0;
			for (int k = 0; k < 3; k++) {
				wynik_funkcji_po_masce_M += abs(tab2[k + 1] - tab2[k]);
				wynik_funkcji_po_masce_nM += abs(tab3[k + 1] - tab3[k]);
			}
			// koniec
			//Wyznaczanie Rm i Sm dla maski M i nM(negacji maski)
			// Dla m
				if (wynik_funkcji > wynik_funkcji_po_masce_M) {
					if(parametr==1)
						S++;
					else
						S2++;
				}
				else if (wynik_funkcji < wynik_funkcji_po_masce_M) {
					if(parametr==1)
						R++;
					else
						R2++;
				}
			// Dla nM
				if (wynik_funkcji > wynik_funkcji_po_masce_nM) {
					if(parametr==1)
						Sm++;
					else
						Sm2++;
				}
				else if (wynik_funkcji < wynik_funkcji_po_masce_nM) {
					if(parametr==1)
						Rm++;
					else
						Rm2++;
				}
				else
					u++;
			// koniec

			licznik++;
			ilosc_iteracji++;
		}
		if (wyjscie == true)
			break;
	}
	return ilosc_iteracji;
}

int main() {
	int ilosc_iteracji;
	double tab[20][9];
	float wynik;
	string pomoc;
	for (int k = 1; k <= 20; k++) {
		pomoc = to_string(k);
		Mat img = imread("zaszyfrowany"+pomoc +".bmp");						//(domyœlnie 3 kana³y: BGR)
		Mat img_do_zamiany_LSB = img.clone();						// kopia obrazu - do zamiany LSB
		if (!img.data)												//Sprawdzenie poprawnosci odczytu 
		{
			cout << "Nie znaleziono pliku .\n";
			system("pause");
			return 0;
		}
		zamiana_bitu(img_do_zamiany_LSB);
		ilosc_iteracji = funkcja_glowna(img, 1);
		cout << "R: " << R << endl;
		cout << "S: " << S << endl;
		cout << "Rm: " << Rm << endl;
		cout << "Sm: " << Sm << endl;
		float wynik_procentowy_R = ((float)R * 100.0) / (float)ilosc_iteracji;
		float wynik_procentowy_S = ((float)S * 100.0) / (float)ilosc_iteracji;
		float wynik_procentowy_Rm = ((float)Rm * 100.0) / (float)ilosc_iteracji;
		float wynik_procentowy_Sm = ((float)Sm * 100.0) / (float)ilosc_iteracji;
		float d0 = wynik_procentowy_R - wynik_procentowy_S;
		float d_0 = wynik_procentowy_Rm - wynik_procentowy_Sm;
		//po zmianie LSB
		ilosc_iteracji = funkcja_glowna(img_do_zamiany_LSB, 2);
		cout << "R2: " << R2 << endl;
		cout << "S2: " << S2 << endl;
		cout << "Rm2: " << Rm2 << endl;
		cout << "Sm2: " << Sm2 << endl;
		wynik_procentowy_R = ((float)R2 * 100.0) / (float)ilosc_iteracji;
		wynik_procentowy_S = ((float)S2 * 100.0) / (float)ilosc_iteracji;
		wynik_procentowy_Rm = ((float)Rm2 * 100.0) / (float)ilosc_iteracji;
		wynik_procentowy_Sm = ((float)Sm2 * 100.0) / (float)ilosc_iteracji;
		float d1 = wynik_procentowy_R - wynik_procentowy_S;
		float d_1 = wynik_procentowy_Rm - wynik_procentowy_Sm;
		wynik = funkcja_x(d0, d1, d_0, d_1);
		float p = wynik / (wynik - 0.5);
		cout << "Wynik: " << p << endl;
		tab[k - 1][0] = R / 10000.0;
		tab[k-1][1] = S / 10000.0;
		tab[k-1][2] = Rm / 10000.0;
		tab[k-1][3] = Sm / 10000.0;
		tab[k-1][4] = R2 / 10000.0;
		tab[k-1][5] = S2 / 10000.0;
		tab[k-1][6] = Rm2 / 10000.0;
		tab[k-1][7] = Sm2 / 10000.0;
		tab[k-1][8] = p;
		R = 0;
		S = 0;
		Rm = 0;
		Sm = 0;
		R2 = 0;
		S2 = 0;
		Rm2 = 0;
		Sm2 = 0;
	}
	string pom;
	string dane;
	bool check = false;
	for (int z = 0; z < 9; z++) {
		for (int w = 0; w < 20; w++) {
			pom = to_string(tab[w][z]);
			if (check == true) {									// ustaw check == false , ¿eby ucinaæ zera po przecinku
				if (z != 8) {
					for (int i = 0; i < pom.length(); i++) {
						if (pom[i] == '.')
							break;
						else
							dane += pom[i];
					}
				}
				else {
					dane += pom;
				}
			}
			else {
				dane += pom;
			}
			dane += "\t";
		}
		dane += "\n";
	}
	fstream plik;
	string abd = "jakiœ tekst";
	plik.open("dane.txt", ios::out | ios::trunc);
	if (plik.good() == true) {
		plik << dane;
	}
	plik.close();
	system("pause");
	return 0;
}