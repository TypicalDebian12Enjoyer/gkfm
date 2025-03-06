#ifndef RYSOWANKO_H
#define RYSOWANKO_H
#include <gtkmm.h>

class Okienko; // Potrzebne by nie wyskakiwały problemy z kompilacją

class Rysowanko : public Gtk::DrawingArea {
public:
  Rysowanko(Okienko& okienko); // Konstruktor z argumentem
  void dodaj_funkcje(double a, double b, double c); // Metoda, która dodaje podane w argumentach dane do najbliższych miejsc w tablicach oraz jeśli limit został osiągnięty zwraca stosowny komunikat
  void usun_funkcje(int index); // Zwalnia miejsce w tablicy funkcji o podanym w argumencie indexie
  void rysuj_wszystko(const Cairo::RefPtr<Cairo::Context>& cr, int width, int height); // Metoda, w której obsługujemy całe rysowanie
private:
  Okienko& okienko; // Instancja obiektu Okienko, dzięki któremu możemy np. pobierać dane z pól edycyjnych
  double funkcje_a[5], funkcje_b[5], funkcje_c[5]; // Zdublowane tablice z klasy Okienko i wiem, że istnieje coś takiego jak klasy zaprzyjaźnione, jednak nie miałem czasu się z nimi zapoznać
  bool funkcje_aktywne[5]; //Ta sama filozofia co z tablicami powyżej
};

#endif // RYSOWANKO_H
