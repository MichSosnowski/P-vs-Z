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

#include "plansza.h"

#define WART_POCZAT 0        //wartość początkowa nadawana niektórym zmiennym w programie
#define ROZMIAR_PLANSZY 21   //identyfikuje rozmiar planszy
#define ZWIEKSZAJ_O_2 2      //stała informująca o tym, że dana zmienna ma być zwiększana o 2
#define ZWIEKSZAJ_O_4 4      //stała informująca o tym, że dana zmienna ma być zwiększana o 4
#define PUSTE_POLE 0         //stała informująca o tym, że jest to pole puste na planszy (bez przeszkody), na które można normalnie wejść
#define PRZESZKODA 1         //stała informująca o tym, że to pole jest zajęte przez przeszkodę, którą nie jest postać
#define POLICJANT1 2         //stała informująca o tym, że to pole jest zajęte przez pierwszego policjanta
#define POLICJANT2 3         //stała informująca o tym, że to pole jest zajęte przez drugiego policjanta
#define ZLODZIEJ 4           //stała informująca o tym, że to pole jest zajęte przez złodzieja
#define LOSOWANIE 5          //identyfikuje liczby możliwe do wylosowania (0, 1, 2, 3, 4)
#define IL_WIERZCHOLKOW 4    //stała reprezentująca ilość wierzchołków pojedynczego pola na planszy
#define ZWIEKSZ_O_1 1        //stała informująca o tym, że dana zmienna ma być zwiększona o 1

enum { WYLOS_1 = 1, WYLOS_2 = 2, WYLOS_3 = 3 };                    //możliwe wylosowane przypadki decydujące o wyglądzie planszy do gry

enum { WIERZCH0 = 0, WIERZCH1 = 1, WIERZCH2 = 2, WIERZCH3 = 3 };   //stałe reprezentujące wierzchołki pojedynczego pola planszy

//stałe identyfikujące wspołrzędne pól na planszy
enum {
	WSP_POLA0 = 0,
	WSP_POLA1 = 1,
	WSP_POLA2 = 2,
	WSP_POLA3 = 3,
	WSP_POLA4 = 4,
	WSP_POLA5 = 5,
	WSP_POLA6 = 6,
	WSP_POLA7 = 7,
	WSP_POLA8 = 8,
	WSP_POLA9 = 9,
	WSP_POLA10 = 10,
	WSP_POLA11 = 11,
	WSP_POLA12 = 12,
	WSP_POLA13 = 13,
	WSP_POLA14 = 14,
	WSP_POLA15 = 15,
	WSP_POLA16 = 16,
	WSP_POLA17 = 17,
	WSP_POLA18 = 18,
	WSP_POLA19 = 19,
	WSP_POLA20 = 20
};

//konstruktor domyślny budujący planszę w pamięci komputera
Plansza::Plansza() {
	srand((unsigned)time(WART_POCZAT));

	//wypełnienie planszy pustymi polami
	for (int i = WART_POCZAT; i < ROZMIAR_PLANSZY; i++) {
		for (int j = WART_POCZAT; j < ROZMIAR_PLANSZY; j++) plansza[i][j] = PUSTE_POLE;
	}

	//utworzenie granic planszy poprzez ustawienie przeszkód
	for (int i = WART_POCZAT; i < ROZMIAR_PLANSZY; i++) plansza[i][WSP_POLA0] = plansza[i][WSP_POLA20] = PRZESZKODA;
	for (int i = WART_POCZAT; i < ROZMIAR_PLANSZY; i++) plansza[WSP_POLA0][i] = plansza[WSP_POLA20][i] = PRZESZKODA;

	//losowanie planszy
	int los = rand() % LOSOWANIE;   //los - przechowuje wylosowany numer planszy

	//utworzenie planszy na podstawie wylsowanej wartości poprzez wypełnienie jej pól przeszkodami
	if (!los) {
		for (int i = WSP_POLA7; i < WSP_POLA11; i++) plansza[i][WSP_POLA1] = PRZESZKODA;
		for (int i = WSP_POLA2; i < WSP_POLA19; i++) plansza[i][WSP_POLA2] = PRZESZKODA;
		plansza[WSP_POLA4][WSP_POLA2] = plansza[WSP_POLA6][WSP_POLA2] = plansza[WSP_POLA11][WSP_POLA2] = plansza[WSP_POLA15][WSP_POLA2] = PUSTE_POLE;
		for (int i = WSP_POLA7; i < WSP_POLA11; i++) plansza[i][WSP_POLA2] = PUSTE_POLE;
		plansza[WSP_POLA2][WSP_POLA3] = plansza[WSP_POLA5][WSP_POLA3] = plansza[WSP_POLA7][WSP_POLA3] = plansza[WSP_POLA8][WSP_POLA3] = plansza[WSP_POLA10][WSP_POLA3] = plansza[WSP_POLA14][WSP_POLA3] = plansza[WSP_POLA16][WSP_POLA3] = PRZESZKODA;
		for (int i = WSP_POLA4; i < WSP_POLA20; i++) plansza[i][WSP_POLA4] = PRZESZKODA;
		plansza[WSP_POLA6][WSP_POLA4] = plansza[WSP_POLA9][WSP_POLA4] = plansza[WSP_POLA13][WSP_POLA4] = plansza[WSP_POLA15][WSP_POLA4] = plansza[WSP_POLA17][WSP_POLA4] = PUSTE_POLE;
		for (int i = WSP_POLA4; i < WSP_POLA17; i += ZWIEKSZAJ_O_4) plansza[i][WSP_POLA5] = PRZESZKODA;
		plansza[WSP_POLA2][WSP_POLA5] = plansza[WSP_POLA7][WSP_POLA5] = PRZESZKODA;
		plansza[WSP_POLA2][WSP_POLA6] = PRZESZKODA;
		for (int i = WSP_POLA6; i < WSP_POLA19; i++) plansza[i][WSP_POLA6] = PRZESZKODA;
		plansza[WSP_POLA9][WSP_POLA6] = plansza[WSP_POLA11][WSP_POLA6] = plansza[WSP_POLA15][WSP_POLA6] = PUSTE_POLE;
		plansza[WSP_POLA2][WSP_POLA7] = plansza[WSP_POLA4][WSP_POLA7] = plansza[WSP_POLA8][WSP_POLA7] = plansza[WSP_POLA10][WSP_POLA7] = PRZESZKODA;
		plansza[WSP_POLA4][WSP_POLA8] = plansza[WSP_POLA5][WSP_POLA8] = plansza[WSP_POLA6][WSP_POLA8] = plansza[WSP_POLA10][WSP_POLA8] = plansza[WSP_POLA12][WSP_POLA8] = PRZESZKODA;
		for (int i = WSP_POLA14; i < WSP_POLA19; i++) plansza[i][WSP_POLA8] = PRZESZKODA;
		plansza[WSP_POLA2][WSP_POLA9] = plansza[WSP_POLA8][WSP_POLA9] = plansza[WSP_POLA12][WSP_POLA9] = plansza[WSP_POLA18][WSP_POLA9] = PRZESZKODA;
		for (int i = WSP_POLA2; i < WSP_POLA19; i++) plansza[i][WSP_POLA10] = PRZESZKODA;
		plansza[WSP_POLA3][WSP_POLA10] = plansza[WSP_POLA7][WSP_POLA10] = plansza[WSP_POLA13][WSP_POLA10] = plansza[WSP_POLA17][WSP_POLA10] = PUSTE_POLE;
		plansza[WSP_POLA2][WSP_POLA11] = plansza[WSP_POLA8][WSP_POLA11] = plansza[WSP_POLA12][WSP_POLA11] = plansza[WSP_POLA18][WSP_POLA11] = PRZESZKODA;
		for (int i = WSP_POLA2; i < WSP_POLA17; i++) plansza[i][WSP_POLA12] = PRZESZKODA;
		plansza[WSP_POLA7][WSP_POLA12] = plansza[WSP_POLA9][WSP_POLA12] = plansza[WSP_POLA11][WSP_POLA12] = plansza[WSP_POLA12][WSP_POLA12] = plansza[WSP_POLA13][WSP_POLA12] = PUSTE_POLE;
		plansza[WSP_POLA10][WSP_POLA13] = plansza[WSP_POLA12][WSP_POLA13] = plansza[WSP_POLA16][WSP_POLA13] = plansza[WSP_POLA18][WSP_POLA13] = PRZESZKODA;
		for (int i = WSP_POLA2; i < WSP_POLA15; i++) plansza[i][WSP_POLA14] = PRZESZKODA;
		plansza[WSP_POLA5][WSP_POLA14] = plansza[WSP_POLA9][WSP_POLA14] = plansza[WSP_POLA11][WSP_POLA14] = PUSTE_POLE;
		plansza[WSP_POLA18][WSP_POLA14] = PRZESZKODA;
		plansza[WSP_POLA4][WSP_POLA15] = plansza[WSP_POLA8][WSP_POLA15] = plansza[WSP_POLA12][WSP_POLA15] = plansza[WSP_POLA13][WSP_POLA15] = plansza[WSP_POLA16][WSP_POLA15] = plansza[WSP_POLA18][WSP_POLA15] = PRZESZKODA;
		for (int i = WSP_POLA1; i < WSP_POLA17; i++) plansza[i][WSP_POLA16] = PRZESZKODA;
		plansza[WSP_POLA3][WSP_POLA16] = plansza[WSP_POLA5][WSP_POLA16] = plansza[WSP_POLA7][WSP_POLA16] = plansza[WSP_POLA11][WSP_POLA16] = plansza[WSP_POLA14][WSP_POLA16] = PUSTE_POLE;
		plansza[WSP_POLA4][WSP_POLA17] = plansza[WSP_POLA6][WSP_POLA17] = plansza[WSP_POLA10][WSP_POLA17] = plansza[WSP_POLA12][WSP_POLA17] = plansza[WSP_POLA13][WSP_POLA17] = plansza[WSP_POLA15][WSP_POLA17] = plansza[WSP_POLA18][WSP_POLA17] = PRZESZKODA;
		for (int i = WSP_POLA2; i < WSP_POLA19; i++) plansza[i][WSP_POLA18] = PRZESZKODA;
		for (int i = WSP_POLA10; i < WSP_POLA14; i++) plansza[i][WSP_POLA18] = PUSTE_POLE;
		plansza[WSP_POLA5][WSP_POLA18] = plansza[WSP_POLA9][WSP_POLA18] = plansza[WSP_POLA14][WSP_POLA18] = plansza[WSP_POLA16][WSP_POLA18] = PUSTE_POLE;
		for (int i = WSP_POLA10; i < WSP_POLA14; i++) plansza[i][WSP_POLA19] = PRZESZKODA;
	}
	else if (los == WYLOS_1) {
		plansza[WSP_POLA7][WSP_POLA1] = PRZESZKODA;
		for (int i = WSP_POLA2; i < WSP_POLA19; i++) plansza[i][WSP_POLA2] = plansza[i][WSP_POLA6] = plansza[i][WSP_POLA10] = plansza[i][WSP_POLA18] = PRZESZKODA;
		plansza[WSP_POLA6][WSP_POLA2] = plansza[WSP_POLA8][WSP_POLA2] = plansza[WSP_POLA11][WSP_POLA2] = plansza[WSP_POLA15][WSP_POLA2] = PUSTE_POLE;
		plansza[WSP_POLA10][WSP_POLA3] = plansza[WSP_POLA18][WSP_POLA3] = PRZESZKODA;
		for (int i = WSP_POLA1; i < WSP_POLA19; i++) plansza[i][WSP_POLA4] = PRZESZKODA;
		plansza[WSP_POLA3][WSP_POLA4] = plansza[WSP_POLA9][WSP_POLA4] = plansza[WSP_POLA11][WSP_POLA4] = plansza[WSP_POLA17][WSP_POLA4] = PUSTE_POLE;
		plansza[WSP_POLA10][WSP_POLA5] = plansza[WSP_POLA18][WSP_POLA5] = PRZESZKODA;
		plansza[WSP_POLA5][WSP_POLA6] = plansza[WSP_POLA9][WSP_POLA6] = plansza[WSP_POLA10][WSP_POLA6] = plansza[WSP_POLA11][WSP_POLA6] = plansza[WSP_POLA15][WSP_POLA6] = plansza[WSP_POLA17][WSP_POLA6] = PUSTE_POLE;
		plansza[WSP_POLA4][WSP_POLA7] = plansza[WSP_POLA8][WSP_POLA7] = plansza[WSP_POLA10][WSP_POLA7] = plansza[WSP_POLA14][WSP_POLA7] = plansza[WSP_POLA16][WSP_POLA7] = PRZESZKODA;
		plansza[WSP_POLA2][WSP_POLA8] = plansza[WSP_POLA4][WSP_POLA8] = plansza[WSP_POLA6][WSP_POLA8] = plansza[WSP_POLA8][WSP_POLA8] = plansza[WSP_POLA10][WSP_POLA8] = plansza[WSP_POLA12][WSP_POLA8] = plansza[WSP_POLA14][WSP_POLA8] = plansza[WSP_POLA16][WSP_POLA8] = plansza[WSP_POLA18][WSP_POLA8] = plansza[WSP_POLA19][WSP_POLA8] = PRZESZKODA;
		plansza[WSP_POLA2][WSP_POLA9] = plansza[WSP_POLA6][WSP_POLA9] = plansza[WSP_POLA12][WSP_POLA9] = plansza[WSP_POLA14][WSP_POLA9] = plansza[WSP_POLA16][WSP_POLA9] = PRZESZKODA;
		plansza[WSP_POLA5][WSP_POLA10] = plansza[WSP_POLA7][WSP_POLA10] = plansza[WSP_POLA13][WSP_POLA10] = plansza[WSP_POLA14][WSP_POLA10] = plansza[WSP_POLA15][WSP_POLA10] = plansza[WSP_POLA17][WSP_POLA10] = PUSTE_POLE;
		plansza[WSP_POLA12][WSP_POLA11] = plansza[WSP_POLA14][WSP_POLA11] = plansza[WSP_POLA16][WSP_POLA11] = plansza[WSP_POLA18][WSP_POLA11] = PRZESZKODA;
		plansza[WSP_POLA1][WSP_POLA12] = plansza[WSP_POLA2][WSP_POLA12] = plansza[WSP_POLA3][WSP_POLA12] = plansza[WSP_POLA4][WSP_POLA12] = plansza[WSP_POLA5][WSP_POLA12] = plansza[WSP_POLA7][WSP_POLA12] = plansza[WSP_POLA8][WSP_POLA12] = plansza[WSP_POLA10][WSP_POLA12] = plansza[WSP_POLA14][WSP_POLA12] = PRZESZKODA;
		plansza[WSP_POLA10][WSP_POLA13] = plansza[WSP_POLA12][WSP_POLA13] = plansza[WSP_POLA14][WSP_POLA13] = plansza[WSP_POLA16][WSP_POLA13] = plansza[WSP_POLA18][WSP_POLA13] = PRZESZKODA;
		plansza[WSP_POLA2][WSP_POLA14] = plansza[WSP_POLA3][WSP_POLA14] = plansza[WSP_POLA4][WSP_POLA14] = plansza[WSP_POLA6][WSP_POLA14] = plansza[WSP_POLA8][WSP_POLA14] = plansza[WSP_POLA9][WSP_POLA14] = plansza[WSP_POLA10][WSP_POLA14] = plansza[WSP_POLA12][WSP_POLA14] = plansza[WSP_POLA18][WSP_POLA14] = PRZESZKODA;
		for (int i = WSP_POLA12; i < WSP_POLA19; i++) plansza[i][WSP_POLA15] = PRZESZKODA;
		plansza[WSP_POLA2][WSP_POLA16] = plansza[WSP_POLA4][WSP_POLA16] = plansza[WSP_POLA5][WSP_POLA16] = plansza[WSP_POLA6][WSP_POLA16] = plansza[WSP_POLA7][WSP_POLA16] = plansza[WSP_POLA8][WSP_POLA16] = plansza[WSP_POLA10][WSP_POLA16] = plansza[WSP_POLA18][WSP_POLA16] = PRZESZKODA;
		plansza[WSP_POLA2][WSP_POLA17] = plansza[WSP_POLA6][WSP_POLA17] = plansza[WSP_POLA10][WSP_POLA17] = plansza[WSP_POLA12][WSP_POLA17] = plansza[WSP_POLA14][WSP_POLA17] = plansza[WSP_POLA16][WSP_POLA17] = PRZESZKODA;
		plansza[WSP_POLA5][WSP_POLA18] = plansza[WSP_POLA7][WSP_POLA18] = plansza[WSP_POLA9][WSP_POLA18] = plansza[WSP_POLA11][WSP_POLA18] = plansza[WSP_POLA13][WSP_POLA18] = plansza[WSP_POLA15][WSP_POLA18] = PUSTE_POLE;
		plansza[WSP_POLA8][WSP_POLA19] = plansza[WSP_POLA14][WSP_POLA19] = PRZESZKODA;
	}
	else if (los == WYLOS_2) {
		plansza[WSP_POLA4][WSP_POLA1] = plansza[WSP_POLA12][WSP_POLA1] = PRZESZKODA;
		for (int i = WSP_POLA2; i < WSP_POLA19; i++) plansza[i][WSP_POLA2] = PRZESZKODA;
		plansza[WSP_POLA3][WSP_POLA2] = plansza[WSP_POLA5][WSP_POLA2] = plansza[WSP_POLA8][WSP_POLA2] = plansza[WSP_POLA11][WSP_POLA2] = plansza[WSP_POLA13][WSP_POLA2] = plansza[WSP_POLA17][WSP_POLA2] = PUSTE_POLE;
		plansza[WSP_POLA2][WSP_POLA3] = plansza[WSP_POLA6][WSP_POLA3] = plansza[WSP_POLA10][WSP_POLA3] = plansza[WSP_POLA12][WSP_POLA3] = plansza[WSP_POLA18][WSP_POLA3] = PRZESZKODA;
		for (int i = WSP_POLA2; i < WSP_POLA19; i += ZWIEKSZAJ_O_2) plansza[i][WSP_POLA4] = PRZESZKODA;
		plansza[WSP_POLA10][WSP_POLA4] = plansza[WSP_POLA16][WSP_POLA4] = PUSTE_POLE;
		plansza[WSP_POLA15][WSP_POLA4] = plansza[WSP_POLA17][WSP_POLA4] = PRZESZKODA;
		plansza[WSP_POLA4][WSP_POLA5] = plansza[WSP_POLA6][WSP_POLA5] = plansza[WSP_POLA8][WSP_POLA5] = plansza[WSP_POLA10][WSP_POLA5] = plansza[WSP_POLA12][WSP_POLA5] = PRZESZKODA;
		for (int i = WSP_POLA2; i < WSP_POLA17; i += ZWIEKSZAJ_O_2) plansza[i][WSP_POLA6] = PRZESZKODA;
		plansza[WSP_POLA6][WSP_POLA6] = PUSTE_POLE;
		plansza[WSP_POLA17][WSP_POLA6] = plansza[WSP_POLA19][WSP_POLA6] = PRZESZKODA;
		for (int i = WSP_POLA2; i < WSP_POLA11; i += ZWIEKSZAJ_O_2) plansza[i][WSP_POLA7] = PRZESZKODA;
		plansza[WSP_POLA14][WSP_POLA7] = PRZESZKODA;
		for (int i = WSP_POLA4; i < WSP_POLA19; i += ZWIEKSZAJ_O_2) plansza[i][WSP_POLA8] = PRZESZKODA;
		plansza[WSP_POLA8][WSP_POLA8] = PUSTE_POLE;
		plansza[WSP_POLA13][WSP_POLA8] = plansza[WSP_POLA17][WSP_POLA8] = PRZESZKODA;
		for (int i = WSP_POLA2; i < WSP_POLA11; i += ZWIEKSZAJ_O_2) plansza[i][WSP_POLA9] = PRZESZKODA;
		plansza[WSP_POLA1][WSP_POLA9] = plansza[WSP_POLA16][WSP_POLA9] = PRZESZKODA;
		plansza[WSP_POLA8][WSP_POLA10] = plansza[WSP_POLA12][WSP_POLA10] = plansza[WSP_POLA14][WSP_POLA10] = plansza[WSP_POLA18][WSP_POLA10] = PRZESZKODA;
		for (int i = WSP_POLA2; i < WSP_POLA19; i++) plansza[i][WSP_POLA11] = PRZESZKODA;
		plansza[WSP_POLA3][WSP_POLA11] = plansza[WSP_POLA7][WSP_POLA11] = plansza[WSP_POLA9][WSP_POLA11] = plansza[WSP_POLA11][WSP_POLA11] = plansza[WSP_POLA13][WSP_POLA11] = plansza[WSP_POLA15][WSP_POLA11] = plansza[WSP_POLA17][WSP_POLA11] = PUSTE_POLE;
		plansza[WSP_POLA2][WSP_POLA12] = plansza[WSP_POLA6][WSP_POLA12] = plansza[WSP_POLA8][WSP_POLA12] = plansza[WSP_POLA10][WSP_POLA12] = plansza[WSP_POLA12][WSP_POLA12] = plansza[WSP_POLA16][WSP_POLA12] = plansza[WSP_POLA18][WSP_POLA12] = PRZESZKODA;
		plansza[WSP_POLA2][WSP_POLA13] = plansza[WSP_POLA3][WSP_POLA13] = plansza[WSP_POLA4][WSP_POLA13] = plansza[WSP_POLA8][WSP_POLA13] = plansza[WSP_POLA12][WSP_POLA13] = plansza[WSP_POLA14][WSP_POLA13] = plansza[WSP_POLA16][WSP_POLA13] = plansza[WSP_POLA18][WSP_POLA13] = PRZESZKODA;
		for (int i = WSP_POLA6; i < WSP_POLA17; i += ZWIEKSZAJ_O_2) plansza[i][WSP_POLA14] = PRZESZKODA;
		for (int i = WSP_POLA1; i < WSP_POLA20; i++) plansza[i][WSP_POLA15] = PRZESZKODA;
		plansza[WSP_POLA3][WSP_POLA15] = plansza[WSP_POLA5][WSP_POLA15] = plansza[WSP_POLA7][WSP_POLA15] = plansza[WSP_POLA9][WSP_POLA15] = plansza[WSP_POLA11][WSP_POLA15] = plansza[WSP_POLA12][WSP_POLA15] = plansza[WSP_POLA13][WSP_POLA15] = plansza[WSP_POLA15][WSP_POLA15] = plansza[WSP_POLA17][WSP_POLA15] = PUSTE_POLE;
		plansza[WSP_POLA4][WSP_POLA16] = plansza[WSP_POLA6][WSP_POLA16] = plansza[WSP_POLA10][WSP_POLA16] = plansza[WSP_POLA12][WSP_POLA16] = plansza[WSP_POLA14][WSP_POLA16] = PRZESZKODA;
		for (int i = WSP_POLA6; i < WSP_POLA19; i += ZWIEKSZAJ_O_2) plansza[i][WSP_POLA17] = PRZESZKODA;
		plansza[WSP_POLA2][WSP_POLA17] = plansza[WSP_POLA17][WSP_POLA17] = PRZESZKODA;
		plansza[WSP_POLA2][WSP_POLA18] = plansza[WSP_POLA3][WSP_POLA18] = plansza[WSP_POLA4][WSP_POLA18] = plansza[WSP_POLA6][WSP_POLA18] = plansza[WSP_POLA8][WSP_POLA18] = plansza[WSP_POLA10][WSP_POLA18] = plansza[WSP_POLA12][WSP_POLA18] = plansza[WSP_POLA18][WSP_POLA18] = PRZESZKODA;
		plansza[WSP_POLA10][WSP_POLA19] = plansza[WSP_POLA14][WSP_POLA19] = plansza[WSP_POLA15][WSP_POLA19] = plansza[WSP_POLA16][WSP_POLA19] = PRZESZKODA;
	}
	else if (los == WYLOS_3) {
		for (int i = WSP_POLA4; i < WSP_POLA8; i++) plansza[i][WSP_POLA1] = PRZESZKODA;
		plansza[WSP_POLA2][WSP_POLA2] = plansza[WSP_POLA9][WSP_POLA2] = PRZESZKODA;
		for (int i = WSP_POLA11; i < WSP_POLA19; i++) plansza[i][WSP_POLA2] = PRZESZKODA;
		plansza[WSP_POLA14][WSP_POLA2] = PUSTE_POLE;
		for (int i = WSP_POLA2; i < WSP_POLA12; i++) plansza[i][WSP_POLA3] = PRZESZKODA;
		plansza[WSP_POLA5][WSP_POLA3] = plansza[WSP_POLA10][WSP_POLA3] = PUSTE_POLE;
		plansza[WSP_POLA18][WSP_POLA3] = PRZESZKODA;
		for (int i = WSP_POLA13; i < WSP_POLA19; i++) plansza[i][WSP_POLA4] = PRZESZKODA;
		plansza[WSP_POLA17][WSP_POLA4] = PUSTE_POLE;
		for (int i = WSP_POLA2; i < WSP_POLA14; i++) plansza[i][WSP_POLA5] = PRZESZKODA;
		plansza[WSP_POLA7][WSP_POLA5] = plansza[WSP_POLA11][WSP_POLA5] = PUSTE_POLE;
		plansza[WSP_POLA18][WSP_POLA5] = PRZESZKODA;
		for (int i = WSP_POLA4; i < WSP_POLA17; i += ZWIEKSZAJ_O_4) plansza[i][WSP_POLA6] = PRZESZKODA;
		plansza[WSP_POLA15][WSP_POLA6] = plansza[WSP_POLA18][WSP_POLA6] = PRZESZKODA;
		for (int i = WSP_POLA1; i < WSP_POLA19; i++) plansza[i][WSP_POLA7] = PRZESZKODA;
		plansza[WSP_POLA3][WSP_POLA7] = plansza[WSP_POLA5][WSP_POLA7] = plansza[WSP_POLA7][WSP_POLA7] = plansza[WSP_POLA9][WSP_POLA7] = plansza[WSP_POLA11][WSP_POLA7] = plansza[WSP_POLA12][WSP_POLA7] = plansza[WSP_POLA13][WSP_POLA7] = plansza[WSP_POLA17][WSP_POLA7] = PUSTE_POLE;
		plansza[WSP_POLA4][WSP_POLA8] = plansza[WSP_POLA6][WSP_POLA8] = PRZESZKODA;
		for (int i = WSP_POLA10; i < WSP_POLA16; i++) plansza[i][WSP_POLA8] = PRZESZKODA;
		for (int i = WSP_POLA2; i < WSP_POLA9; i += ZWIEKSZAJ_O_2) plansza[i][WSP_POLA9] = PRZESZKODA;
		plansza[WSP_POLA17][WSP_POLA9] = plansza[WSP_POLA18][WSP_POLA9] = PRZESZKODA;
		plansza[WSP_POLA2][WSP_POLA10] = plansza[WSP_POLA4][WSP_POLA10] = plansza[WSP_POLA15][WSP_POLA10] = plansza[WSP_POLA17][WSP_POLA10] = plansza[WSP_POLA18][WSP_POLA10] = PRZESZKODA;
		for (int i = WSP_POLA8; i < WSP_POLA14; i++) plansza[i][WSP_POLA10] = PRZESZKODA;
		for (int i = WSP_POLA2; i < WSP_POLA7; i += ZWIEKSZAJ_O_2) plansza[i][WSP_POLA11] = PRZESZKODA;
		plansza[WSP_POLA15][WSP_POLA11] = plansza[WSP_POLA17][WSP_POLA11] = plansza[WSP_POLA18][WSP_POLA11] = PRZESZKODA;
		plansza[WSP_POLA2][WSP_POLA12] = plansza[WSP_POLA15][WSP_POLA12] = plansza[WSP_POLA17][WSP_POLA12] = plansza[WSP_POLA18][WSP_POLA12] = PRZESZKODA;
		for (int i = WSP_POLA6; i < WSP_POLA14; i++) plansza[i][WSP_POLA12] = PRZESZKODA;
		plansza[WSP_POLA4][WSP_POLA13] = plansza[WSP_POLA12][WSP_POLA13] = plansza[WSP_POLA15][WSP_POLA13] = PRZESZKODA;
		for (int i = WSP_POLA2; i < WSP_POLA20; i++) plansza[i][WSP_POLA14] = PRZESZKODA;
		plansza[WSP_POLA5][WSP_POLA14] = plansza[WSP_POLA9][WSP_POLA14] = plansza[WSP_POLA11][WSP_POLA14] = plansza[WSP_POLA13][WSP_POLA14] = plansza[WSP_POLA16][WSP_POLA14] = PUSTE_POLE;
		plansza[WSP_POLA2][WSP_POLA15] = plansza[WSP_POLA10][WSP_POLA15] = plansza[WSP_POLA14][WSP_POLA15] = PRZESZKODA;
		for (int i = WSP_POLA2; i < WSP_POLA19; i++) plansza[i][WSP_POLA16] = PRZESZKODA;
		plansza[WSP_POLA3][WSP_POLA16] = plansza[WSP_POLA7][WSP_POLA16] = plansza[WSP_POLA9][WSP_POLA16] = plansza[WSP_POLA11][WSP_POLA16] = plansza[WSP_POLA13][WSP_POLA16] = plansza[WSP_POLA15][WSP_POLA16] = plansza[WSP_POLA17][WSP_POLA16] = PUSTE_POLE;
		for (int i = WSP_POLA2; i < WSP_POLA19; i += ZWIEKSZAJ_O_2) plansza[i][WSP_POLA17] = PRZESZKODA;
		plansza[WSP_POLA6][WSP_POLA17] = plansza[WSP_POLA14][WSP_POLA17] = PUSTE_POLE;
		for (int i = WSP_POLA2; i < WSP_POLA19; i += ZWIEKSZAJ_O_2) plansza[i][WSP_POLA18] = PRZESZKODA;
		plansza[WSP_POLA10][WSP_POLA18] = PUSTE_POLE;
		plansza[WSP_POLA13][WSP_POLA18] = PRZESZKODA;
		plansza[WSP_POLA6][WSP_POLA19] = plansza[WSP_POLA10][WSP_POLA19] = PRZESZKODA;
	}
	else {
		plansza[WSP_POLA7][WSP_POLA1] = plansza[WSP_POLA8][WSP_POLA1] = plansza[WSP_POLA16][WSP_POLA1] = PRZESZKODA;
		for (int i = WSP_POLA2; i < WSP_POLA6; i++) plansza[i][WSP_POLA2] = PRZESZKODA;
		for (int i = WSP_POLA10; i < WSP_POLA19; i++) plansza[i][WSP_POLA2] = PRZESZKODA;
		plansza[WSP_POLA3][WSP_POLA2] = plansza[WSP_POLA11][WSP_POLA2] = plansza[WSP_POLA15][WSP_POLA2] = plansza[WSP_POLA17][WSP_POLA2] = PUSTE_POLE;
		plansza[WSP_POLA2][WSP_POLA3] = plansza[WSP_POLA7][WSP_POLA3] = plansza[WSP_POLA9][WSP_POLA3] = plansza[WSP_POLA10][WSP_POLA3] = plansza[WSP_POLA14][WSP_POLA3] = plansza[WSP_POLA18][WSP_POLA3] = PRZESZKODA;
		for (int i = WSP_POLA2; i < WSP_POLA6; i++) plansza[i][WSP_POLA4] = PRZESZKODA;
		for (int i = WSP_POLA14; i < WSP_POLA19; i++) plansza[i][WSP_POLA4] = PRZESZKODA;
		plansza[WSP_POLA7][WSP_POLA4] = plansza[WSP_POLA9][WSP_POLA4] = plansza[WSP_POLA12][WSP_POLA4] = PRZESZKODA;
		plansza[WSP_POLA7][WSP_POLA5] = plansza[WSP_POLA11][WSP_POLA5] = plansza[WSP_POLA12][WSP_POLA5] = plansza[WSP_POLA16][WSP_POLA5] = PRZESZKODA;
		for (int i = WSP_POLA2; i < WSP_POLA6; i++) plansza[i][WSP_POLA6] = PRZESZKODA;
		plansza[WSP_POLA7][WSP_POLA6] = plansza[WSP_POLA9][WSP_POLA6] = plansza[WSP_POLA12][WSP_POLA6] = plansza[WSP_POLA14][WSP_POLA6] = plansza[WSP_POLA18][WSP_POLA6] = PRZESZKODA;
		plansza[WSP_POLA5][WSP_POLA7] = plansza[WSP_POLA7][WSP_POLA7] = plansza[WSP_POLA9][WSP_POLA7] = plansza[WSP_POLA10][WSP_POLA7] = plansza[WSP_POLA14][WSP_POLA7] = plansza[WSP_POLA16][WSP_POLA7] = plansza[WSP_POLA17][WSP_POLA7] = plansza[WSP_POLA18][WSP_POLA7] = PRZESZKODA;
		for (int i = WSP_POLA1; i < WSP_POLA13; i++) plansza[i][WSP_POLA8] = PRZESZKODA;
		plansza[WSP_POLA4][WSP_POLA8] = plansza[WSP_POLA5][WSP_POLA8] = plansza[6][WSP_POLA8] = plansza[WSP_POLA8][WSP_POLA8] = plansza[WSP_POLA9][WSP_POLA8] = PUSTE_POLE;
		plansza[WSP_POLA16][WSP_POLA8] = PRZESZKODA;
		plansza[WSP_POLA5][WSP_POLA9] = plansza[WSP_POLA7][WSP_POLA9] = plansza[WSP_POLA8][WSP_POLA9] = plansza[WSP_POLA10][WSP_POLA9] = plansza[WSP_POLA14][WSP_POLA9] = plansza[WSP_POLA16][WSP_POLA9] = plansza[WSP_POLA18][WSP_POLA9] = PRZESZKODA;
		for (int i = WSP_POLA2; i < WSP_POLA6; i++) plansza[i][WSP_POLA10] = PRZESZKODA;
		plansza[WSP_POLA12][WSP_POLA10] = plansza[WSP_POLA14][WSP_POLA10] = plansza[WSP_POLA18][WSP_POLA10] = PRZESZKODA;
		plansza[WSP_POLA4][WSP_POLA11] = plansza[WSP_POLA5][WSP_POLA11] = plansza[WSP_POLA7][WSP_POLA11] = PRZESZKODA;
		for (int i = WSP_POLA8; i < WSP_POLA19; i += ZWIEKSZAJ_O_2) plansza[i][WSP_POLA11] = PRZESZKODA;
		plansza[WSP_POLA1][WSP_POLA12] = plansza[WSP_POLA2][WSP_POLA12] = plansza[WSP_POLA4][WSP_POLA12] = PRZESZKODA;
		for (int i = WSP_POLA8; i < WSP_POLA19; i += ZWIEKSZAJ_O_2) plansza[i][WSP_POLA12] = PRZESZKODA;
		plansza[WSP_POLA14][WSP_POLA12] = PUSTE_POLE;
		for (int i = WSP_POLA4; i < WSP_POLA19; i += ZWIEKSZAJ_O_2) plansza[i][WSP_POLA13] = PRZESZKODA;
		plansza[WSP_POLA15][WSP_POLA13] = PRZESZKODA;
		for (int i = WSP_POLA2; i < WSP_POLA11; i++) plansza[i][WSP_POLA14] = PRZESZKODA;
		plansza[WSP_POLA5][WSP_POLA14] = plansza[WSP_POLA7][WSP_POLA14] = plansza[WSP_POLA9][WSP_POLA14] = PUSTE_POLE;
		plansza[WSP_POLA14][WSP_POLA14] = PRZESZKODA;
		plansza[WSP_POLA4][WSP_POLA15] = plansza[WSP_POLA6][WSP_POLA15] = plansza[WSP_POLA8][WSP_POLA15] = plansza[WSP_POLA12][WSP_POLA15] = plansza[WSP_POLA14][WSP_POLA15] = PRZESZKODA;
		for (int i = WSP_POLA16; i < WSP_POLA20; i++) plansza[i][WSP_POLA15] = PRZESZKODA;
		plansza[WSP_POLA19][WSP_POLA15] = PUSTE_POLE;
		for (int i = WSP_POLA2; i < WSP_POLA17; i += ZWIEKSZAJ_O_2) plansza[i][WSP_POLA16] = PRZESZKODA;
		plansza[WSP_POLA11][WSP_POLA16] = PRZESZKODA;
		plansza[WSP_POLA2][WSP_POLA17] = plansza[WSP_POLA8][WSP_POLA17] = plansza[WSP_POLA12][WSP_POLA17] = plansza[WSP_POLA16][WSP_POLA17] = plansza[WSP_POLA18][WSP_POLA17] = PRZESZKODA;
		for (int i = WSP_POLA2; i < WSP_POLA19; i++) plansza[i][WSP_POLA18] = PRZESZKODA;
		plansza[WSP_POLA3][WSP_POLA18] = plansza[WSP_POLA9][WSP_POLA18] = plansza[WSP_POLA11][WSP_POLA18] = plansza[WSP_POLA13][WSP_POLA18] = plansza[WSP_POLA15][WSP_POLA18] = plansza[WSP_POLA17][WSP_POLA18] = PUSTE_POLE;
		plansza[WSP_POLA10][WSP_POLA19] = plansza[WSP_POLA14][WSP_POLA19] = PRZESZKODA;
	}
	//ustawienie postaci na planszy gry
	plansza[WSP_POLA1][WSP_POLA1] = POLICJANT1;
	plansza[WSP_POLA19][WSP_POLA19] = POLICJANT2;
	plansza[WSP_POLA10][WSP_POLA10] = ZLODZIEJ;
}

/*
  metoda rysująca graficzną postać planszy
  przyjmuje argumenty:
  -> cel - przechowuje informację o tym, co ma narysować
  -> stany - ustawia wygląd poszczególnych pól
*/
void Plansza::draw(sf::RenderTarget& cel, sf::RenderStates stany) const {
	stany.transform *= getTransform();
	stany.texture = &tekstura;
	cel.draw(wierzcholki, stany);
}

/*
  metoda tworzy graficzną postać planszy w oparciu o planszę zapisaną w pamięci komputera i o wygląd poszczególnych pól tej planszy
  przyjmuje argumenty:
  -> plan - plik przechowujący wygląd pól planszy
  -> rozmiar - wektor przechowujący graficzną wersję planszy
*/
bool Plansza::zaladuj(const std::string& plan, sf::Vector2u rozmiar) {
	if (!tekstura.loadFromFile(plan)) return false;

	wierzcholki.setPrimitiveType(sf::Quads);
	wierzcholki.resize(ROZMIAR_PLANSZY * ROZMIAR_PLANSZY * IL_WIERZCHOLKOW);

	//utworzenie graficznej postaci planszy
	for (int i = WART_POCZAT; i < ROZMIAR_PLANSZY; i++) {
		for (int j = WART_POCZAT; j < ROZMIAR_PLANSZY; j++) {
			int numer = plansza[i][j];   //numer - przechowuje informacje o zajętości pola, na podstawie której wybiera jego wygląd

			int tu = numer % (tekstura.getSize().x / rozmiar.x);   //tu - pozycja pola w teksturze (kolumna)
			int tv = numer / (tekstura.getSize().x / rozmiar.x);   //tv - pozycja pola w teksturze (rzad)

			sf::Vertex* plansza = &wierzcholki[(i + j * ROZMIAR_PLANSZY) * IL_WIERZCHOLKOW];   //graficzna reprezentacja planszy

			plansza[WIERZCH0].position = sf::Vector2f(i * rozmiar.x, j * rozmiar.y);
			plansza[WIERZCH1].position = sf::Vector2f((i + ZWIEKSZ_O_1) * rozmiar.x, j * rozmiar.y);
			plansza[WIERZCH2].position = sf::Vector2f((i + ZWIEKSZ_O_1) * rozmiar.x, (j + ZWIEKSZ_O_1) * rozmiar.y);
			plansza[WIERZCH3].position = sf::Vector2f(i * rozmiar.x, (j + ZWIEKSZ_O_1) * rozmiar.y);

			plansza[WIERZCH0].texCoords = sf::Vector2f(tu * rozmiar.x, tv * rozmiar.y);
			plansza[WIERZCH1].texCoords = sf::Vector2f((tu + ZWIEKSZ_O_1) * rozmiar.x, tv * rozmiar.y);
			plansza[WIERZCH2].texCoords = sf::Vector2f((tu + ZWIEKSZ_O_1) * rozmiar.x, (tv + ZWIEKSZ_O_1) * rozmiar.y);
			plansza[WIERZCH3].texCoords = sf::Vector2f(tu * rozmiar.x, (tv + ZWIEKSZ_O_1) * rozmiar.y);
		}
	}

	return true;
}
