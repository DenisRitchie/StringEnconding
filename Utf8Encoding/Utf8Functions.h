#pragma once
#include <cuchar>
#include <cwchar>
#include <string>
#include <string_view>
#include <iostream>
#include <memory>
#include <clocale>
#include <cassert>
#include "Utf8Extensions.h"

using namespace std;

void Utf16FunctionsTest_Utf16ToAnsi()
{
  const char16_t *pointer = u"Juan Souli\u00e9";
  char *const buffer = new char[MB_CUR_MAX];
  mbstate_t mbstate{ };

  while (*pointer)
  {
    size_t length = c16rtomb(buffer, *pointer, &mbstate);

    if (length == 0 || length > MB_CUR_MAX)
      break;

    for (size_t index = 0; index < length; ++index)
      putchar(buffer[index]);

    ++pointer;
  }

  delete[] buffer;
}

void Utf32FunctionsTest_Utf32ToAnsi()
{
  const char32_t *pointer = U"Juan Souli\u00e9";
  char *const buffer = new char[MB_CUR_MAX];
  mbstate_t mbstate{ };

  while (*pointer)
  {
    size_t length = c32rtomb(buffer, *pointer, &mbstate);

    if (length == 0 || length > MB_CUR_MAX)
      break;

    for (size_t index = 0; index < length; ++index)
      putchar(buffer[index]);

    ++pointer;
  }

  delete[] buffer;
}

void Utf16FunctionsTest_AnsiToUtf16()
{
  std::setlocale(LC_ALL, "en_US.utf8");

  std::string str = (cstr_t)u8"zß水🍌☺☻♥Æ¦↑È";
  std::u16string u16str;

  std::cout << "Processing " << str.size() << " bytes: [ " << std::showbase;

  for (unsigned char c : str)
    std::cout << std::hex << +c << ' ';

  std::cout << "]\n";

  std::mbstate_t state{ }; // zero-initialized to initial state
  char16_t c16;
  const char *ptr = &str[0], *end = &str[0] + str.size();

  while (std::size_t rc = std::mbrtoc16(&c16, ptr, end - ptr + 1, &state))
  {
    u16str.push_back(c16);

    std::cout << "Next UTF-16 char: " << std::hex << +c16 << " obtained from ";

    if (rc == (std::size_t) - 3)
      std::cout << "earlier surrogate pair\n";
    else if (rc == (std::size_t) - 2)
      break;
    else if (rc == (std::size_t) - 1)
      break;
    else
    {
      std::cout << std::dec << rc << " bytes [ ";

      for (std::size_t n = 0; n < rc; ++n)
        std::cout << std::hex << +(unsigned char)ptr[n] << ' ';

      std::cout << "]\n";

      ptr += rc;
    }
  }

  cout << u"\nLa cadena es: " << u16str << endl;
}


void Utf32FunctionsTest_AnsiToUtf32()
{
  std::setlocale(LC_ALL, "en_US.utf8");

  std::string str = (cstr_t)u8"zß水🍌☺☻♥Æ¦↑È";
  std::u32string u32str;

  std::cout << "Processing " << str.size() << " bytes: [ " << std::showbase;

  for (unsigned char c : str)
    std::cout << std::hex << +c << ' ';

  std::cout << "]\n";

  std::mbstate_t state{ }; // zero-initialized to initial state
  char32_t c32;
  const char *ptr = str.c_str(), *end = str.c_str() + str.size() + 1;

  while (std::size_t rc = std::mbrtoc32(&c32, ptr, end - ptr, &state))
  {
    u32str.push_back(c32);

    std::cout << "Next UTF-32 char: " << std::hex << +c32 << " obtained from ";

    assert(rc != (std::size_t) - 3); // no surrogates in UTF-32
    if (rc == (std::size_t) - 1) break;
    if (rc == (std::size_t) - 2) break;

    std::cout << std::dec << rc << " bytes [ ";

    for (std::size_t n = 0; n < rc; ++n)
      std::cout << std::hex << +(unsigned char)ptr[n] << ' ';

    std::cout << "]\n";
    ptr += rc;
  }

  cout << u"\nLa cadena es: " << u32str << endl;
}
