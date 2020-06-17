#pragma once
#include <iostream>
#include <locale>
#include <codecvt>
#include <fstream>
#include <string>
#include <cuchar>
#include <vector>

#include "Utf8Extensions.h"

using namespace std;


void Test_01_FileOnUtf8()
{
  // https://unicode.org/emoji/charts/full-emoji-list.html

  {
    ofstream file{ "test_file.txt", ios::out | ios::binary };
    file.imbue(locale("es_NI.utf8"));

    if (file.is_open())
    {
      file
        << U"Este esto se escribirá en utf8\n"
        << U"\U0001F600 \U0001F601 \U0001F607 \U0001F911 \U0001F637 \U00002620 \U0001F47B	\U0001F91F \U0001F448 \U0001F469"
        << endl
        << U"\U0001F469\U0000200D\U0001F3A4 \U0001F9D1\U0000200D\U0001F3A8 \U0001F9D1\U0000200D\U00002708\U0000FE0F \U0001F468\U0000200D\U0001F468\U0000200D\U0001F466\U0000200D\U0001F466";

      file.close();
    }
  }

  {
    ifstream file{ "test_file.txt", ios::in | ios::binary };
    file.imbue(locale("es_NI.utf8"));

    if (file.is_open())
    {

      vector<u16string> contents;

      while (!file.eof())
      {
        string ansi;
        getline(file, ansi);

        u32string u32content;
        mbstate_t mbstate{ };
        char32_t c32;
        const char *ptr = ansi.data(), *end = ansi.data() + ansi.size();

        while (size_t rc = mbrtoc32(&c32, ptr, end - ptr + 1, &mbstate))
        {
          u32content.push_back(c32);
          ptr += rc;
        }

        u16string u16content;
        {
          u8string external;
          {
            auto &facet = use_facet<facet_utf32_t>(locale(".65001"));
            external.resize(u32content.length() * facet.max_length(), u8'\0');
            mbstate = mbstate_t{ };
            const char32_t *from_next;
            char8_t *to_next;

            facet_utf32_t::result rc =
              facet.out(mbstate,
                        u32content.data(), u32content.data() + u32content.size(), from_next,
                        external.data(), external.data() + external.size(), to_next);

            external.resize(to_next - external.data());
          }
          {
            auto &facet = use_facet<facet_utf16_t>(locale(".65001"));
            u16content.resize(external.size() * facet.max_length(), u'\0');
            mbstate = mbstate_t{ };
            const char8_t *from_next;
            char16_t *to_next;

            facet_utf16_t::result rc =
              facet.in(mbstate,
                       external.data(), external.data() + external.size(), from_next,
                       u16content.data(), u16content.data() + u16content.size(), to_next);

            u16content.resize(to_next - u16content.data());
          }
        }

        contents.push_back(move(u16content));
      }

      HWND hconsole = GetConsoleWindow();
      HDC hdc = GetDC(hconsole);
      HFONT hfont = CreateFontW(50, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, L"MV Boli");
      HFONT oldfont = (HFONT)SelectObject(hdc, hfont);

      for (int index = 0; const auto & content : contents)
      {
        TextOutW(hdc, 20, ++index * 100, (LPWSTR)content.c_str(), content.length());
      }

      SelectObject(hdc, oldfont);
      DeleteObject(hfont);
      ReleaseDC(hconsole, hdc);
    }
  }
}


