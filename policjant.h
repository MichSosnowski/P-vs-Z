#ifndef POLICJANT_H
#define POLICJANT_H
#include "plansza.h"
#include "zlodziej.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <stack>
#include <set>

#define IL_WSPOLRZEDNE 2     //ilość współrzędnych określających położenie na planszy
#define ROZMIAR_PLANSZY 21   //rozmiar planszy do gry

struct komorka;
class Plansza;
class Zlodziej;

//klasa odpowiedzialna za postać policjanta
class Policjant {
	const int numer;                                           //numer reprezentujący policjanta na planszy
	int miejsce[IL_WSPOLRZEDNE];                               //współrzędne położenia policjanta na planszy
public:
	Policjant(int, int, int);                                  //konstruktor tworzący postać policjanta poprzez inicjalizację powyższych pól klasy
	void porusz(Plansza&, char, int&, Zlodziej&, int&);        //metoda odpowiedzialna za zmianę wizualną położenia policjanta na planszy
	int dokonaj_ruch(Plansza&, Zlodziej&);                     //metoda obliczająca najkrótszą drogę policjanta do złodzieja; algorytm A*: https://www.geeksforgeeks.org/a-search-algorithm/
	bool jest_prawidlowy(int, int);                            //metoda sprawdza, czy postać nie wychodzi poza planszę; algorytm A*: https://www.geeksforgeeks.org/a-search-algorithm/
	bool jest_cel(int, int, int, int);                         //metoda sprawdza, czy na sąsiednim polu policjanta stoi złodziej; algorytm A*: https://www.geeksforgeeks.org/a-search-algorithm/
	int wyznacz_trase(komorka**, int, int, Plansza&);          //metoda wyznacza najkrótszą trasę policjanta do złodzieja i podaje, w którą stronę powinien ruszyć się policjant; algorytm A*: https://www.geeksforgeeks.org/a-search-algorithm/
	bool jest_odblokowane(int[][ROZMIAR_PLANSZY], int, int);   //metoda sprawdza, czy sąsiednie pole jest puste (można na nie wejść); algorytm A*: https://www.geeksforgeeks.org/a-search-algorithm/
	double policz_h(int, int, int, int);                       //metoda licząca funkcję heurystyczną (h) w algorytmie A*: https://www.geeksforgeeks.org/a-search-algorithm/
	friend Zlodziej;
};

#endif
