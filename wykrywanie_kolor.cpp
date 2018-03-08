#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include "opencv2\opencv.hpp"


using namespace std;
using namespace cv;

 int  R=0;
 int  S=0;

int Rm=0;
int Sm=0;
// vectory do zapisu po zmianit LSB
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
void zastosowanie_maski(int M[4],int tab[12],int tab2[12]) {
	int pom = 0;
	bool wyjscie = false;
	for (int i = 0; i < 12; i++) {
		if (i % 3 == 0 && i!=0) {
			pom++;
		}
		if (M[pom] == 1) {
			if (tab[i] % 2 == 0)
				tab2[i] = tab[i] + 1;
			else
				tab2[i] = tab[i] - 1;
		}
		else if (M[pom] == -1) {
			if (tab[i] % 2 == 0)
				tab2[i] = tab[i] - 1;
			else
				tab2[i] = tab[i] + 1;
		}
		else if (M[pom] == 0) {
			tab2[i] = tab[i];
		}
	}
}//dzia³a
void zamiana_bitu(Mat img) {
	Vec3b intensity;											// zmienna posiadaj¹ce pole .val z wartoœciami rgb
	int pom1=0,pom2=0;
	for (int i = 0; i < img.rows; i++) {
		for (int j = 0; j < img.cols; j++) {

			intensity = img.at<Vec3b>(i, j);

			uchar blue = intensity.val[0];
			pom1 = uchar_na_binarke(blue);
			pom2 = zamiana_LSB(pom1);
			pom1 = binarnie_na_liczbe(pom2);
			img.at<cv::Vec3b>(i, j)[0] = (uchar)pom1;

			uchar green = intensity.val[1];
			pom1 = uchar_na_binarke(green);
			pom2 = zamiana_LSB(pom1);
			pom1 = binarnie_na_liczbe(pom2);
			img.at<cv::Vec3b>(i, j)[1] = (uchar)pom1;

			uchar red = intensity.val[2];
			pom1 = uchar_na_binarke(red);
			pom2 = zamiana_LSB(pom1);
			pom1 = binarnie_na_liczbe(pom2);
			img.at<cv::Vec3b>(i, j)[2] = (uchar)pom1;
		}
	}
}
int funkcja_glowna(Mat img, int parametr) {
	bool wyjscie = false;
	int u = 0;
	int ilosc_iteracji = 0;
	//int M[4] = { 1,0,0,1 };
	//int nM[4] = { -1,0,0,-1 };

	//int M[4] = { 1, 1, 1, -1 };
	//int nM[4] = { -1,-1,-1,1 };

	int M[4] = { 0, 1, 1, 0 };							// maska
	int nM[4] = { 0, -1, -1, 0 };						// negacja maski
	Vec3b intensity;
	int R_m = 0, S_m = 0;							// Grupy
	int nR_m = 0, nS_m = 0;							// grupy z zanegowan¹ mask¹
	int tab[12], tab2[12],tab3[12], licznik=0, pom=0;		// tab - tabela z kanalami 4 pikselow / tab2 - tabela z kanalami 4 pikselow po zastosowaniu maski M/ tab3 - tabela z kanalami 4 pikselow po zastosowaniu maski nM
	int a = img.rows / 2;
	int b = img.cols / 2;
	int *tablica_wynikow = new int[a*b];
	int *tablica_wynikow_po_masce_M = new int[a*b];
	int *tablica_wynikow_po_masce_nM = new int[a*b];
	for (int i = 0; i < a*b; i++) {
		tablica_wynikow[i] = 0;
		tablica_wynikow_po_masce_M[i] = 0;
		tablica_wynikow_po_masce_nM[i] = 0;
	}

	for (int i = 0; i < img.rows; i++) {

		for (int j = 0; j < img.cols-3; j+=4) {
			intensity = img.at<Vec3b>(i, j);				

			uchar blue = intensity.val[0];
			tab[0] = (int)blue;
			uchar green = intensity.val[1];
			tab[1] = (int)green;
			uchar red = intensity.val[2];
			tab[2] = (int)red;

			intensity = img.at<Vec3b>(i, j+1);

			blue = intensity.val[0];
			tab[3] = (int)blue;
			green = intensity.val[1];
			tab[4] = (int)green;
			red = intensity.val[2];
			tab[5] = (int)red;

			intensity = img.at<Vec3b>(i, j+2);

			blue = intensity.val[0];
			tab[6] = (int)blue;
			green = intensity.val[1];
			tab[7] = (int)green;
			red = intensity.val[2];
			tab[8] = (int)red;

			intensity = img.at<Vec3b>(i, j+3);

			blue = intensity.val[0];
			tab[9] = (int)blue;
			green = intensity.val[1];
			tab[10] = (int)green;
			red = intensity.val[2];
			tab[11] = (int)red;

			//obliczanie funkcji
			int wynik_funkcji=0;
			for (int k = 1; k <= 4; k++) {
				wynik_funkcji += abs(tab[k*3] - tab[k*3-3]);
				wynik_funkcji += abs(tab[k*3 + 1] - tab[k*3-2]);
				wynik_funkcji += abs(tab[k*3 + 2] - tab[k*3]-1);
			}
			// koniec

			// zastosowanie maski M i -M
			zastosowanie_maski(M, tab, tab2);
			zastosowanie_maski(nM, tab, tab3);
			// koniec maski

			//obliczanie funkcji po zastosowaniu maski
			int wynik_funkcji_po_masce_M = 0, wynik_funkcji_po_masce_nM = 0;
			for (int k = 1; k <= 4; k++) {
				wynik_funkcji_po_masce_M += abs(tab2[k * 3] - tab2[k * 3 - 3]);
				wynik_funkcji_po_masce_M += abs(tab2[k * 3 + 1] - tab2[k * 3 - 2]);
				wynik_funkcji_po_masce_M += abs(tab2[k * 3 + 2] - tab2[k * 3] - 1);
				wynik_funkcji_po_masce_nM += abs(tab3[k * 3] - tab3[k * 3 - 3]);
				wynik_funkcji_po_masce_nM += abs(tab3[k * 3 + 1] - tab3[k * 3 - 2]);
				wynik_funkcji_po_masce_nM += abs(tab3[k * 3 + 2] - tab3[k * 3] - 1);
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
	float wynik;
	Mat img = imread("zaszyfrowany.bmp");						//(domyœlnie 3 kana³y: BGR)
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
	cout << "R: " << R2 << endl;
	cout << "S: " << S2 << endl;
	cout << "Rm: " << Rm2 << endl;
	cout << "Sm: " << Sm2 << endl;
	wynik_procentowy_R = ((float)R2 * 100.0) / (float)ilosc_iteracji;
	wynik_procentowy_S = ((float)S2 * 100.0) / (float)ilosc_iteracji;
	wynik_procentowy_Rm = ((float)Rm2 * 100.0) / (float)ilosc_iteracji;
	wynik_procentowy_Sm = ((float)Sm2 * 100.0) / (float)ilosc_iteracji;
	float d1 = wynik_procentowy_R - wynik_procentowy_S;
	float d_1 = wynik_procentowy_Rm - wynik_procentowy_Sm;
	wynik = funkcja_x(d0, d1, d_0, d_1);
	float p = wynik / (wynik - 0.5);
	cout << "Wynik: " << p << endl;
	system("pause");
	return 0;
}