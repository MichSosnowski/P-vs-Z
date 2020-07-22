#ifndef ZLODZIEJ_H
#define ZLODZIEJ_H
#include "plansza.h"
#include <cmath>

#define IL_WSPOLRZEDNE 2     //ilość współrzędnych określających położenie na planszy
#define ROZMIAR_PLANSZY 21   //rozmiar planszy do gry

class Plansza;
class Policjant;

//struktura reprezentująca właściwości komórki (pola) planszy, które cechują to pole dla algorytmu A*
//struktura komorka należy do algorytmu A* ze strony https://www.geeksforgeeks.org/a-search-algorithm/
struct komorka {
	int rodzic_i, rodzic_j;   //współrzędne pola poprzedzającego dane pole zgodnie z algorytmem A*
	double f, g, h;           //wartości funkcji odległości stosowane w algorytmie A* (f - suma wartości funkcji g i h, g - odległość w kratkach, h - funkcja heurytstyczna)
};

//klasa odpowiedzialna za postać złodzieja
class Zlodziej {
	const int numer;                                           //numer reprezentujący postać złodzieja na planszy
	int miejsce[IL_WSPOLRZEDNE];                               //współrzędne złodzieja na planszy
	int ruchy;                                                 //ilość dokonanych przez złodzieja ruchów
public:
	Zlodziej(int, int, int);                                   //konstruktor tworzący postać złodzieja poprzez inicjalizację powyższych pól klasy
	void porusz(Plansza&, char, int&, int&);                   //metoda odpowiedzialna za zmianę wizualną położenia złodzieja na planszy
	int wybierz_ruch(Plansza&, Policjant&, Policjant&);        //metoda odpowiedzialna za wybranie ruchu przez złodzieja w przypadku, gdy złodziejem steruje komputer
	int dokonaj_ruch(Plansza&, Policjant&, int);               //metoda obliczająca najkrótszą drogę złodzieja od obu policjantów; algorytm A*: https://www.geeksforgeeks.org/a-search-algorithm/
	bool jest_prawidlowy(int, int);                            //metoda sprawdza, czy postać nie wychodzi poza planszę; algorytm A*: https://www.geeksforgeeks.org/a-search-algorithm/
	bool jest_cel(int, int, int, int);                         //metoda sprawdza, czy na sąsiednim polu złodzieja stoi policjant; algorytm A*: https://www.geeksforgeeks.org/a-search-algorithm/
	int wyznacz_trase(komorka**, int, int, Plansza&);          //metoda wyznacza najkrótsze trasy do policjantów i podaje ich długości w kratkach; algorytm A*: https://www.geeksforgeeks.org/a-search-algorithm/
	bool jest_odblokowane(int[][ROZMIAR_PLANSZY], int, int);   //metoda sprawdza, czy sąsiednie pole jest puste (można na nie wejść); algorytm A*: https://www.geeksforgeeks.org/a-search-algorithm/
	double policz_h(int, int, int, int);                       //metoda licząca funkcję heurystyczną (h) w algorytmie A*: https://www.geeksforgeeks.org/a-search-algorithm/
	void ustaw_ponownie(Plansza&, int);                        //metoda ustawia początkowe położenie złodzieja po serii testów, które służyły do znalezienia najlepszego ruchu oddalającego od obu policjantów
	friend Policjant;
};

#endif
