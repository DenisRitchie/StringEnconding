#pragma once
#include <iostream>
#include <iomanip>
#include <locale>
#include <codecvt>
#include <cuchar>
#include <string>

using namespace std;

void TestFacetOut()
{
  locale::global(locale("es_NI.utf8"));

  using cstr_t = const char *;
  using facet_t = codecvt<wchar_t, char, mbstate_t>;

  auto &facet = use_facet<facet_t>(locale());

  wstring internal = L"☺☻♥♦♣♠•◘○◙♂♀♪♫☼►◄↕‼¶§▬↨↑↓→←∟↔▲♥";
  string external(internal.length() * facet.max_length(), '\0');

  mbstate_t mbstate{ };
  const wchar_t *from_next;
  char *to_next;

  facet_t::result result = facet.out(mbstate,
                                     &internal[0], &internal[internal.size()], from_next,
                                     &external[0], &external[external.size()], to_next);

  external.resize(to_next - external.data());

  if (result == facet_t::ok)
    cout << (cstr_t)u8"Traducción correcta: " << external << '\n';
  else
    cout << "Error en traducir el texto.\n";
}

