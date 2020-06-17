#pragma once
#include <iostream>
#include <string>
#include <locale>
#include <codecvt>

using namespace std;

void TestFacetOutUtf16()
{
  using cstr_t = const char *;
  using facet_t = codecvt<char16_t, char8_t, mbstate_t>;

  auto &facet = use_facet<facet_t>(locale(".65001"));
  std::u16string internal = u"☺☻♥♦♣♠•◘○◙♂♀♪♫☼►◄↕‼¶§▬↨↑↓→←∟↔▲▼╣║╗";

  facet_t::state_type mb{ };
  std::u8string external(internal.size() * facet.max_length(), u8'\0');

  const char16_t *from_next;
  char8_t *to_next;

  facet_t::result facet_result =
    facet.out(mb,
              internal.data(), internal.data() + internal.length(), from_next,
              external.data(), external.data() + external.length(), to_next);

  external.resize(to_next - external.data());

  switch (facet_result)
  {
    case facet_t::ok:
    {
      cout << "La cadena es: " << (cstr_t)external.c_str() << endl;
      break;
    }

    case facet_t::partial:
    {
      cout << "Conversión parcial: " << (cstr_t)external.c_str() << endl;
      break;
    }

    case facet_t::noconv:
    {
      cout << "No hubo conversión: " << (cstr_t)external.c_str() << endl;
      break;
    }
    case facet_t::error:
    {
      cout << "Error al convertir" << endl;
      break;
    }

    default:
      __assume(0);
  }
}

