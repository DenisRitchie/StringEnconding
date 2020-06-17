#pragma once
#include <iostream>
#include <locale>

using namespace std;

void InformacionNicaragua()
{
  using cstr_t = const char *;
  #define Nicaragua use_facet<moneypunct<char, true>>(locale("en_NI.utf8"))
  cout.imbue(locale(".utf8"));
  cout
    << (cstr_t)u8"\nInformaci�n Nicaragua:\n"
    << (cstr_t)u8"S�mbolo moneda: " << Nicaragua.curr_symbol() << endl
    << "Separador decimal: " << Nicaragua.decimal_point() << endl
    << "Separador de miles: " << Nicaragua.thousands_sep() << endl
    << (cstr_t)u8"D�gitos decimales: " << Nicaragua.frac_digits() << endl
    << "Signo negativo: " << Nicaragua.negative_sign() << endl
    << "Formato negativo: " << Nicaragua.neg_format().field << endl
    << "Signo positivo: " << Nicaragua.positive_sign() << endl
    << "Formato positivo: " << Nicaragua.pos_format().field << endl
    << (cstr_t)u8"Agrupaci�n: " << Nicaragua.grouping() << endl
    << endl;
}
