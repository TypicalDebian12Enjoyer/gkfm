#include "ui/rysowanko.h"
#include "ui/okienko.h"
#include <iostream>
#include <cairomm/context.h>
#include <cmath>
#include <string>
#include <cstdio>

Rysowanko::Rysowanko(Okienko& _okienko) : okienko(_okienko) {
  set_size_request(500, 500); // Ustawia wymagany rozmiar dla obszaru rysowania
  set_hexpand(false); // Zakazuje mu się rozszerzać poziommo
  set_vexpand(false); // To samo co wyzej, ale pionowo

  set_draw_func([this](const Cairo::RefPtr<Cairo::Context>& cr, int width, int height) {
    rysuj_wszystko(cr, width, height); // Wywołuje metodę rysującą
  });
}

// Metoda dodająca funkcję do obszaru rysowania. Działa tak samo jak ta z klasy Okienko, więc nie będę tego jeszcze raz tłumaczyć
void Rysowanko::dodaj_funkcje(double a, double b, double c){
  int wolne_miejsce = -1;
  for (int i = 0; i < 5; i++){
    if (!funkcje_aktywne[i]){
      wolne_miejsce = i;
      break;
    }
  }
  if (wolne_miejsce == -1){
    return;
  }
  funkcje_a[wolne_miejsce] = a;
  funkcje_b[wolne_miejsce] = b;
  funkcje_c[wolne_miejsce] = c;
  funkcje_aktywne[wolne_miejsce] = true;
  queue_draw();
}

// Metoda usuwająca funkcję z obszaru rysowania, w argumecie przyjmuje index funkcji którą ma usunąć
void Rysowanko::usun_funkcje(int idx){
  funkcje_aktywne[idx] = false; // Dezaktywuje funkcję
  queue_draw(); // Odświeża interfejs
}

// Metoda odpowiedzialna za rysowanie wszystkiego w obszarze rysowania
void Rysowanko::rysuj_wszystko(const Cairo::RefPtr<Cairo::Context>& cr, int width, int height){
  cr -> set_source_rgb(0.0, 0.0, 0.0); // Ustawia kolor pędzla na czarny
  cr -> set_line_width(2.0); // Ustawia grubość pędzla na 2 piksele
  cr -> rectangle(0, 0, width, height); // Zakreśla obwódke obszaru rysowania
  cr -> stroke(); // Wypełnia kolorem obwódke

  // Stałe dane dla rysowania
  double axis_color[3] = {0.0, 0.0, 0.0}; // Kolor do rysowania osi
  double text_color[3] = {0.0, 0.0, 0.0}; // Kolor do rysowania tekstu
  double re_text_color[3] = {1.0, 0.0, 0.0}; // Kolor do rysowania X i Y
  double im_text_color[3] = {0.0, 0.0, 1.0}; // Kolor do rysowania Re i Im
  double arrow_size = 8.0; // Wielkość strzałek na końcu osi
  double font_size = 12.0; // Wielkość czcionki

  // Rysowanie osi
  cr -> set_source_rgb(axis_color[0], axis_color[1], axis_color[2]);
  cr -> set_line_width(1.5);
  cr -> move_to(0, height / 2.0);
  cr -> line_to(width, height / 2.0);
  cr -> stroke();
  cr -> move_to(width / 2.0, 0);
  cr -> line_to(width / 2.0, height);
  cr -> stroke();
  cr -> move_to(width - arrow_size, height / 2.0 - arrow_size / 2.0);
  cr -> line_to(width, height / 2.0);
  cr -> close_path();
  cr -> fill();
  cr -> move_to(width / 2.0 - arrow_size / 2.0, arrow_size);
  cr -> line_to(width / 2.0, 0);
  cr -> line_to(width / 2.0 + arrow_size / 2.0, arrow_size);
  cr -> close_path();
  cr -> fill();

  // Ustawianie czcionki
  cr -> set_font_size(font_size);
  cr -> select_font_face("Sans", Cairo::ToyFontFace::Slant::NORMAL, Cairo::ToyFontFace::Weight::BOLD);
  cr -> set_source_rgb(text_color[0], text_color[1], text_color[2]);
  Cairo::TextExtents extents;

  // W zależności od tego jakie są funkcje pokazuje osie OX i OY, Re i Im albo obie
  if (Okienko::rzeczywiste == 0 && Okienko::urojone > 0){ // Rysowanie opisu osi Re i Im
    cr -> set_source_rgb(im_text_color[0], im_text_color[1], im_text_color[2]);

    cr -> get_text_extents("Re", extents);
    double re_label_x = width - arrow_size / 2.0 - extents.width / 2.0 - 7;
    double re_label_y = height / 2.0 - extents.height * 1.8 + 2 + 35;
    cr->move_to(re_label_x, re_label_y);
    cr->show_text("Re");

    cr->get_text_extents("Im", extents);
    double im_label_x = width / 2.0 + arrow_size + 5 - 38;
    double im_label_y = arrow_size + extents.height / 2.0;
    cr->move_to(im_label_x, im_label_y);
    cr->show_text("Im");
  }
  else if (Okienko::rzeczywiste > 0 && Okienko::urojone == 0) { // Rysowanie opisu osi OX i OY
    cr->set_source_rgb(re_text_color[0], re_text_color[1], re_text_color[2]);

    cr->get_text_extents("X", extents);
    double x_label_x = width - arrow_size / 2.0 - extents.width / 2.0 - 2;
    double x_label_y = height / 2.0 - extents.height * 1.8 + 2;
    cr->move_to(x_label_x, x_label_y);
    cr->show_text("X");

    cr->get_text_extents("Y", extents);
    double y_label_x = width / 2.0 + arrow_size + 5;
    double y_label_y = arrow_size + extents.height / 2.0;
    cr->move_to(y_label_x, y_label_y);
    cr->show_text("Y");
  }
  else if (Okienko::rzeczywiste > 0 && Okienko::urojone > 0) { // Rysowanie opisu osi OX, OY, Re i Im
    cr->set_source_rgb(re_text_color[0], re_text_color[1], re_text_color[2]);

    cr->get_text_extents("X", extents);
    double x_label_x = width - arrow_size / 2.0 - extents.width / 2.0 - 2;
    double x_label_y = height / 2.0 - extents.height * 1.8 + 2;
    cr->move_to(x_label_x, x_label_y);
    cr->show_text("X");

    cr->get_text_extents("Y", extents);
    double y_label_x = width / 2.0 + arrow_size + 5;
    double y_label_y = arrow_size + extents.height / 2.0;
    cr->move_to(y_label_x, y_label_y);
    cr->show_text("Y");

    cr->set_source_rgb(im_text_color[0], im_text_color[1], im_text_color[2]);

    cr->get_text_extents("Re", extents);
    double re_label_x = width - arrow_size / 2.0 - extents.width / 2.0 - 7;
    double re_label_y = height / 2.0 - extents.height * 1.8 + 2 + 35;
    cr->move_to(re_label_x, re_label_y);
    cr->show_text("Re");

    cr->get_text_extents("Im", extents);
    double im_label_x = width / 2.0 + arrow_size + 5 - 38;
    double im_label_y = arrow_size + extents.height / 2.0;
    cr->move_to(im_label_x, im_label_y);
    cr->show_text("Im");
  }

  cr -> set_source_rgb(text_color[0], text_color[1], text_color[2]);

  // Rysowanie przedziałki na osi poziomej
  for (int x = -width / 2; x < width / 2; x += 10){
    if (x == 0) continue;

    double screen_x = x + width / 2.0;
    double screen_y = height / 2.0;

    cr -> move_to(screen_x, screen_y - 5);
    cr -> line_to(screen_x, screen_y + 5);
    cr -> stroke();
  }

  // Rysowanie przedziałki na osi pionowej
  for (int y = -height / 2; y < height / 2; y += 10){
    if (y == 0) continue;

    double screen_x = width / 2.0;
    double screen_y = y + height / 2.0;

    cr -> move_to(screen_x - 5, screen_y);
    cr -> line_to(screen_x + 5, screen_y);
    cr -> stroke();
  }

  // Rysowanie funkcji
  for (int i = 0; i < 5; i++){
    if (!funkcje_aktywne[i]) continue;
    double a = funkcje_a[i];
    double b = funkcje_b[i];
    double c = funkcje_c[i];
    double delta = pow(b, 2) - 4 * a * c;
    if (delta >= 0.0){
      cr -> set_source_rgb(1.0, 0.0, 0.0);
    }
    else{
      cr -> set_source_rgb(0.0, 0.0, 1.0);
    }
    bool pierwszy_punkt = true; // Pierwszy punkt jest potrzebny po to aby go pominąć, gdyż występuje jakiś dziwny błąd przez który dla pierwszego x funkcja zwraca pionową kręskę co nawet nie można wynikiem funkcji nazwać
    for (int x = -width / 2; x < width / 2; x++){
      double y = a * pow(x, 2) + b * x + c;
      if (pierwszy_punkt){
        cr -> move_to(x + width / 2.0, height / 2.0 - y);
        pierwszy_punkt = false;
      }
      else{
        cr -> line_to(x + width / 2.0, height / 2.0 - y);
      }
    }
    cr -> stroke();
  }
}
