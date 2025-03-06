#include "ui/okienko.h"
#include "ui/rysowanko.h"
#include "utils/fun.h"
#include <gtkmm.h>

// Zeruje ilość funkcji, której potrzebuje do rysowania układu współrzędnych
int Okienko::rzeczywiste = 0;
int Okienko::urojone = 0;

Okienko::Okienko() : all_VBox(Gtk::Orientation::VERTICAL){
  set_title("Graficzny Kalkulator Funkcji Matematycznych"); // Ustawia tytuł widoczny na górze aplikacji
  set_default_size(600, 1000); // Ustawia domyślną wieklość okna

  // Górny pasek z polami tekstowymi i przyciskiem "Dodaj"
  auto i_HBox = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::HORIZONTAL); // Pojemnik przechowujący górny pasek. Nazwa od inputs_HorizontalBox
  i_HBox -> set_spacing(5); // Ustawia margines pomiędzy elementami pojemnika na 5 pikseli
  i_a.set_placeholder_text("Podaj wartość a"); // ↓
  i_b.set_placeholder_text("Podaj wartość b"); //  } Dodają tekst tymczasowy dla pól edycyjnych
  i_c.set_placeholder_text("Podaj wartość c"); // ↑
  i_a.set_hexpand(true); // ↓
  i_b.set_hexpand(true); //  } Pozwala podanym polom edycyjnym rozszerzać się w poziomie
  i_c.set_hexpand(true); // ↑

  auto dodaj = Gtk::make_managed<Gtk::Button>(); // Tworzy guzik dodający nowe dane funkcji
  auto i_dodaj = Gtk::make_managed<Gtk::Image>("resources/images/button.xpm"); // Tworzy element przechowujący obrazek
  dodaj -> set_child(*i_dodaj); // Ustawia obrazek jako dziecko guzika dzięki czemu będzie się pokazywał w środku
  dodaj -> set_hexpand(false); // Zakazuje przyciskowi dodaj rozszerzać się poziomo
  dodaj -> set_vexpand(false); // Zakazuje przyciskowi dodaj rozszerzać się pionowo
  dodaj -> signal_clicked().connect(sigc::mem_fun(*this, &Okienko::dodaj_fun)); // Ta instrukcja sprawia, że kiedy przycisk jest kliknięty wywołuje metodę dodaj_fun()


  i_HBox -> append(i_a); //     ↓
  i_HBox -> append(i_b); //      } Dodaje do pojemnika i_HBox elementy górnego paska
  i_HBox -> append(i_c); //     ↑
  i_HBox -> append(*dodaj); //  ↑

  all_VBox.append(*i_HBox); // Dodaje do ogólnego pojemnika pojemnik z górnym paskiem


  auto odstep1 = Gtk::make_managed<Gtk::Box>(); //  ↓
  odstep1 -> set_vexpand(true); //                   } Tutaj dodaje pojemnik jako odstęp między polami edycyjnymi a etykietami funkcji, który jest potrzebny po to aby razem z pojemnikiem odstep2 jakimś cudem trzyma na samym dole okna legendę oraz obszar rysowania
  all_VBox.append(*odstep1); //                     ↑

// Funkcja pomocnicza, która tworzy parę: etykietę z wzorem funkcji i przycisk "Usuń".
// Dzięki niej unikam powtarzania tego samego kodu 5 razy (bo mamy 5 funkcji).
// Przyjmuje jako argumenty:
// - label: referencję do Gtk::Label, który będzie wyświetlał wzór funkcji
// - button: referencję do wskaźnika na Gtk::Button, który będzie służył do usuwania funkcji
// - idx: indeks funkcji (0-4), potrzebny do identyfikacji której funkcji dotyczy przycisk

  auto dodaj_etykiete_i_przycisk = [ &](Gtk::Label & label, Gtk::Button* & button, int idx){
    auto hbox = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::HORIZONTAL); // Pojemnik na etykietę i przycisk
    hbox -> set_spacing(5); // Ustawiam odstęp między elementami na 5 pikseli
    hbox -> set_halign(Gtk::Align::CENTER); // Wyśrodkowuję poziomo
    hbox -> set_margin_start(10); // Dodaję lewy margines, żeby ładnie wyglądało
    hbox -> append(label); // Dodaję etykietę do pojemnika

    button = Gtk::make_managed<Gtk::Button>(); // Tworzę przycisk "Usuń"
    auto image = Gtk::make_managed<Gtk::Image>("resources/images/x.xpm"); // Obrazek dla przycisku (ikona "X")
    button -> set_child(*image); // Ustawiam obrazek jako zawartość przycisku
    button -> set_hexpand(false); // Zakazuję przyciskowi rozszerzać się poziomo
    button -> set_vexpand(false); // Zakazuję przyciskowi rozszerzać się pionowo

    // Podłączam akcję do przycisku: gdy kliknięty, wywołuje metodę usun_funkcje(idx)
    button -> signal_clicked().connect([this, idx](){
      usun_funkcje(idx);
    });

    hbox -> append(*button); // Dodaję przycisk do pojemnika
    all_VBox.append(*hbox); // Dodaję całą parę (etykietę i przycisk) do głównego pionowego pojemnika

    button -> hide(); // Na początku przycisk jest ukryty, pojawi się dopiero po dodaniu funkcji
  };

  dodaj_etykiete_i_przycisk(f, usun_f, 0); //  ↓
  dodaj_etykiete_i_przycisk(g, usun_g, 1); //  ↓
  dodaj_etykiete_i_przycisk(h, usun_h, 2); //   } Wywołuje ową metodę, która tworzy nam etykiety i przyciski
  dodaj_etykiete_i_przycisk(i, usun_i, 3); //  ↑
  dodaj_etykiete_i_przycisk(j, usun_j, 4); //  ↑

  auto odstep2 = Gtk::make_managed<Gtk::Box>(); //  ↓
  odstep2 -> set_vexpand(true); //                   } Tutaj dodaje drugi odstep
  all_VBox.append(*odstep2); //                     ↑

  // Obszar rysowania
  rysowanko = new Rysowanko(*this); // Sprawia, że rysowanko ma wszystkie cechy, które są opisane w klasie Rysowanko po klasie Okienko, jednak sprawia to, że sami musimy zwolnić pamięć po tym obiekcie
  auto r_Box = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::HORIZONTAL); // Pojemnik na obszar rysowania. Potrzebuje go po to by można było to wycentrować ładnie. Nazwa od rysowanko_Box
  r_Box -> append(*rysowanko); // Dodaje do pojemnika obszar rysowania
  r_Box -> set_halign(Gtk::Align::CENTER); // Sprawia, że obszar rysowania jest wycentrowany poziomo
  r_Box -> set_valign(Gtk::Align::END); // Sprawia, że obszar rysowania trzyma się końca okna
  r_Box -> set_margin_bottom(10); // Ustawia dolny margines na 10 pikseli
  all_VBox.append(*r_Box); // Dodaje pojemnik obszaru rysowania do ogólnego pojemnika

  // Legenda
  auto l_VBox = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::VERTICAL); // Tworzy pojemnik na legendę. Nazwa od legenda_VerticalBox
  l_VBox -> set_valign(Gtk::Align::END); // Pozycjonuje pojemnik na dole okna
  l_VBox -> set_spacing(5); // Ustawia margines pomiędzy elementami na 5 piksel
  l_VBox -> set_margin_start(10);

  auto rz_HBox = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::HORIZONTAL); // Tworzy pojemnik przechowujący elementy legendy opisujące liczby rzeczywiste. Nazwa od rzeczywiste_HorizontalBox
  auto legendarz = Gtk::make_managed<Gtk::Label>(" - rozwiązanie w zbiorze liczb rzeczywistych"); // Tworzy etykietę opisującą właściwość rozwiązań w zbiorze liczb rzeczywistych. Nazwa od legendarzeczywiste
  auto rzeczywiste = Gtk::make_managed<Gtk::Image>("resources/images/rzeczywiste.xpm"); // Tworzy element, przechowujący obrazek koloru czerwonego
  rz_HBox -> append(*rzeczywiste); // Dodaje do pojemnika czerwony obrazek
  rz_HBox -> append(*legendarz); // Dodaje do pojemnika etykietę odpowiednią

  auto u_HBox = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::HORIZONTAL); // Tworzy pojemnik przechowujący elementy legendy opisujące liczby urojone. Nazwa od urojone_HorizontalBox
  auto legendau = Gtk::make_managed<Gtk::Label>(" - rozwiązanie w zbiorze liczb zespolonych"); // Tworzy etykietę opisującą właściwość rozwiązań w zbiorze liczb zespolonych. Nazwa od legendaurojone
  auto urojone = Gtk::make_managed<Gtk::Image>("resources/images/urojone.xpm"); // Tworzy element, przechowujący obrazek koloru niebieskiego
  u_HBox -> append(*urojone); // Dodaje do pojemnika niebieski obrazek
  u_HBox -> append(*legendau); // Dodaje do pojemnika etykietę odpowiednią

  l_VBox -> append(*rz_HBox); // Dodaje do pojemnika legendy pojemnik legendy o liczbach rzeczywistych
  l_VBox -> append(*u_HBox); // Dodaje do pojemnika legendy pojemnik legendy o liczbach urojonych

  auto podzialka = Gtk::make_managed<Gtk::Label>("Podziałka osi z krokiem co 10 pikseli"); // Tworzy etykietę mówiącą o podziałce
  podzialka -> set_halign(Gtk::Align::START); // Ustawia podziałkę na doczepioną do lewej
  podzialka -> set_margin_start(0); // Ustawia lewy margines na 10 pikseli
  podzialka -> set_margin_bottom(10);
  l_VBox -> append(*podzialka); // Dodaje etykietę o przedziałce do legendy

  all_VBox.append(*l_VBox); // Dodaje całą legendę do ogólnego pojemnik


  all_VBox.set_spacing(10); // Dodaje margines dookoła każdego elementu w ogólnym pajemniku o wartości 10 pikseli
  set_child(all_VBox); // Dodaje pojemnik ogólny do okienka
}

Okienko::~Okienko(){
  delete rysowanko; // Zwalniamy pamięć po obiekcie rysowanko
}

// Metoda dodająca nową funkcję
void Okienko::dodaj_fun(){
  int wolne_miejsce = -1;
  for (int i = 0; i < 5; i++){ // Szukamy najbliższego wolnego miejsca na nową funkcję
    if (!funkcje_aktywne[i]){
      wolne_miejsce = i;
      break;
    }
  }
  if (wolne_miejsce == -1){
    poka_blad("Nie można dodać więcej niż 5 funkcji!"); // Jeśli nie ma wolnego miejsca wyświetla błąd
    return;
  }

  double a, b, c; // Deklaruje zmienne do pobrania danych z pól edycyjncyh

  try{
    a = std::stod(i_a.get_text()); // ↓
    b = std::stod(i_b.get_text()); //  } Pobieram dane z pól edycyjnych
    c = std::stod(i_c.get_text()); // ↑
  }
  catch(const std::exception& e){ // Jeśli zamiana danych z pól edycyjnych na double zwróci std::exception to aplikacja zamiast się zakmnąć zwróci błąd poniżej
    poka_blad("Podano nieprawidłowe dane! Podaj prawidłowe dane liczbowe.");
    return;
  }

  funkcje_a[wolne_miejsce] = a; // ↓
  funkcje_b[wolne_miejsce] = b; //  } Dodają pobrane dane do odpowiadającej tablicy
  funkcje_c[wolne_miejsce] = c; // ↑
  funkcje_aktywne[wolne_miejsce] = true; // Wskazuje, że miejsce, które wcześniej było wolne zostało zajęte

  // Akrualizuje etykiety i przyciski
  if (wolne_miejsce == 0){
    if(a >= 0 && b >= 0 && c >= 0){
      f.set_text("f(x) = " + zaokraglona_wartosc(a) + "x² + " + zaokraglona_wartosc(b) + "x + " + zaokraglona_wartosc(c));
    }
    else if(a >= 0 && b >= 0 && c < 0){
      f.set_text("f(x) = " + zaokraglona_wartosc(a) + "x² + " + zaokraglona_wartosc(b) + "x " + zaokraglona_wartosc(c));
    }
    else if(a >= 0 && b < 0 && c >= 0){
      f.set_text("f(x) = " + zaokraglona_wartosc(a) + "x² " + zaokraglona_wartosc(b) + "x + " + zaokraglona_wartosc(c));
    }
    else if(a >= 0 && b < 0 && c < 0){
      f.set_text("f(x) = " + zaokraglona_wartosc(a) + "x² " + zaokraglona_wartosc(b) + "x " + zaokraglona_wartosc(c));
    }

    f.show();
    usun_f -> show();
  }
  else if (wolne_miejsce == 1){
    if(a >= 0 && b >= 0 && c >= 0){
      g.set_text("g(x) = " + zaokraglona_wartosc(a) + "x² + " + zaokraglona_wartosc(b) + "x + " + zaokraglona_wartosc(c));
    }
    else if(a >= 0 && b >= 0 && c < 0){
      g.set_text("g(x) = " + zaokraglona_wartosc(a) + "x² + " + zaokraglona_wartosc(b) + "x " + zaokraglona_wartosc(c));
    }
    else if(a >= 0 && b < 0 && c >= 0){
      g.set_text("g(x) = " + zaokraglona_wartosc(a) + "x² " + zaokraglona_wartosc(b) + "x + " + zaokraglona_wartosc(c));
    }
    else if(a >= 0 && b < 0 && c < 0){
      g.set_text("g(x) = " + zaokraglona_wartosc(a) + "x² " + zaokraglona_wartosc(b) + "x " + zaokraglona_wartosc(c));
    }

    g.show();
    usun_g -> show();
  }
  else if (wolne_miejsce == 2){
    if(a >= 0 && b >= 0 && c >= 0){
      h.set_text("h(x) = " + zaokraglona_wartosc(a) + "x² + " + zaokraglona_wartosc(b) + "x + " + zaokraglona_wartosc(c));
    }
    else if(a >= 0 && b >= 0 && c < 0){
      h.set_text("h(x) = " + zaokraglona_wartosc(a) + "x² + " + zaokraglona_wartosc(b) + "x " + zaokraglona_wartosc(c));
    }
    else if(a >= 0 && b < 0 && c >= 0){
      h.set_text("h(x) = " + zaokraglona_wartosc(a) + "x² " + zaokraglona_wartosc(b) + "x + " + zaokraglona_wartosc(c));
    }
    else if(a >= 0 && b < 0 && c < 0){
      h.set_text("h(x) = " + zaokraglona_wartosc(a) + "x² " + zaokraglona_wartosc(b) + "x " + zaokraglona_wartosc(c));
    }

    h.show();
    usun_h -> show();
  }
  else if (wolne_miejsce == 3){
    if(a >= 0 && b >= 0 && c >= 0){
      i.set_text("i(x) = " + zaokraglona_wartosc(a) + "x² + " + zaokraglona_wartosc(b) + "x + " + zaokraglona_wartosc(c));
    }
    else if(a >= 0 && b >= 0 && c < 0){
      i.set_text("i(x) = " + zaokraglona_wartosc(a) + "x² + " + zaokraglona_wartosc(b) + "x " + zaokraglona_wartosc(c));
    }
    else if(a >= 0 && b < 0 && c >= 0){
      i.set_text("i(x) = " + zaokraglona_wartosc(a) + "x² " + zaokraglona_wartosc(b) + "x + " + zaokraglona_wartosc(c));
    }
    else if(a >= 0 && b < 0 && c < 0){
      i.set_text("i(x) = " + zaokraglona_wartosc(a) + "x² " + zaokraglona_wartosc(b) + "x " + zaokraglona_wartosc(c));
    }

    i.show();
    usun_i -> show();
  }
  else if (wolne_miejsce == 4){
    if(a >= 0 && b >= 0 && c >= 0){
      j.set_text("j(x) = " + zaokraglona_wartosc(a) + "x² + " + zaokraglona_wartosc(b) + "x + " + zaokraglona_wartosc(c));
    }
    else if(a >= 0 && b >= 0 && c < 0){
      j.set_text("j(x) = " + zaokraglona_wartosc(a) + "x² + " + zaokraglona_wartosc(b) + "x " + zaokraglona_wartosc(c));
    }
    else if(a >= 0 && b < 0 && c >= 0){
      j.set_text("j(x) = " + zaokraglona_wartosc(a) + "x² " + zaokraglona_wartosc(b) + "x + " + zaokraglona_wartosc(c));
    }
    else if(a >= 0 && b < 0 && c < 0){
      j.set_text("j(x) = " + zaokraglona_wartosc(a) + "x² " + zaokraglona_wartosc(b) + "x " + zaokraglona_wartosc(c));
    }

    j.show();
    usun_j -> show();
  }

  if(wolne_miejsce != -1){
    if(pow(b, 2) - (4 * a * c) >= 0){
      rzeczywiste++;
    }
    else if(pow(b, 2) - (4 * a * c) < 0){
      urojone++;
    }
  }

  rysowanko -> dodaj_funkcje(a, b, c); // Wywołuje metodę dodaj_funkcje, która dodaje funkcje o podanych w argumentach wartościach do obszaru rysowania
}

// Metoda zwalniająca miejsce funkcji
void Okienko::usun_funkcje(int idx){
  if (!funkcje_aktywne[idx]) return; // Jeśli funkcja nie jest aktywna, nic nie robi

  // Ukrywa odpowiednią etykietę i przycisk
  if (idx == 0){
    f.hide();
    usun_f -> hide();
  }
  else if (idx == 1){
    g.hide();
    usun_g -> hide();
  }
  else if (idx == 2){
    h.hide();
    usun_h -> hide();
  }
  else if (idx == 3){
    this -> i.hide();
    usun_i -> hide();
  }
  else if (idx == 4){
    j.hide();
    usun_j -> hide();
  }

  if(pow(funkcje_b[idx], 2) - (4 * funkcje_a[idx] * funkcje_c[idx]) >= 0){
    rzeczywiste--;
  }
  else if(pow(funkcje_b[idx], 2) - (4 * funkcje_a[idx] * funkcje_c[idx]) < 0){
    urojone--;
  }

  funkcje_a[idx] = 0; // ↓
  funkcje_b[idx] = 0; //  } Resetuje wartości funkcji
  funkcje_c[idx] = 0; // ↑
  funkcje_aktywne[idx] = false; // Ustawia funckję na nieaktywną

  rysowanko -> usun_funkcje(idx); // Usuwa funkcję z obszaru rysowania
  queue_draw(); // Odświeża interfejs
}

void Okienko::poka_blad(const char* komunikat){
  auto dialog = Gtk::make_managed<Gtk::Dialog>("Błąd", *this);
    dialog -> set_modal(true);
    Gtk::Label label(komunikat);
    label.set_margin(20);
    dialog -> get_content_area()->append(label);
    auto ok_button = Gtk::make_managed<Gtk::Button>("OK");
    auto button_box = Gtk::make_managed<Gtk::CenterBox>();
    button_box->set_center_widget(*ok_button);
    dialog -> get_content_area()->append(*button_box);
    ok_button->signal_clicked().connect([dialog](){dialog->close();});
    dialog -> show();
}
