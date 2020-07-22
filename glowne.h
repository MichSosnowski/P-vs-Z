#ifndef GLOWNE_H
#define GLOWNE_H
#include <QApplication>
#include <QFile>
#include <QWidget>
#include <QPixmap>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QMessageBox>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QTextCodec>
#include <SFML/Graphics.hpp>

/*
  klasa odpowiedzialna za utworzenie okna programu
  i dokonywanie w nim różnych zmian po naciśnięciu
  konkretnych przycisków
*/
class Glowne : public QWidget {

	Q_OBJECT

	int jezyk;                     //przechowuje informację o wybranym języku programu (domyślnie język polski)
	int sterowanie;                //przechowuje informację o wybranym sterowaniu postaciami (domyślnie klawisze WSAD)
	QWidget okno;                  //reprezentuje główne okno programu
	QLabel* projekt;               //reprezentuje okno powitalne informujące o celu tworzenia programu
	QPixmap* logo;                 //reprezentuje logo UKSW na ekranie powitalnym programu
	QLabel* autor;                 //przechowuje informacje o autorze programu wyświetlane na ekranie powitalnym
	QLabel* obraz;                 //ustawia logo UKSW na ekranie powitalnym programu
	QWidget* strona1;              //przechowuje wszelkie informacje o ekranie powitalnym programu
	QWidget* strona2;              //przechowuje wszelkie informacje o menu głównym programu
	QWidget* strona3;              //przechowuje wszelkie informacje o oknie START
	QWidget* strona4;              //przechowuje wszelkie informacje o oknie ZASADY
	QWidget* strona5;              //przechowuje wszelkie informacje o oknie USTAWIENIA
	QLabel* wersja;                //przechowuje numer wersji oprogramowania wyświetlany w menu głównym programu
	QLabel* tekst;                 //przechowuje tytuł gry wyświetlany w menu głównym programu
	QLabel* tekst2;                //przechowuje tekst wyświetlany w oknie START
	QLabel* tekst3;                //przechowuje tekst wyświetlany w oknie ZASADY
	QPushButton* dalej;            //przechowuje informacje o przycisku DALEJ wyświetlanym na ekranie powitalnym programu
	QPushButton* start;            //przechowuje informacje o przycisku START wyświetlanym w menu głównym programu
	QPushButton* zasady;           //przechowuje informacje o przycisku ZASADY wyświetlanym w menu głównym programu
	QPushButton* opcje;            //przechowuje informacje o przycisku USTAWIENIA wyświetlanym w menu głównym programu
	QPushButton* wyjdz;            //przechowuje informacje o przycisku WYJDŹ wyświetlanym w menu głównym programu
	QPushButton* policjanci;       //przechowuje informacje o przycisku POLICJANCI wyświetlanym w oknie START
	QPushButton* zlodziej;         //przechowuje informacje o przycisku ZŁODZIEJ wyświetlanym w oknie START
	QLabel* opcja;                 //przechowuje nazwę pierwszego z ustawień dostępnych w oknie USTAWIENIA (język)
	QLabel* opcja2;                //przechowuje nazwę drugiego z ustawień dostępnych w oknie USTAWIENIA (sterowanie)
	QComboBox* lista;              //przechowuje opcje możliwe do wyboru w ustawieniach języka w oknie USTAWIENIA
	QComboBox* lista2;             //przechowuje opcje możliwe do wyboru w ustawieniach sterowania w oknie USTAWIENIA
	QHBoxLayout* ustaw;            //ustawia wygląd opcji język w oknie USTAWIENIA (najpierw nazwa opcji, a potem jej wybór)
	QHBoxLayout* ustaw2;           //ustawia wygląd opcji sterowania w oknie USTAWIENIA (najpierw nazwa opcji, a potem jej wybór)
	QPushButton* powrot;           //przechowuje informacje o przycisku POWRÓT wyświetlanym w oknie START
	QPushButton* powrot2;          //przechowuje informacje o przycisku POWRÓT wyświetlanym w oknie ZASADY
	QPushButton* powrot3;          //przechowuje informacje o przycisku POWRÓT wyświetlanym w oknie USTAWIENIA
	QStackedWidget* okna;          //wiąże wszystkie okna programu w jedno okno prezentowane użytkownikowi
	QVBoxLayout* warstwa1;         //przechowuje wygląd ekranu powitalnego programu
	QVBoxLayout* warstwa2;         //przechowuje wygląd menu głównego programu
	QVBoxLayout* warstwa3;         //przechowuje wygląd okna START
	QVBoxLayout* warstwa4;         //przechowuje wygląd okna ZASADY
	QVBoxLayout* warstwa5;         //przechowuje wygląd okna USTAWIENIA
	QLabel* zapelniacz;            //poprawia wygląd okna START
public:
	Glowne();                                                //konstruktor tworzący główne okno programu i wyświetlający to okno po jego utworzeniu
	~Glowne();                                               //destruktor niszczący główne okno programu
	void sprawdz_pliki(QString);                             //metoda sprawdzająca, czy istnieją konkretne pliki graficzne wymagane do poprawnego uruchomienia programu
	void uruchom_gre(int, int);                              //metoda uruchamiająca grę w zależności od wybranego trybu gry i wybranej wersji językowej
	void zakoncz_gre(sf::RenderWindow&, int&, int&, int&);   //metoda kończąca grę i informująca o jej zakończeniu
public slots:
	void wyswietl_glowne();                                  //metoda wyświetlająca menu główne gry
	void wyswietl_start();                                   //metoda wyświetlająca okno startowe gry z wyborem postaci do grania
	void wyswietl_zasady();                                  //metoda wyświetlająca okno Zasady informujące o zasadach gry
	void wyswietl_opcje();                                   //metoda wyświetlająca okno Ustawienia umożilwiające zmianę ustawień gry
	void uruchom_gre_pol();                                  //metoda wywołująca metodę uruchom_gre(int, int) w wersji, gdy gracz gra policjantami
	void uruchom_gre_zl();                                   //metoda wywołująca metodę uruchom_gre(int, int) w wersji, gdy gracz gra złodziejem
};

#endif
