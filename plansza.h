#ifndef PLANSZA_H
#define PLANSZA_H
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>
#include "policjant.h"

#define ROZMIAR_PLANSZY 21   //rozmiar planszy do gry

//klasa odpowiedzialna za utworzenie planszy
class Plansza : public sf::Drawable, public sf::Transformable {
	int plansza[ROZMIAR_PLANSZY][ROZMIAR_PLANSZY];                  //tablica reprezentująca planszę w pamięci komputera
	sf::Texture tekstura;                                           //pole przechowujące wygląd poszczególnych pól planszy
	sf::VertexArray wierzcholki;                                    //przechowuje graficzną postać wierzchołków pól na planszy
	virtual void draw(sf::RenderTarget&, sf::RenderStates) const;   //metoda rysująca graficzną postać planszy
public:
	Plansza();                                                      //konstruktor budujący planszę w pamięci komputera
	bool zaladuj(const std::string&, sf::Vector2u);                 //metoda tworzy graficzną postać planszy w oparciu o planszę zapisaną w pamięci komputera i o wygląd poszczególnych pól tej planszy
	friend class Policjant;
	friend class Zlodziej;
};

#endif
