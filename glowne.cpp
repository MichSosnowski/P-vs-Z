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

#include "glowne.h"
#include "plansza.h"
#include "policjant.h"

#define WART_POCZAT 0           //wartość początkowa nadawana niektórym zmiennym w programie
#define SZER_OKNA 616           //szerokość głównego okna programu podana w pikselach
#define WYS_OKNA 433            //wysokość głównego okna programu podana w pikselach
#define SZER_PRZYCISK 150       //szerokość przycisków używanych w programie podana w pikselach
#define WYS_PRZYCISK 55         //wysokość przycisków używanych w programie podana w pikselach
#define WYS_PRZYCISK_DALEJ 30   //wysokość przycisku DALEJ używanego w oknie początkowym programu podana w pikselach
#define SZER_LISTY 400          //szerokość list rozwijanych używanych w ustawieniach
#define WYS_LISTY 30            //wysokość list rozwijanych używanych w ustawieniach
#define ROZMIAR_CZCIONKI 12     //rozmiar czcionki wykorzystywanej do wypisywania tekstów w oknach programu
#define ROZM_CZCIONKI 8         //rozmiar czcionki wykorzystywanej do wypisywania tekstów w oknach programu
#define ATRYBUT 1               //atrybut pozwalający na wyświetlenie odpowiednich części okien programu
#define TRYB0 0                 //stała informująca o aktualnym trybie gry (gracz gra policjantami)
#define TRYB1 1                 //stała informująca o aktualnym trybie gry (gracz gra złodziejem)
#define WERSJA0 0               //stała informująca o aktualnej wersji językowej programu (język polski)
#define WERSJA1 1               //stała informująca o aktualnej wersji językowej programu (język angielski)
#define ROZMIAR_OKNA 672        //rozmiar okna gry prezentującego planszę
#define ROZMIAR_IKONY 100       //rozmiar ikony pojawiającej się na pasku tytułowym okna gry prezentującego planszę
#define ROZMIAR_WEKTOR 32       //rozmiar kontenera wektor służącego do przechowywania graficznej postaci planszy
#define WSPOL_POL1 1            //początkowa współrzędna położenia pierwszego policjanta na planszy
#define WSPOL_POL2 19           //początkowa współrzędna położenia drugiego policjanta na planszy
#define WSPOL_ZLO 10            //początkowa współrzędna położenia złodzieja na planszy

enum { STR_OKNA1 = 1, STR_OKNA2 = 2, STR_OKNA3 = 3, STR_OKNA4 = 4 };   //stałe reprezentujące strony okna głównego programu, które są wyświetlane po naciśnięciu odpowiedniego przycisku w menu głównym

enum { POL1 = 2, POL2 = 3, ZLO = 4 };                                  //stałe reprezentujące postaci policjantów i złodzieja na planszy

enum { KONIEC1 = 1, KONIEC2 = 2, KONIEC3 = 3 };                        //stałe informujące o rodzaju zakończenia gry

enum { RUCH1 = 1, RUCH2 = 2, RUCH3 = 3 };                              //stałe informujące o tym, czyja jest teraz kolej na ruch

enum { GORA = 'W', DOL = 'S', LEWO = 'A', PRAWO = 'D' };               //stałe określające możliwe ruchy postaci

enum { LOS1 = 1, LOS2 = 2, LOS3 = 3 };                                 //najlepszy możliwy ruch, który może wykonać w tym momencie komputer grając policjantami lub złodziejem

//konstruktor tworzący główne okno programu i wyświetlający to okno po jego utworzeniu
Glowne::Glowne() : jezyk(WART_POCZAT), sterowanie(WART_POCZAT) {
	QTextCodec* kod = QTextCodec::codecForName("Windows-1250");   //kod - przechowuje informację o kodowaniu czcionki

	//sprawdzanie, czy ważne dla programu pliki istnieją
	try {
		sprawdz_pliki("ikona.ico");
		sprawdz_pliki("ikona.png");
		sprawdz_pliki("logo.png");
		sprawdz_pliki("plan.png");
		sprawdz_pliki("plan2.png");
	}
	catch (QString blad) {                                        //blad - przechowuje informacje o błędzie
		QByteArray napis = "Błąd!";                               //napis - przechowuje napis w języku polskim
		QString opis = kod->toUnicode(napis);                     //opis - przechowuje napis w odpowiednim kodowaniu
		napis.clear();
		napis = "Brak spójności plików.";
		napis += "\nProgram nie może zostać uruchomiony.";
		QString opis2 = kod->toUnicode(napis);                    //opis2 - przechowuje napis w odpowiednim kodowaniu
		QMessageBox krytyczny;                                    //krytyczny - tworzy okno o krytycznym błędzie
		krytyczny.setWindowTitle(opis);
		krytyczny.setText(opis2);
		krytyczny.setIcon(QMessageBox::Critical);
		krytyczny.setStandardButtons(QMessageBox::Ok);
		krytyczny.setWindowIcon(QIcon("ikona.png"));
		krytyczny.exec();
		exit(EXIT_FAILURE);
	}

	//tworzenie ekranu powitalnego programu
	QByteArray napis = "Policjanci vs złodziej";                  //napis - przechowuje napis w języku polskim
	QString opis = kod->toUnicode(napis);                         //opis - przechowuje napis w odpowiednim kodowaniu
	okno.resize(SZER_OKNA, WYS_OKNA);
	okno.setMinimumSize(SZER_OKNA, WYS_OKNA);
	okno.setMaximumSize(SZER_OKNA, WYS_OKNA);
	okno.setWindowTitle(opis);
	okno.setWindowIcon(QIcon("ikona.png"));

	okno.setStyleSheet("background-color: white");
	projekt = new QLabel(&okno);
	napis.clear();
	opis.clear();
	napis = "Projekt: Gra statyczna w Policjantów i złodzieja";
	opis = kod->toUnicode(napis);
	projekt->setText(opis);
	projekt->setFont(QFont("Times New Roman", ROZMIAR_CZCIONKI));

	logo = new QPixmap("logo.png");
	obraz = new QLabel(&okno);
	obraz->setPixmap(*logo);

	autor = new QLabel(&okno);
	napis.clear();
	opis.clear();
	napis = "Autor: Michał Sosnowski";
	opis = kod->toUnicode(napis);
	autor->setText(opis);
	autor->setFont(QFont("Times New Roman", ROZMIAR_CZCIONKI));

	dalej = new QPushButton("DALEJ", &okno);
	dalej->setFixedSize(SZER_PRZYCISK, WYS_PRZYCISK_DALEJ);
	dalej->setCursor(Qt::PointingHandCursor);
	okno.connect(dalej, &QPushButton::clicked, this, &Glowne::wyswietl_glowne);

	//tworzenie menu głównego programu
	tekst = new QLabel(&okno);
	napis.clear();
	opis.clear();
	napis = "\n\n\t\t       POLICJANCI  VS  ZŁODZIEJ\n\n";
	opis = kod->toUnicode(napis);
	tekst->setText(opis);
	tekst->setFont(QFont("Times New Roman", ROZMIAR_CZCIONKI));

	start = new QPushButton("START", &okno);
	start->setFixedSize(SZER_PRZYCISK, WYS_PRZYCISK);
	start->setCursor(Qt::PointingHandCursor);
	okno.connect(start, &QPushButton::clicked, this, &Glowne::wyswietl_start);

	zasady = new QPushButton("ZASADY", &okno);
	zasady->setFixedSize(SZER_PRZYCISK, WYS_PRZYCISK);
	zasady->setCursor(Qt::PointingHandCursor);
	okno.connect(zasady, &QPushButton::clicked, this, &Glowne::wyswietl_zasady);

	opcje = new QPushButton("USTAWIENIA", &okno);
	opcje->setFixedSize(SZER_PRZYCISK, WYS_PRZYCISK);
	opcje->setCursor(Qt::PointingHandCursor);
	okno.connect(opcje, &QPushButton::clicked, this, &Glowne::wyswietl_opcje);

	napis.clear();
	opis.clear();
	napis = "WYJDŹ";
	opis = kod->toUnicode(napis);
	wyjdz = new QPushButton(opis, &okno);
	wyjdz->setFixedSize(SZER_PRZYCISK, WYS_PRZYCISK);
	wyjdz->setCursor(Qt::PointingHandCursor);
	okno.connect(wyjdz, &QPushButton::clicked, qApp, &QApplication::quit);

	wersja = new QLabel(&okno);
	wersja->setText("\n\n\t\t\t\t\t\t\t\t\t\t\t\t\t      v 1.0.0.0");
	wersja->setFont(QFont("Times New Roman", ROZM_CZCIONKI));

	//tworzenie okna START
	napis.clear();
	napis = "\t\t   Wybierz postać, którą chcesz grać:\n";
	opis.clear();
	opis = kod->toUnicode(napis);
	tekst2 = new QLabel(opis, &okno);
	tekst2->setFont(QFont("Times New Roman", ROZMIAR_CZCIONKI));

	napis.clear();
	opis.clear();
	napis = "POLICJANCI";
	opis = kod->toUnicode(napis);
	policjanci = new QPushButton(opis, &okno);
	policjanci->setFixedSize(SZER_PRZYCISK, WYS_PRZYCISK);
	policjanci->setCursor(Qt::PointingHandCursor);
	okno.connect(policjanci, &QPushButton::clicked, this, &Glowne::uruchom_gre_pol);

	napis.clear();
	opis.clear();
	napis = "ZŁODZIEJ";
	opis = kod->toUnicode(napis);
	zlodziej = new QPushButton(opis, &okno);
	zlodziej->setFixedSize(SZER_PRZYCISK, WYS_PRZYCISK);
	zlodziej->setCursor(Qt::PointingHandCursor);
	okno.connect(zlodziej, &QPushButton::clicked, this, &Glowne::uruchom_gre_zl);

	opis.clear();
	opis = "\n\n";
	zapelniacz = new QLabel(opis, &okno);

	napis.clear();
	opis.clear();
	napis = "POWRÓT";
	opis = kod->toUnicode(napis);
	powrot = new QPushButton(opis, &okno);
	powrot->setFixedSize(SZER_PRZYCISK, WYS_PRZYCISK);
	powrot->setCursor(Qt::PointingHandCursor);
	okno.connect(powrot, &QPushButton::clicked, this, &Glowne::wyswietl_glowne);

	//tworzenie okna ZASADY
	napis.clear();
	napis = "Zasady:\n\n";
	napis += "Gra polega odpowiednio na poruszaniu się policjantami lub złodziejem.\n";
	napis += "Gracz dokonuje wyboru postaci, którymi będzie grał:\n- albo dwoma policjantami,\n- albo złodziejem.\n";
	napis += "Postać, której nie wybierze gracz, będzie sterowana przez komputer.\n";
	napis += "Zadaniem policjantów jest złapanie złodzieja, a celem złodzieja - ucieczka.\n";
	napis += "Złapanie złodzieja polega na otoczeniu go przez policjantów tak, aby\nnie mógł się poruszyć.\n";
	napis += "Złodziej ucieka, jeżeli nie został złapany przez policjantów przed\nwykonaniem 200. ruchu.\n";
	napis += "Gra się kończy, gdy policjanci złapią złodzieja lub gdy złodziej ucieknie.\n";
	opis.clear();
	opis = kod->toUnicode(napis);
	tekst3 = new QLabel(opis, &okno);
	tekst3->setFont(QFont("Times New Roman", ROZMIAR_CZCIONKI));

	napis.clear();
	opis.clear();
	napis = "POWRÓT";
	opis = kod->toUnicode(napis);
	powrot2 = new QPushButton(opis, &okno);
	powrot2->setFixedSize(SZER_PRZYCISK, WYS_PRZYCISK);
	powrot2->setCursor(Qt::PointingHandCursor);
	okno.connect(powrot2, &QPushButton::clicked, this, &Glowne::wyswietl_glowne);

	//tworzenie okna USTAWIENIA
	napis.clear();
	opis.clear();
	napis = "Język: ";
	opis = kod->toUnicode(napis);
	opcja = new QLabel(opis, &okno);
	QStringList jezyki = { "Polski", "English" };                    //jezyki - przechowuje języki do wyboru w liście
	lista = new QComboBox;
	lista->addItems(jezyki);
	lista->setFixedSize(SZER_LISTY, WYS_LISTY);
	ustaw = new QHBoxLayout;
	ustaw->addWidget(opcja);
	ustaw->addWidget(lista);
	okno.connect(lista, QOverload<int>::of(&QComboBox::activated), [=](int indeks) { jezyk = indeks; wyswietl_opcje(); });

	napis.clear();
	napis = "Sterowanie: ";
	opcja2 = new QLabel(napis, &okno);
	napis.clear();
	opis.clear();
	napis = "Strzałki";
	opis = kod->toUnicode(napis);
	QStringList ster = { "W, S, A, D", opis };                     //ster - przechowuje opcje sterowania do wyboru w liście
	lista2 = new QComboBox;
	lista2->addItems(ster);
	lista2->setFixedSize(SZER_LISTY, WYS_LISTY);
	ustaw2 = new QHBoxLayout;
	ustaw2->addWidget(opcja2);
	ustaw2->addWidget(lista2);
	okno.connect(lista2, QOverload<int>::of(&QComboBox::activated), [=](int indeks) { sterowanie = indeks; wyswietl_opcje(); });

	napis.clear();
	opis.clear();
	napis = "POWRÓT";
	opis = kod->toUnicode(napis);
	powrot3 = new QPushButton(opis, &okno);
	powrot3->setFixedSize(SZER_PRZYCISK, WYS_PRZYCISK);
	powrot3->setCursor(Qt::PointingHandCursor);
	okno.connect(powrot3, &QPushButton::clicked, this, &Glowne::wyswietl_glowne);

	//scalanie wszystkich utworzonych okien w jedno okno główne programu
	strona1 = new QWidget(&okno);
	warstwa1 = new QVBoxLayout(strona1);
	warstwa1->addWidget(projekt, ATRYBUT, Qt::AlignJustify);
	warstwa1->addWidget(obraz, ATRYBUT, Qt::AlignJustify);
	warstwa1->addWidget(autor, ATRYBUT, Qt::AlignJustify);
	warstwa1->addWidget(dalej, ATRYBUT, Qt::AlignRight);

	strona2 = new QWidget(&okno);
	warstwa2 = new QVBoxLayout(strona2);
	warstwa2->addWidget(tekst);
	warstwa2->addWidget(start, ATRYBUT, Qt::AlignJustify);
	warstwa2->addWidget(zasady, ATRYBUT, Qt::AlignJustify);
	warstwa2->addWidget(opcje, ATRYBUT, Qt::AlignJustify);
	warstwa2->addWidget(wyjdz, ATRYBUT, Qt::AlignJustify);
	warstwa2->addWidget(wersja);

	strona3 = new QWidget(&okno);
	warstwa3 = new QVBoxLayout(strona3);
	warstwa3->addWidget(tekst2);
	warstwa3->addWidget(policjanci, ATRYBUT, Qt::AlignJustify);
	warstwa3->addWidget(zlodziej, ATRYBUT, Qt::AlignJustify);
	warstwa3->addWidget(zapelniacz);
	warstwa3->addWidget(powrot, ATRYBUT, Qt::AlignRight);

	strona4 = new QWidget(&okno);
	warstwa4 = new QVBoxLayout(strona4);
	warstwa4->addWidget(tekst3);
	warstwa4->addWidget(powrot2, ATRYBUT, Qt::AlignRight);

	strona5 = new QWidget(&okno);
	warstwa5 = new QVBoxLayout(strona5);
	warstwa5->addLayout(ustaw);
	warstwa5->addLayout(ustaw2);
	warstwa5->addWidget(powrot3, ATRYBUT, Qt::AlignRight);

	okna = new QStackedWidget(&okno);
	okna->addWidget(strona1);
	okna->addWidget(strona2);
	okna->addWidget(strona3);
	okna->addWidget(strona4);
	okna->addWidget(strona5);

	//wyświetlenie okna głównego programu
	okno.show();
}

//destruktor niszczący główne okno programu
Glowne::~Glowne() {
	delete projekt;
	delete logo;
	delete autor;
	delete obraz;
	delete strona1;
	delete strona2;
	delete strona3;
	delete strona4;
	delete strona5;
	delete wersja;
	delete tekst;
	delete tekst2;
	delete tekst3;
	delete dalej;
	delete start;
	delete zasady;
	delete opcje;
	delete wyjdz;
	delete policjanci;
	delete zlodziej;
	delete opcja;
	delete opcja2;
	delete lista;
	delete lista2;
	delete ustaw;
	delete ustaw2;
	delete powrot;
	delete powrot2;
	delete powrot3;
	delete okna;
	delete warstwa1;
	delete warstwa2;
	delete warstwa3;
	delete warstwa4;
	delete warstwa5;
	delete zapelniacz;
}

/*
  metoda sprawdzająca, czy istnieją konkretne pliki graficzne wymagane do poprawnego uruchomienia programu
  przyjmuje argument:
  -> nazwa - nazwa pliku do sprawdzenia, czy istnieje
*/
inline void Glowne::sprawdz_pliki(QString nazwa) {
	if (!QFile(nazwa).exists()) throw nazwa;
}

//metoda wyświetlająca menu główne gry
void Glowne::wyswietl_glowne() {
	if (!jezyk) {
		//ustawia wersję polską menu głównego gry
		QTextCodec* kod = QTextCodec::codecForName("Windows-1250");   //kod - przechowuje informację o kodowaniu czcionki
		QByteArray napis = "Policjanci vs złodziej";                  //napis - przechowuje napis w języku polskim
		QString opis = kod->toUnicode(napis);                         //opis - przechowuje napis w odpowiednim kodowaniu czcionki
		okno.setStyleSheet("background-color: rgb(240, 240, 240)");
		okna->setCurrentIndex(STR_OKNA1);
		okno.setWindowTitle(opis);
		napis.clear();
		opis.clear();
		napis = "\n\n\t\t       POLICJANCI  VS  ZŁODZIEJ\n\n";
		opis = kod->toUnicode(napis);
		tekst->setText(opis);
		tekst->setFont(QFont("Times New Roman", ROZMIAR_CZCIONKI));
		zasady->setText("ZASADY");
		opcje->setText("USTAWIENIA");
		napis.clear();
		opis.clear();
		napis = "WYJDŹ";
		opis = kod->toUnicode(napis);
		wyjdz->setText(opis);
	}
	else {
		//ustawia wersję angielską menu głównego gry
		QString opis = "Policemen vs thief";                         //opis - przechowuje napis w języku angielskim
		okna->setCurrentIndex(STR_OKNA1);
		okno.setWindowTitle(opis);
		opis = "\n\n\t\t          POLICEMEN  VS  THIEF\n\n";
		tekst->setText(opis);
		tekst->setFont(QFont("Times New Roman", ROZMIAR_CZCIONKI));
		zasady->setText("RULES");
		opcje->setText("SETTINGS");
		wyjdz->setText("EXIT");
	}
}

//metoda wyświetlająca okno startowe gry z wyborem postaci do grania
void Glowne::wyswietl_start() {
	if (!jezyk) {
		//ustawia wersję polską okna startowego gry
		QTextCodec* kod = QTextCodec::codecForName("Windows-1250");   //kod - przechowuje informację o kodowaniu czcionki
		QByteArray napis = "Policjanci vs złodziej: Start";           //napis - przechowuje napis w języku polskim
		QString opis = kod->toUnicode(napis);                         //opis - przechowuje napis w odpowiednim kodowaniu czcionki
		okna->setCurrentIndex(STR_OKNA2);
		okno.setWindowTitle(opis);
		napis.clear();
		napis = "\t\t   Wybierz postać, którą chcesz grać:\n";
		opis.clear();
		opis = kod->toUnicode(napis);
		tekst2->setText(opis);
		napis.clear();
		opis.clear();
		napis = "POLICJANCI";
		opis = kod->toUnicode(napis);
		policjanci->setText(opis);
		napis.clear();
		opis.clear();
		napis = "ZŁODZIEJ";
		opis = kod->toUnicode(napis);
		zlodziej->setText(opis);
		napis.clear();
		opis.clear();
		napis = "POWRÓT";
		opis = kod->toUnicode(napis);
		powrot->setText(opis);
	}
	else {
		//ustawia wersję angielską okna startowego gry
		QString opis = "Policemen vs thief: Start";                        //opis - przechowuje napis w języku angielskim
		okna->setCurrentIndex(STR_OKNA2);
		okno.setWindowTitle(opis);
		opis.clear();
		opis = "\t             Choose a character who you want to play:\n";
		tekst2->setText(opis);
		policjanci->setText("POLICEMEN");
		zlodziej->setText("THIEF");
		powrot->setText("BACK");
	}
}

//metoda wyświetlająca okno Zasady informujące o zasadach gry
void Glowne::wyswietl_zasady() {
	if (!jezyk) {
		//ustawia wersję polską okna zasad gry
		QTextCodec* kod = QTextCodec::codecForName("Windows-1250");   //kod - przechowuje informację o kodowaniu czcionki
		QByteArray napis = "Policjanci vs złodziej: Zasady";          //napis - przechowuje napis w języku polskim
		QString opis = kod->toUnicode(napis);                         //opis - przechowuje napis w odpowiednim kodowaniu czcionki
		okna->setCurrentIndex(STR_OKNA3);
		okno.setWindowTitle(opis);
		napis.clear();
		napis = "Zasady:\n\n";
		napis += "Gra polega odpowiednio na poruszaniu się policjantami lub złodziejem.\n";
		napis += "Gracz dokonuje wyboru postaci, którymi będzie grał:\n- albo dwoma policjantami,\n- albo złodziejem.\n";
		napis += "Postać, której nie wybierze gracz, będzie sterowana przez komputer.\n";
		napis += "Zadaniem policjantów jest złapanie złodzieja, a celem złodzieja - ucieczka.\n";
		napis += "Złapanie złodzieja polega na otoczeniu go przez policjantów tak, aby\nnie mógł się poruszyć.\n";
		napis += "Złodziej ucieka, jeżeli nie został złapany przez policjantów przed\nwykonaniem 200. ruchu.\n";
		napis += "Gra się kończy, gdy policjanci złapią złodzieja lub gdy złodziej ucieknie.\n";
		opis.clear();
		opis = kod->toUnicode(napis);
		tekst3->setText(opis);
		napis.clear();
		opis.clear();
		napis = "POWRÓT";
		opis = kod->toUnicode(napis);
		powrot2->setText(opis);
	}
	else {
		//ustawia wersję angielską okna zasad gry
		QString opis = "Policemen vs thief: Rules";                 //opis - przechowuje napis w języku angielskim
		okna->setCurrentIndex(STR_OKNA3);
		okno.setWindowTitle(opis);
		opis.clear();
		opis = "Rules:\n\n";
		opis += "You move either policemen or a thief in this game.\n";
		opis += "First you choose a character who you want to play:\n- either two policemen,\n- or the thief.\n";
		opis += "The character who you will not choose will be controlled by the computer.\n";
		opis += "The policemen's task is to catch the thief and the thief's task is to escape.\n";
		opis += "The thief will be caught if he cannot move anywhere.\n";
		opis += "The thief will escape unless policemen catch him before his 200th turn.\n";
		opis += "The game finishes when either policemen catches the thief or the thief\nescapes.\n";
		tekst3->setText(opis);
		powrot2->setText("BACK");
	}
}

//metoda wyświetlająca okno Ustawienia umożilwiające zmianę ustawień gry
void Glowne::wyswietl_opcje() {
	if (!jezyk) {
		//ustawia wersję polską okna Ustawienia gry
		QTextCodec* kod = QTextCodec::codecForName("Windows-1250");   //kod - przechowuje informację o kodowaniu czcionki
		QByteArray napis = "Policjanci vs złodziej: Ustawienia";      //napis - przechowuje napis w języku polskim
		QString opis = kod->toUnicode(napis);                         //opis - przechowuje napis w odpowiednim kodowaniu czcionki
		okna->setCurrentIndex(STR_OKNA4);
		okno.setWindowTitle(opis);
		napis.clear();
		opis.clear();
		napis = "Język: ";
		opis = kod->toUnicode(napis);
		opcja->setText(opis);
		napis.clear();
		napis = "Sterowanie: ";
		opcja2->setText(napis);
		napis.clear();
		opis.clear();
		napis = "Strzałki";
		opis = kod->toUnicode(napis);
		QStringList ster = { "W, S, A, D", opis };                   //ster - lista możliwych opcji sterowania do wyboru
		lista2->clear();
		lista2->addItems(ster);
		if (!sterowanie) lista2->setCurrentText("W, S, A, D");
		else lista2->setCurrentText(opis);
		napis.clear();
		opis.clear();
		napis = "POWRÓT";
		opis = kod->toUnicode(napis);
		powrot3->setText(opis);
	}
	else {
		//ustawia wersję angielską okna Ustawienia gry
		QString opis = "Policemen vs thief: Settings";              //opis - przechowuje napis w języku angielskim
		okna->setCurrentIndex(STR_OKNA4);
		okno.setWindowTitle(opis);
		opcja->setText("Language: ");
		opcja2->setText("Control: ");
		QStringList ster = { "W, S, A, D", "Arrow Keys" };          //ster - lista możliwych opcji sterowania do wyboru
		lista2->clear();
		lista2->addItems(ster);
		if (!sterowanie) lista2->setCurrentText("W, S, A, D");
		else lista2->setCurrentText("Arrow Keys");
		powrot3->setText("BACK");
	}
}

//metoda wywołująca metodę uruchom_gre(int, int) w wersji, gdy gracz gra policjantami
void Glowne::uruchom_gre_pol() {
	QTextCodec* kod = QTextCodec::codecForName("Windows-1250");   //kod - przechowuje informację o kodowaniu czcionki
	QByteArray napis;                                             //napis - przechowuje napis
	QString opis, opis2;                                          //opis, opis2 - przechowują napisy w odpowiednim kodowaniu czcionki
	//wyświetlenie informacji o sterowaniu w języku polskim
	if (!jezyk) {
		napis = "Informacja";
		if (!sterowanie) {
			opis = kod->toUnicode(napis);
			napis.clear();
			napis = "Użyj klawiszy W, S, A, D,\naby wykonać ruch policjantami.";
			opis2 = kod->toUnicode(napis);
		}
		else {
			opis = kod->toUnicode(napis);
			napis.clear();
			napis = "Użyj strzałek,\naby wykonać ruch policjantami.";
			opis2 = kod->toUnicode(napis);
		}
		QMessageBox info;                                        //info - tworzy okienko informujące o sterowaniu w grze
		info.setWindowTitle(opis);
		info.setText(opis2);
		info.setStandardButtons(QMessageBox::Ok);
		info.setIcon(QMessageBox::Information);
		info.setWindowIcon(QIcon("ikona.png"));
		info.exec();
		uruchom_gre(TRYB0, WERSJA0);
	}
	else {
		//wyświetlenie informacji o sterowaniu w języku angielskim
		napis = "Information";
		QMessageBox info;                                       //info - tworzy okienko informujące o sterowaniu w grze
		info.setWindowTitle(napis);
		napis.clear();
		if (!sterowanie) napis = "Use keys W, S, A, D\nto move policemen.";
		else napis = "Use arrow keys\nto move policemen.";
		info.setText(napis);
		info.setStandardButtons(QMessageBox::Ok);
		info.setIcon(QMessageBox::Information);
		info.setWindowIcon(QIcon("ikona.png"));
		info.exec();
		uruchom_gre(TRYB0, WERSJA1);
	}
	wyswietl_glowne();
	okno.setVisible(true);
}

//metoda wywołująca metodę uruchom_gre(int, int) w wersji, gdy gracz gra złodziejem
void Glowne::uruchom_gre_zl() {
	QTextCodec* kod = QTextCodec::codecForName("Windows-1250");   //kod - przechowuje informację o kodowaniu czcionki
	QByteArray napis;                                             //napis - przechowuje napis
	QString opis, opis2;                                          //opis, opis2 - przechowują napisy w odpowiednim kodowaniu czcionki
	if (!jezyk) {
		//wyświetlenie informacji o sterowaniu w języku polskim
		napis = "Informacja";
		if (!sterowanie) {
			opis = kod->toUnicode(napis);
			napis.clear();
			napis = "Użyj klawiszy W, S, A, D,\naby wykonać ruch złodziejem.";
			opis2 = kod->toUnicode(napis);
		}
		else {
			opis = kod->toUnicode(napis);
			napis.clear();
			napis = "Użyj strzałek,\naby wykonać ruch złodziejem.";
			opis2 = kod->toUnicode(napis);
		}
		QMessageBox info;                                        //info - tworzy okienko informujące o sterowaniu w grze 
		info.setWindowTitle(opis);
		info.setText(opis2);
		info.setStandardButtons(QMessageBox::Ok);
		info.setIcon(QMessageBox::Information);
		info.setWindowIcon(QIcon("ikona.png"));
		info.exec();
		uruchom_gre(TRYB1, WERSJA0);
	}
	else {
		//wyświetlenie informacji o sterowaniu w języku angielskim
		napis = "Information";
		QMessageBox info;                                       //info - tworzy okienko informujące o sterowaniu w grze
		info.setWindowTitle(napis);
		napis.clear();
		if (!sterowanie) napis = "Use keys W, S, A, D\nto move the thief.";
		else napis = "Use arrow keys\nto move the thief.";
		info.setText(napis);
		info.setStandardButtons(QMessageBox::Ok);
		info.setIcon(QMessageBox::Information);
		info.setWindowIcon(QIcon("ikona.png"));
		info.exec();
		uruchom_gre(TRYB1, WERSJA1);
	}
	wyswietl_glowne();
	okno.setVisible(true);
}

/*
  metoda uruchamiająca grę w zależności od wybranego trybu gry i wybranej wersji językowej
  przyjmuje argumenty:
  -> tryb - identyfikuje tryb gry (gracz gra policjantami a komputer złodziejem lub gracz gra złodziejem a komputer policjantami)
  -> wersja - przechowuje informacje o wersji językowej programu (język polski lub angielski)
*/
void Glowne::uruchom_gre(int tryb, int wersja) {
	//wyświetlenie okna gry
	okno.setVisible(false);
	sf::RenderWindow gra(sf::VideoMode(ROZMIAR_OKNA, ROZMIAR_OKNA), "P vs Z", sf::Style::Titlebar | sf::Style::Close);   //gra - tworzy okno gry
	gra.setKeyRepeatEnabled(false);
	sf::Image ikona;   //ikona - reprezentuje ikonę na pasku tytułowym okna
	ikona.loadFromFile("ikona.png");
	gra.setIcon(ROZMIAR_IKONY, ROZMIAR_IKONY, ikona.getPixelsPtr());
	gra.setFramerateLimit(WART_POCZAT);
	gra.setVerticalSyncEnabled(true);

	Plansza mapa;   //mapa - tworzy planszę gry
	if (!wersja) mapa.zaladuj("plan.png", sf::Vector2u(ROZMIAR_WEKTOR, ROZMIAR_WEKTOR));
	else mapa.zaladuj("plan2.png", sf::Vector2u(ROZMIAR_WEKTOR, ROZMIAR_WEKTOR));
	Policjant pol(POL1, WSPOL_POL1, WSPOL_POL1);    //pol - tworzy postać pierwszego policjanta
	Policjant pol2(POL2, WSPOL_POL2, WSPOL_POL2);   //pol2 - tworzy postać drugiego policjanta
	Zlodziej zlo(ZLO, WSPOL_ZLO, WSPOL_ZLO);        //zlo - tworzy postać złodzieja

	//rozpoczęcie gry
	int ruch = RUCH1;                               //ruch - przedstawia kolejność ruchu w trybie gry: gracz - policjanci, komputer - złodziej
	int ruch2 = RUCH3;                              //ruch2 - przedstawia kolejność ruchu w trybie gry: gracz - złodziej, komputer - policjanci
	int koniec = KONIEC1;                           //koniec - przechowuje informacje o zakończeniu gry, czy też nie
	while (gra.isOpen()) {
		sf::Event zdarz;                            //zdarz - przechowuje zdarzenie, które nastąpiło (naciśnięcie klawisza albo zamknięcie okna)
		while (gra.pollEvent(zdarz)) {
			if (!tryb) {
				if (zdarz.type == sf::Event::KeyPressed) {
					if (ruch == RUCH1) {
						if (!sterowanie) {
							if (zdarz.key.code == sf::Keyboard::W) pol.porusz(mapa, GORA, ruch, zlo, koniec);
							else if (zdarz.key.code == sf::Keyboard::S) pol.porusz(mapa, DOL, ruch, zlo, koniec);
							else if (zdarz.key.code == sf::Keyboard::A) pol.porusz(mapa, LEWO, ruch, zlo, koniec);
							else if (zdarz.key.code == sf::Keyboard::D) pol.porusz(mapa, PRAWO, ruch, zlo, koniec);
						}
						else {
							if (zdarz.key.code == sf::Keyboard::Up) pol.porusz(mapa, GORA, ruch, zlo, koniec);
							else if (zdarz.key.code == sf::Keyboard::Down) pol.porusz(mapa, DOL, ruch, zlo, koniec);
							else if (zdarz.key.code == sf::Keyboard::Left) pol.porusz(mapa, LEWO, ruch, zlo, koniec);
							else if (zdarz.key.code == sf::Keyboard::Right) pol.porusz(mapa, PRAWO, ruch, zlo, koniec);
						}
					}
					else if (ruch == RUCH2) {
						if (!sterowanie) {
							if (zdarz.key.code == sf::Keyboard::W) pol2.porusz(mapa, GORA, ruch, zlo, koniec);
							else if (zdarz.key.code == sf::Keyboard::S) pol2.porusz(mapa, DOL, ruch, zlo, koniec);
							else if (zdarz.key.code == sf::Keyboard::A) pol2.porusz(mapa, LEWO, ruch, zlo, koniec);
							else if (zdarz.key.code == sf::Keyboard::D) pol2.porusz(mapa, PRAWO, ruch, zlo, koniec);
						}
						else {
							if (zdarz.key.code == sf::Keyboard::Up) pol2.porusz(mapa, GORA, ruch, zlo, koniec);
							else if (zdarz.key.code == sf::Keyboard::Down) pol2.porusz(mapa, DOL, ruch, zlo, koniec);
							else if (zdarz.key.code == sf::Keyboard::Left) pol2.porusz(mapa, LEWO, ruch, zlo, koniec);
							else if (zdarz.key.code == sf::Keyboard::Right) pol2.porusz(mapa, PRAWO, ruch, zlo, koniec);
						}
					}
				}
			}
			else {
				if (zdarz.type == sf::Event::KeyPressed) {
					if (ruch2 == RUCH3) {
						if (!sterowanie) {
							if (zdarz.key.code == sf::Keyboard::W) zlo.porusz(mapa, GORA, ruch2, koniec);
							else if (zdarz.key.code == sf::Keyboard::S) zlo.porusz(mapa, DOL, ruch2, koniec);
							else if (zdarz.key.code == sf::Keyboard::A) zlo.porusz(mapa, LEWO, ruch2, koniec);
							else if (zdarz.key.code == sf::Keyboard::D) zlo.porusz(mapa, PRAWO, ruch2, koniec);
						}
						else {
							if (zdarz.key.code == sf::Keyboard::Up) zlo.porusz(mapa, GORA, ruch2, koniec);
							else if (zdarz.key.code == sf::Keyboard::Down) zlo.porusz(mapa, DOL, ruch2, koniec);
							else if (zdarz.key.code == sf::Keyboard::Left) zlo.porusz(mapa, LEWO, ruch2, koniec);
							else if (zdarz.key.code == sf::Keyboard::Right) zlo.porusz(mapa, PRAWO, ruch2, koniec);
						}
					}
				}
			}
			if(!wersja) mapa.zaladuj("plan.png", sf::Vector2u(ROZMIAR_WEKTOR, ROZMIAR_WEKTOR));
			else mapa.zaladuj("plan2.png", sf::Vector2u(ROZMIAR_WEKTOR, ROZMIAR_WEKTOR));
			if (zdarz.type == sf::Event::Closed) gra.close();
		}
		if (!tryb) {
			if (ruch == RUCH3) {
				int los = zlo.wybierz_ruch(mapa, pol, pol2);
				if (!los) zlo.porusz(mapa, GORA, ruch, koniec);
				else if (los == LOS1) zlo.porusz(mapa, DOL, ruch, koniec);
				else if (los == LOS2) zlo.porusz(mapa, LEWO, ruch, koniec);
				else zlo.porusz(mapa, PRAWO, ruch, koniec);
			}
		}
		else {
			if (ruch2 == RUCH1) {
				int los = pol.dokonaj_ruch(mapa, zlo);
				if (!los) pol.porusz(mapa, GORA, ruch2, zlo, koniec);
				else if (los == LOS1) pol.porusz(mapa, DOL, ruch2, zlo, koniec);
				else if (los == LOS2) pol.porusz(mapa, LEWO, ruch2, zlo, koniec);
				else if (los == LOS3) pol.porusz(mapa, PRAWO, ruch2, zlo, koniec);
				else ruch2++;
			}
			if (ruch2 == RUCH2) {
				int los = pol2.dokonaj_ruch(mapa, zlo);
				if (!los) pol2.porusz(mapa, GORA, ruch2, zlo, koniec);
				else if (los == LOS1) pol2.porusz(mapa, DOL, ruch2, zlo, koniec);
				else if (los == LOS2) pol2.porusz(mapa, LEWO, ruch2, zlo, koniec);
				else if (los == LOS3) pol2.porusz(mapa, PRAWO, ruch2, zlo, koniec);
				else ruch2++;
			}
			if (!wersja) mapa.zaladuj("plan.png", sf::Vector2u(ROZMIAR_WEKTOR, ROZMIAR_WEKTOR));
			else mapa.zaladuj("plan2.png", sf::Vector2u(ROZMIAR_WEKTOR, ROZMIAR_WEKTOR));
			gra.clear();
			gra.draw(mapa);
			gra.display();
		}

		gra.clear();
		gra.draw(mapa);
		gra.display();
		zakoncz_gre(gra, tryb, wersja, koniec);
	}
}

void Glowne::zakoncz_gre(sf::RenderWindow& gra, int& tryb, int& wersja, int& koniec) {
	//wyświetlenie informacji o zakończeniu gry
	if (!wersja) {
		if (!tryb) {
			if (koniec == KONIEC2) {
				QTextCodec* kod = QTextCodec::codecForName("Windows-1250");   //kod - przechowuje informację o kodowaniu czcionki
				QByteArray napis = "Przegrana!";                              //napis - przechowuje napis w języku polskim
				QString opis = kod->toUnicode(napis);                         //opis - przechowuje napis w odpowiednim kodowaniu czcionki
				napis.clear();
				napis = "Niestety, złodziej uciekł.";
				QString opis2 = kod->toUnicode(napis);                        //opis2 - przechowuje napis w odpowiednim kodowaniu czcionki
				QMessageBox info;                                             //info - tworzy okno informujące o zakończeniu gry
				info.setWindowTitle(opis);
				info.setText(opis2);
				info.setStandardButtons(QMessageBox::Ok);
				info.setIcon(QMessageBox::Information);
				info.setWindowIcon(QIcon("ikona.png"));
				info.exec();
				gra.close();
			}
			else if (koniec == KONIEC3) {
				QTextCodec* kod = QTextCodec::codecForName("Windows-1250");   //kod - przechowuje informację o kodowaniu czcionki
				QByteArray napis = "Zwycięstwo!";                             //napis - przechowuje napis w języku polskim
				QString opis = kod->toUnicode(napis);                         //opis - przechowuje napis w odpowiednim kodowaniu czcionki
				napis.clear();
				napis = "Gratulacje!\nZłodziej został schwytany.";
				QString opis2 = kod->toUnicode(napis);                        //opis2 - przechowuje napis w odpowiednim kodowaniu czcionki
				QMessageBox info;                                             //info - tworzy okno informujące o zakończeniu gry
				info.setWindowTitle(opis);
				info.setText(opis2);
				info.setStandardButtons(QMessageBox::Ok);
				info.setIcon(QMessageBox::Information);
				info.setWindowIcon(QIcon("ikona.png"));
				info.exec();
				gra.close();
			}
		}
		else {
			if (koniec == KONIEC2) {
				QTextCodec* kod = QTextCodec::codecForName("Windows-1250");   //kod - przechowuje informację o kodowaniu czcionki
				QByteArray napis = "Zwycięstwo!";                             //napis - przechowuje napis w języku polskim
				QString opis = kod->toUnicode(napis);                         //opis - przechowuje napis w odpowiednim kodowaniu czcionki
				napis.clear();
				napis = "Gratulacje!\nPolicjanci cię nie złapali.";
				QString opis2 = kod->toUnicode(napis);
				QMessageBox info;                                             //info - tworzy okno informujące o zakończeniu gry
				info.setWindowTitle(opis);
				info.setText(opis2);
				info.setStandardButtons(QMessageBox::Ok);
				info.setIcon(QMessageBox::Information);
				info.setWindowIcon(QIcon("ikona.png"));
				info.exec();
				gra.close();
			}
			else if (koniec == KONIEC3) {
				QTextCodec* kod = QTextCodec::codecForName("Windows-1250");   //kod - przechowuje informację o kodowaniu czcionki
				QByteArray napis = "Przegrana!";                              //napis - przechowuje napis w języku polskim
				QString opis = kod->toUnicode(napis);                         //opis - przechowuje napis w odpowiednim kodowaniu czcionki
				napis.clear();
				napis = "Niestety, policjanci cię złapali.";
				QString opis2 = kod->toUnicode(napis);
				QMessageBox info;                                             //info - tworzy okno informujące o zakończeniu gry
				info.setWindowTitle(opis);
				info.setText(opis2);
				info.setStandardButtons(QMessageBox::Ok);
				info.setIcon(QMessageBox::Information);
				info.setWindowIcon(QIcon("ikona.png"));
				info.exec();
				gra.close();
			}
		}
	}
	else {
		if (!tryb) {
			if (koniec == KONIEC2) {
				QString opis = "Defeat!";                                          //opis - przechowuje napis w języku angielskim
				QString opis2 = "I am sorry, you lose.\nThe thief has escaped.";   //opis2 - przechowuje napis w języku angielskim
				QMessageBox info;                                                  //info - tworzy okno informujące o zakończeniu gry
				info.setWindowTitle(opis);
				info.setText(opis2);
				info.setStandardButtons(QMessageBox::Ok);
				info.setIcon(QMessageBox::Information);
				info.setWindowIcon(QIcon("ikona.png"));
				info.exec();
				gra.close();
			}
			else if (koniec == KONIEC3) {
				QString opis = "Victory!";                                                 //opis - przechowuje napis w języku angielskim
				QString opis2 = "Congratulations! You win!\nThe thief has been caught.";   //opis2 - przechowuje napis w języku angielskim
				QMessageBox info;                                                          //info - tworzy okno informujące o zakończeniu gry
				info.setWindowTitle(opis);
				info.setText(opis2);
				info.setStandardButtons(QMessageBox::Ok);
				info.setIcon(QMessageBox::Information);
				info.setWindowIcon(QIcon("ikona.png"));
				info.exec();
				gra.close();
			}
		}
		else {
			if (koniec == KONIEC2) {
				QString opis = "Victory!";                                        //opis - przechowuje napis w języku angielskim
				QString opis2 = "Congratulations! You win!\nYou have escaped.";   //opis2 - przechowuje napis w języku angielskim
				QMessageBox info;                                                 //info - tworzy okno informujące o zakończeniu gry
				info.setWindowTitle(opis);
				info.setText(opis2);
				info.setStandardButtons(QMessageBox::Ok);
				info.setIcon(QMessageBox::Information);
				info.setWindowIcon(QIcon("ikona.png"));
				info.exec();
				gra.close();
			}
			else if (koniec == KONIEC3) {
				QString opis = "Defeat!";                                                  //opis - przechowuje napis w języku angielskim
				QString opis2 = "I am sorry, you lose.\nThe policemen have caught you.";   //opis2 - przechowuje napis w języku angielskim
				QMessageBox info;                                                          //info - tworzy okno informujące o zakończeniu gry
				info.setWindowTitle(opis);
				info.setText(opis2);
				info.setStandardButtons(QMessageBox::Ok);
				info.setIcon(QMessageBox::Information);
				info.setWindowIcon(QIcon("ikona.png"));
				info.exec();
				gra.close();
			}
		}
	}
}
