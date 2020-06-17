#pragma once
#include <iostream>
#include <codecvt>
#include <string>

using namespace std;

void TestFacetIn()
{
  locale::global(locale("es_NI.utf8"));
  using facet_t = codecvt<wchar_t, char, mbstate_t>;

  auto &facet = use_facet<facet_t>(locale());

  string external = (const char *)u8"☺☻♥♠○◘•♦╗╝¢╚♠○";
  wstring internal(external.size() * facet.max_length(), L'\0');

  std::mbstate_t mbstate{};
  const char *from_next;
  wchar_t *to_next;

  facet_t::result result = facet.in(mbstate,
                                    &external[0], &external[external.length()], from_next,
                                    &internal[0], &internal[internal.length()], to_next);

  if (result == facet_t::ok)
  {
    std::wcout << L"Traducción correcta: " << internal << endl;
  }
}
