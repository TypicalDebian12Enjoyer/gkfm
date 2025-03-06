#include "utils/fun.h"

// Funkcja, która zwraca z liczby zmienno-przecinkowej ją samą, ale tylko z liczbami znaczącymi po przecinku to jest np. przed 1.30000 po 1.3
std::string zaokraglona_wartosc(double wartosc){
  char buffer[50];
  snprintf(buffer, sizeof(buffer), "%.6f", wartosc);
  std::string wynik = buffer;
  size_t pozycja_kropki = wynik.find('.');
  if (pozycja_kropki != std::string::npos){
    while (wynik.size() > pozycja_kropki + 1 && wynik.back() == '0'){
      wynik.pop_back();
    }
    if (wynik.back() == '.'){
      wynik.pop_back();
    }
  }
  return wynik;
}
