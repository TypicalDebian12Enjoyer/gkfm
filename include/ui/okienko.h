#ifndef OKIENKO_H
#define OKIENKO_H
#include <gtkmm.h>

class Rysowanko; //Deklaracja klasy Rysowanko. Potrzebna, ponieważ w klasie Okienko tworzymy instancję obiektu tej klasy i bez tej krótkiej instrukcji kompilator mówi, że Rysowanko nie określa typu danych i ma rację, ponieważ typ danych Rysowanko jest określany dopiero później

class Okienko : public Gtk::Window {
public:
  Okienko(); // Konstruktor jawny domyślny klasy Okienko
  ~Okienko(); // Destruktor (Dekonstruktor :)), dzięki któremu dynamicznie zwalniam pamięć po obiekcie rysowanko zawartym w tej klasie
  void dodaj_fun(); // Chyba najważniejsza metoda w tym projekcie. Sprawdza dostępność miejsc dla funkcji, obsługuje limit, pobiera wartości, dodaje wartości funkcji do tablicy, aktualizuje etykiety, dodaje funkcje do wykresu oraz odświeża widok
  void usun_funkcje(int index); // Metoda wywoływana przez guziki "X", która zwalnia miejsce funkcji o id podanym w argumencie
  void poka_blad(const char* komunikat);
  static int rzeczywiste, urojone;
private:
  Gtk::Box all_VBox; // Pionowy pojemnik na wszystkie elementy
  double funkcje_a[5], funkcje_b[5], funkcje_c[5]; // Tablice typu zmienno-przecinkowego przechowujące współczynniki a, b i c wszystkich funkcji
  bool funkcje_aktywne[5]; // Bardzo potrzebna tablica boolowska dla funkcjonalności kodu, która zawiera informacje czy funckja jest widoczna na ekranie, czyli "działa"
  Gtk::Entry i_a; // Pole wprowadzania parametru a do funkcji
  Gtk::Entry i_b; // Pole wprowadzania parametru b do funkcji
  Gtk::Entry i_c; // Pole wprowadzania parametru c do funkcji
  Gtk::Label f, g, h, i, j; // Etykiety - z początku ukryte - zawierające wzory funkcji
  Gtk::Button* usun_f; // Przycisk zwalniający miejsce funkcji f
  Gtk::Button* usun_g; // Przycisk zwalniający miejsce funkcji g
  Gtk::Button* usun_h; // Przycisk zwalniający miejsce funkcji h
  Gtk::Button* usun_i; // Przycisk zwalniający miejsce funkcji i
  Gtk::Button* usun_j; // Przycisk zwalniający miejsce funkcji j
  Rysowanko* rysowanko; // Instancja obiektu klasy Rysowanko, na której rysujemy wykresy funkcji matematycznych oraz dzięki niej metody z klasy Rysowanko mogą "dostać" dane z pól edycyjnych klasy Okienko
};

#endif // OKIENKO_H
