#pragma once
#include <iostream>
#include <bit>

using namespace std;

void DetectEndian()
{
  using cstr_t = const char *;
  cout.imbue(locale(".utf8"));

  if constexpr (endian::native == endian::big)
  {
    cout << (cstr_t)u8"Big endian system" << endl;
  }
  else if constexpr (endian::native == endian::little)
  {
    cout << (cstr_t)u8"Little endian system" << endl;
  }

  cout << endl;
}
