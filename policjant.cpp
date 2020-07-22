/*  Policjanci vs złodziej, the static game 2D
	Copyright(C) 2020 Michał Sosnowski

	This program is free software : you can redistribute it and /or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation version 3 of the License, or
	any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.If not, see < https://www.gnu.org/licenses/>.
*/

#include "policjant.h"

#define WSPOLRZEDNA1 0       //pierwsza współrzędna określająca położenie na planszy
#define WSPOLRZEDNA2 1       //druga współrzędna określająca położenie na planszy
#define PUSTE_POLE 0         //stała informująca o tym, że jest to pole puste na planszy (bez przeszkody), na które można normalnie wejść
#define SPRAWDZ_POLE 1       //stała pozwalająca sprawdzić, czy pola dookoła zajmowanego przez postać pola są zajęte czy wolne
#define KONIEC 3             //stała przekazywana w celu informacji o tym, że gra się skończyła z powodu braku dostępnych ruchów dla złodzieja (jest zablokowany z każdej strony)
#define POCZATEK_TABLICY 0   //identyfikuje początek tablicy reprezentującej planszę
#define ROZMIAR_TABLICY 21   //identyfikuje rozmiar tablicy reprezentującej planszę
#define WART_POCZATKOWA -1   //wartość początkowa nadawana niektórym zmiennym w programie
#define WART_POCZAT 0        //wartość początkowa nadawana niektórym zmiennym w programie
#define WART_POCZAT1 1       //wartość początkowa nadawana niektórym zmiennym w programie
#define ZAJETE_POLE 2        //stała informująca o tym, czy to pole jest zajęte przez drugiego policjanta
#define NIC_NIE_ROB 4        //stała informująca o tym, że postać nie musi dokonywać w tym momencie ruchu

enum { GORA = 'W', DOL = 'S', LEWO = 'A', PRAWO = 'D' };               //stałe określające możliwe ruchy postaci
enum { RUCH_GORA = 0, RUCH_DOL = 1, RUCH_LEWO = 2, RUCH_PRAWO = 3 };   //stałe będące ostatecznymi informacjami dla wybrania najlepszego ruchu w tym ustawieniu

/*
  konstruktor tworzący postać policjanta
  przyjmuje argumenty:
  -> num - numer reprezentujący policjanta na planszy
  -> x - pierwsza współrzędna identyfikująca pole, na którym znajduje się policjant
  -> y - druga współrzędna identyfikująca pole, na którym znajduje się policjant
*/
Policjant::Policjant(int num, int x, int y) : numer(num) {
	miejsce[WSPOLRZEDNA1] = x;
	miejsce[WSPOLRZEDNA2] = y;
}

/*
  metoda odpowiedzialna za zmianę wizualną położenia policjanta na planszy
  przyjmuje argumenty:
  -> plansza - przechowuje planszę w pamięci komputera
  -> znak - określa klawisz, który został wciśnięty w celu dokonania ruchu
  -> ruch - informuje o tym, czyja jest teraz kolej na ruch w grze
  -> zlo - przechowuje istotne informacje o złodzieju
  -> koniec - ustawia informacje o trybie zakończenia gry
*/
void Policjant::porusz(Plansza& plansza, char znak, int& ruch, Zlodziej& zlo, int& koniec) {
	//w zależności od wybranego ruchu dokonuje zmian w pamięciowej i wizualnej reprezentacji planszy
	if (znak == GORA) {
		if (!plansza.plansza[miejsce[WSPOLRZEDNA1]][miejsce[WSPOLRZEDNA2] - SPRAWDZ_POLE]) {
			plansza.plansza[miejsce[WSPOLRZEDNA1]][miejsce[WSPOLRZEDNA2]] = PUSTE_POLE;
			plansza.plansza[miejsce[WSPOLRZEDNA1]][miejsce[WSPOLRZEDNA2] - SPRAWDZ_POLE] = numer;
			miejsce[WSPOLRZEDNA2]--;
			ruch++;
		}
	}
	else if (znak == DOL) {
		if (!plansza.plansza[miejsce[WSPOLRZEDNA1]][miejsce[WSPOLRZEDNA2] + SPRAWDZ_POLE]) {
			plansza.plansza[miejsce[WSPOLRZEDNA1]][miejsce[WSPOLRZEDNA2]] = PUSTE_POLE;
			plansza.plansza[miejsce[WSPOLRZEDNA1]][miejsce[WSPOLRZEDNA2] + SPRAWDZ_POLE] = numer;
			miejsce[WSPOLRZEDNA2]++;
			ruch++;
		}
	}
	else if (znak == LEWO) {
		if (!plansza.plansza[miejsce[WSPOLRZEDNA1] - SPRAWDZ_POLE][miejsce[WSPOLRZEDNA2]]) {
			plansza.plansza[miejsce[WSPOLRZEDNA1]][miejsce[WSPOLRZEDNA2]] = PUSTE_POLE;
			plansza.plansza[miejsce[WSPOLRZEDNA1] - SPRAWDZ_POLE][miejsce[WSPOLRZEDNA2]] = numer;
			miejsce[WSPOLRZEDNA1]--;
			ruch++;
		}
	}
	else if (znak == PRAWO) {
		if (!plansza.plansza[miejsce[WSPOLRZEDNA1] + SPRAWDZ_POLE][miejsce[WSPOLRZEDNA2]]) {
			plansza.plansza[miejsce[WSPOLRZEDNA1]][miejsce[WSPOLRZEDNA2]] = PUSTE_POLE;
			plansza.plansza[miejsce[WSPOLRZEDNA1] + SPRAWDZ_POLE][miejsce[WSPOLRZEDNA2]] = numer;
			miejsce[WSPOLRZEDNA1]++;
			ruch++;
		}
	}
	if (plansza.plansza[zlo.miejsce[WSPOLRZEDNA1]][zlo.miejsce[WSPOLRZEDNA2] - SPRAWDZ_POLE] && plansza.plansza[zlo.miejsce[WSPOLRZEDNA1]][zlo.miejsce[WSPOLRZEDNA2] + SPRAWDZ_POLE] && plansza.plansza[zlo.miejsce[WSPOLRZEDNA1] - SPRAWDZ_POLE][zlo.miejsce[WSPOLRZEDNA2]] && plansza.plansza[zlo.miejsce[WSPOLRZEDNA1] + SPRAWDZ_POLE][zlo.miejsce[WSPOLRZEDNA2]]) koniec = KONIEC;
}

/*
  metoda obliczająca najkrótszą drogę policjanta do złodzieja; algorytm A*: https://www.geeksforgeeks.org/a-search-algorithm/
  przyjmuje argumenty:
  -> plansza - przechowuje planszę w pamięci komputera
  -> zlo - przechowuje istotne informacje o złodzieju
*/
int Policjant::dokonaj_ruch(Plansza& plansza, Zlodziej& zlo) {
	bool** zamkn_zbior = new bool* [ROZMIAR_TABLICY];   //zamkn_zbior - przechowuje informacje o tym, czy dane pole było sprawdzane już przez algorytm
	for (int i = WART_POCZAT; i < ROZMIAR_TABLICY; i++) zamkn_zbior[i] = new bool[ROZMIAR_TABLICY];
	for (int i = WART_POCZAT; i < ROZMIAR_TABLICY; i++) {
		for (int j = WART_POCZAT; j < ROZMIAR_TABLICY; j++) zamkn_zbior[i][j] = false;
	}

	komorka** szczegoly = new komorka * [ROZMIAR_TABLICY];   //szczegoly - przechowuje istotne informacje dla algorytmu w związku z danym polem na planszy
	for (int i = WART_POCZAT; i < ROZMIAR_TABLICY; i++) szczegoly[i] = new komorka[ROZMIAR_TABLICY];

	for (int i = WART_POCZAT; i < ROZMIAR_TABLICY; i++) {
		for (int j = WART_POCZAT; j < ROZMIAR_TABLICY; j++) {
			szczegoly[i][j].f = FLT_MAX;
			szczegoly[i][j].g = FLT_MAX;
			szczegoly[i][j].h = FLT_MAX;
			szczegoly[i][j].rodzic_i = WART_POCZATKOWA;
			szczegoly[i][j].rodzic_j = WART_POCZATKOWA;
		}
	}

	//i, j - współrzędne policjanta stosowane w celu zmniejszenia ilości pisanego kodu
	int i = miejsce[WSPOLRZEDNA2];
	int j = miejsce[WSPOLRZEDNA1];

	szczegoly[i][j].f = WART_POCZAT;
	szczegoly[i][j].g = WART_POCZAT;
	szczegoly[i][j].h = WART_POCZAT;
	szczegoly[i][j].rodzic_i = i;
	szczegoly[i][j].rodzic_j = j;

	std::set<std::pair<double, std::pair<int, int>>> zbior_otwarty;   //zbior_otwarty - przechowuje informacje o polach, które będą rozpatrywane przez algorytm

	zbior_otwarty.insert(std::make_pair(WART_POCZAT, std::make_pair(i, j)));

	//szukanie najlepszej ścieżki algorytmem A*
	while (!zbior_otwarty.empty()) {
		//p - współrzędne pola rozpatrywanego w tym momencie przez algorytm
		std::pair<double, std::pair<int, int>> p = *zbior_otwarty.begin();

		zbior_otwarty.erase(zbior_otwarty.begin());

		i = p.second.first;
		j = p.second.second;
		zamkn_zbior[i][j] = true;

		//gNew, hNew, fNew - nowe wartości funkcji g, h i f obliczone przez algorytm
		double gNew, hNew, fNew;

		//sprawdzanie pole znajdującego się na górze od danego pola
		if (jest_prawidlowy(i - SPRAWDZ_POLE, j) == true) {
			if (jest_cel(i - SPRAWDZ_POLE, j, zlo.miejsce[WSPOLRZEDNA2], zlo.miejsce[WSPOLRZEDNA1]) == true) {
				szczegoly[i - SPRAWDZ_POLE][j].rodzic_i = i;
				szczegoly[i - SPRAWDZ_POLE][j].rodzic_j = j;
				zbior_otwarty.clear();
				//decyzja - wybrany najlepszy ruch w tym ustawieniu na planszy
				int decyzja = wyznacz_trase(szczegoly, zlo.miejsce[WSPOLRZEDNA2], zlo.miejsce[WSPOLRZEDNA1], plansza);
				for (int i = WART_POCZAT; i < ROZMIAR_TABLICY; i++) delete[] zamkn_zbior[i];
				delete[] zamkn_zbior;
				for (int i = WART_POCZAT; i < ROZMIAR_TABLICY; i++) delete[] szczegoly[i];
				delete[] szczegoly;
				return decyzja;
			}
			else if (zamkn_zbior[i - SPRAWDZ_POLE][j] == false && jest_odblokowane(plansza.plansza, i - SPRAWDZ_POLE, j) == true) {
				gNew = szczegoly[i][j].g + WART_POCZAT1;
				hNew = policz_h(i - SPRAWDZ_POLE, j, zlo.miejsce[WSPOLRZEDNA2], zlo.miejsce[WSPOLRZEDNA1]);
				fNew = gNew + hNew;

				if (szczegoly[i - SPRAWDZ_POLE][j].f > fNew) {
					zbior_otwarty.insert(std::make_pair(fNew, std::make_pair(i - SPRAWDZ_POLE, j)));

					szczegoly[i - SPRAWDZ_POLE][j].f = fNew;
					szczegoly[i - SPRAWDZ_POLE][j].g = gNew;
					szczegoly[i - SPRAWDZ_POLE][j].h = hNew;
					szczegoly[i - SPRAWDZ_POLE][j].rodzic_i = i;
					szczegoly[i - SPRAWDZ_POLE][j].rodzic_j = j;
				}
			}
		}

		//sprawdzanie pole znajdującego się na dole od danego pola
		if (jest_prawidlowy(i + SPRAWDZ_POLE, j) == true) {
			if (jest_cel(i + SPRAWDZ_POLE, j, zlo.miejsce[WSPOLRZEDNA2], zlo.miejsce[WSPOLRZEDNA1]) == true) {
				szczegoly[i + SPRAWDZ_POLE][j].rodzic_i = i;
				szczegoly[i + SPRAWDZ_POLE][j].rodzic_j = j;
				zbior_otwarty.clear();
				//decyzja - wybrany najlepszy ruch w tym ustawieniu na planszy
				int decyzja = wyznacz_trase(szczegoly, zlo.miejsce[WSPOLRZEDNA2], zlo.miejsce[WSPOLRZEDNA1], plansza);
				for (int i = WART_POCZAT; i < ROZMIAR_TABLICY; i++) delete[] zamkn_zbior[i];
				delete[] zamkn_zbior;
				for (int i = WART_POCZAT; i < ROZMIAR_TABLICY; i++) delete[] szczegoly[i];
				delete[] szczegoly;
				return decyzja;
			}
			else if (zamkn_zbior[i + SPRAWDZ_POLE][j] == false && jest_odblokowane(plansza.plansza, i + SPRAWDZ_POLE, j) == true) {
				gNew = szczegoly[i][j].g + WART_POCZAT1;
				hNew = policz_h(i + SPRAWDZ_POLE, j, zlo.miejsce[WSPOLRZEDNA2], zlo.miejsce[WSPOLRZEDNA1]);
				fNew = gNew + hNew;

				if (szczegoly[i + SPRAWDZ_POLE][j].f > fNew) {
					zbior_otwarty.insert(std::make_pair(fNew, std::make_pair(i + SPRAWDZ_POLE, j)));

					szczegoly[i + SPRAWDZ_POLE][j].f = fNew;
					szczegoly[i + SPRAWDZ_POLE][j].g = gNew;
					szczegoly[i + SPRAWDZ_POLE][j].h = hNew;
					szczegoly[i + SPRAWDZ_POLE][j].rodzic_i = i;
					szczegoly[i + SPRAWDZ_POLE][j].rodzic_j = j;
				}
			}
		}

		//sprawdzanie pole znajdującego się na lewo od danego pola
		if (jest_prawidlowy(i, j + SPRAWDZ_POLE) == true) {
			if (jest_cel(i, j + SPRAWDZ_POLE, zlo.miejsce[WSPOLRZEDNA2], zlo.miejsce[WSPOLRZEDNA1]) == true) {
				szczegoly[i][j + SPRAWDZ_POLE].rodzic_i = i;
				szczegoly[i][j + SPRAWDZ_POLE].rodzic_j = j;
				zbior_otwarty.clear();
				//decyzja - wybrany najlepszy ruch w tym ustawieniu na planszy
				int decyzja = wyznacz_trase(szczegoly, zlo.miejsce[WSPOLRZEDNA2], zlo.miejsce[WSPOLRZEDNA1], plansza);
				for (int i = WART_POCZAT; i < ROZMIAR_TABLICY; i++) delete[] zamkn_zbior[i];
				delete[] zamkn_zbior;
				for (int i = WART_POCZAT; i < ROZMIAR_TABLICY; i++) delete[] szczegoly[i];
				delete[] szczegoly;
				return decyzja;
			}
			else if (zamkn_zbior[i][j + SPRAWDZ_POLE] == false && jest_odblokowane(plansza.plansza, i, j + SPRAWDZ_POLE) == true) {
				gNew = szczegoly[i][j].g + WART_POCZAT1;
				hNew = policz_h(i, j + SPRAWDZ_POLE, zlo.miejsce[WSPOLRZEDNA2], zlo.miejsce[WSPOLRZEDNA1]);
				fNew = gNew + hNew;

				if (szczegoly[i][j + SPRAWDZ_POLE].f > fNew) {
					zbior_otwarty.insert(std::make_pair(fNew, std::make_pair(i, j + SPRAWDZ_POLE)));

					szczegoly[i][j + SPRAWDZ_POLE].f = fNew;
					szczegoly[i][j + SPRAWDZ_POLE].g = gNew;
					szczegoly[i][j + SPRAWDZ_POLE].h = hNew;
					szczegoly[i][j + SPRAWDZ_POLE].rodzic_i = i;
					szczegoly[i][j + SPRAWDZ_POLE].rodzic_j = j;
				}
			}
		}

		//sprawdzanie pole znajdującego się na prawo od danego pola
		if (jest_prawidlowy(i, j - SPRAWDZ_POLE) == true) {
			if (jest_cel(i, j - SPRAWDZ_POLE, zlo.miejsce[WSPOLRZEDNA2], zlo.miejsce[WSPOLRZEDNA1]) == true) {
				szczegoly[i][j - SPRAWDZ_POLE].rodzic_i = i;
				szczegoly[i][j - SPRAWDZ_POLE].rodzic_j = j;
				zbior_otwarty.clear();
				//decyzja - wybrany najlepszy ruch w tym ustawieniu na planszy
				int decyzja = wyznacz_trase(szczegoly, zlo.miejsce[WSPOLRZEDNA2], zlo.miejsce[WSPOLRZEDNA1], plansza);
				for (int i = WART_POCZAT; i < ROZMIAR_TABLICY; i++) delete[] zamkn_zbior[i];
				delete[] zamkn_zbior;
				for (int i = WART_POCZAT; i < ROZMIAR_TABLICY; i++) delete[] szczegoly[i];
				delete[] szczegoly;
				return decyzja;
			}
			else if (zamkn_zbior[i][j - SPRAWDZ_POLE] == false && jest_odblokowane(plansza.plansza, i, j - SPRAWDZ_POLE) == true) {
				gNew = szczegoly[i][j].g + WART_POCZAT1;
				hNew = policz_h(i, j - SPRAWDZ_POLE, zlo.miejsce[WSPOLRZEDNA2], zlo.miejsce[WSPOLRZEDNA1]);
				fNew = gNew + hNew;

				if (szczegoly[i][j - SPRAWDZ_POLE].f > fNew) {
					zbior_otwarty.insert(std::make_pair(fNew, std::make_pair(i, j - SPRAWDZ_POLE)));

					szczegoly[i][j - SPRAWDZ_POLE].f = fNew;
					szczegoly[i][j - SPRAWDZ_POLE].g = gNew;
					szczegoly[i][j - SPRAWDZ_POLE].h = hNew;
					szczegoly[i][j - SPRAWDZ_POLE].rodzic_i = i;
					szczegoly[i][j - SPRAWDZ_POLE].rodzic_j = j;
				}
			}
		}
	}
	for (int i = WART_POCZAT; i < ROZMIAR_TABLICY; i++) delete[] zamkn_zbior[i];
	delete[] zamkn_zbior;
	for (int i = WART_POCZAT; i < ROZMIAR_TABLICY; i++) delete[] szczegoly[i];
	delete[] szczegoly;
}

/*
  metoda sprawdza, czy postać nie wychodzi poza planszę; algorytm A*: https://www.geeksforgeeks.org/a-search-algorithm/
  przyjmuje argumenty:
  -> x - pierwsza współrzędna danego pola na planszy
  -> y - druga współrzędna danego pola na planszy
*/
bool Policjant::jest_prawidlowy(int x, int y) {
	return (x >= POCZATEK_TABLICY) && (x < ROZMIAR_TABLICY) && (y >= POCZATEK_TABLICY) && (y < ROZMIAR_TABLICY);
}

/*
  metoda sprawdza, czy na sąsiednim polu policjanta stoi złodziej; algorytm A*: https://www.geeksforgeeks.org/a-search-algorithm/
  przyjmuje argumenty:
  -> x - pierwsza współrzędna pola, na którym jest policjant
  -> y - druga współrzędna pola, na którym jest policjant
  -> z - pierwsza współrzędna pola, na którym jest złodziej
  -> w - druga współrzędna pola, na którym jest złodziej
*/
bool Policjant::jest_cel(int x, int y, int z, int w) {
	if (x == z && y == w) return true;
	else return false;
}

/*
  metoda sprawdza, czy sąsiednie pole jest puste (można na nie wejść); algorytm A*: https://www.geeksforgeeks.org/a-search-algorithm/
  przyjmuje argumenty:
  -> plansza - przechowuje planszę w postaci tablicy w pamięci komputera
  -> x - pierwsza współrzędna pola, na które chce wejść policjant
  -> y - druga współrzędna pola, na które chce wejść policjant
*/
bool Policjant::jest_odblokowane(int plansza[][ROZMIAR_TABLICY], int x, int y) {
	if (!plansza[y][x]) return true;
	else return false;
}

/*
  metoda licząca funkcję heurystyczną (h) w algorytmie A*: https://www.geeksforgeeks.org/a-search-algorithm/
  przyjmuje argumenty:
  -> rzad - pierwsza współrzędna danego pola
  -> kol - druga współrzędna danego pola
  -> x - pierwsza współrzędna pola, na którym jest złodziej
  -> y - druga współrzędna pola, na którym jest złodziej
*/
double Policjant::policz_h(int rzad, int kol, int x, int y) {
	return ((double)sqrt((rzad - x) * (rzad - x) + (kol - y) * (kol - y)));
}

/*
  metoda wyznacza najkrótszą trasę policjanta do złodzieja i podaje, w którą stronę powinien ruszyć się policjant; algorytm A*: https://www.geeksforgeeks.org/a-search-algorithm/
  przyjmuje argumenty:
  -> szczegoly - przechowuje istotne informacje dla algorytmu w związku z danym polem na planszy
  -> x - pierwsza współrzędna pola, na którym jest złodziej
  -> y - druga współrzędna pola, na którym jest złodziej
  -> plansza - przechowuje planszę w pamięci komputera
*/
int Policjant::wyznacz_trase(komorka** szczegoly, int x, int y, Plansza& plansza) {
	int rzad = x;      //rzad - pierwsza współrzędna pola, na którym jest złodziej
	int kolumna = y;   //kolumna - druga współrzędna pola, na którym jest złodziej

	std::stack<std::pair<int, int>> Sciezka;   //Sciezka - przechowuje całą informacje o najlepszej ścieżce

	//tworzy ostateczną najlepszą ścieżkę dojścia do złodzieja
	while (!(szczegoly[rzad][kolumna].rodzic_i == rzad && szczegoly[rzad][kolumna].rodzic_j == kolumna)) {
		Sciezka.push(std::make_pair(rzad, kolumna));
		int temp_rzad = szczegoly[rzad][kolumna].rodzic_i;   //temp_rzad - przechowuje pierwszą współrzędną kolejnego rozpatrywanego pola planszy
		int temp_kol = szczegoly[rzad][kolumna].rodzic_j;    //temp_kol - przechowuje drugą współrzędną kolejnego rozpatrywanego pola planszy
		rzad = temp_rzad;
		kolumna = temp_kol;
	}

	std::pair<int, int> para = Sciezka.top();

	//nie wykonuj ruchu policjantem, jeśli nastąpi jeden z poniższych warunków
	if ((miejsce[WSPOLRZEDNA1] == y && miejsce[WSPOLRZEDNA2] + SPRAWDZ_POLE == x) || (miejsce[WSPOLRZEDNA1] == y && miejsce[WSPOLRZEDNA2] - SPRAWDZ_POLE == x) || (miejsce[WSPOLRZEDNA1] - SPRAWDZ_POLE == y && miejsce[WSPOLRZEDNA2] == x) || (miejsce[WSPOLRZEDNA1] + SPRAWDZ_POLE == y && miejsce[WSPOLRZEDNA2] == x)) return NIC_NIE_ROB;
	if (plansza.plansza[miejsce[WSPOLRZEDNA2] - SPRAWDZ_POLE][miejsce[WSPOLRZEDNA1]] == ZAJETE_POLE || plansza.plansza[miejsce[WSPOLRZEDNA2] + SPRAWDZ_POLE][miejsce[WSPOLRZEDNA1]] == ZAJETE_POLE || plansza.plansza[miejsce[WSPOLRZEDNA2]][miejsce[WSPOLRZEDNA1] - SPRAWDZ_POLE] == ZAJETE_POLE || plansza.plansza[miejsce[WSPOLRZEDNA2]][miejsce[WSPOLRZEDNA1] + SPRAWDZ_POLE] == ZAJETE_POLE) return NIC_NIE_ROB;

	//wybór najlepszego ruchu w tym ustawieniu
	if (miejsce[WSPOLRZEDNA2] + SPRAWDZ_POLE == para.first) return RUCH_DOL;
	else if (miejsce[WSPOLRZEDNA2] - SPRAWDZ_POLE == para.first) return RUCH_GORA;
	else if (miejsce[WSPOLRZEDNA1] - SPRAWDZ_POLE == para.second) return RUCH_LEWO;
	else return RUCH_PRAWO;
}
