//**Część implementacyjna interfejsu użytkownika i funkcjonalności programu**//
//**Written by Igor Puścion**//

#include <gtkmm/application.h>
#include "ui/okienko.h"
#include "ui/rysowanko.h"
#include "utils/fun.h"

int main(int argc, char* argv[]) {
  auto apka = Gtk::Application::create();
  return apka -> make_window_and_run<Okienko>(argc, argv);
}
