#ifndef FUN_H
#define FUN_H
#include <string>
#include <cstdio>

// Funkcja, która zwraca z liczby zmienno-przecinkowej ją samą, ale tylko z liczbami znaczącymi po przecinku to jest np. przed 1.30000 po 1.3
std::string zaokraglona_wartosc(double wartosc);

#endif // FUN_H
