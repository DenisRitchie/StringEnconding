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

// https://stackoverflow.com/questions/42899410/convert-utf-32-wide-char-to-utf-16-wide-char-in-linux-for-supplementary-plane-ch
// https://stackoverflow.com/questions/42899410/convert-utf-32-wide-char-to-utf-16-wide-char-in-linux-for-supplementary-plane-ch/43176982#43176982

u16string ConvertUTF32ToUTF16(u32string_view u32_source) noexcept
{
  u16string u16content;

  for (char32_t u32_character : u32_source)
  {
    if (u32_character <= 0x0000FFFF)
    {
      /* UTF-16 surrogate values are illegal in UTF-32
         0xFFFF or 0xFFFE are both reserved values */
      if (u32_character >= 0xD800 && u32_character <= 0xDFFF)
      {
        u16content.push_back(0x0000FFFD);
      }
      else
      {
        /* u32_source is a BMP Character */
        u16content.push_back(u32_character);
      }
    }
    else if (u32_character > 0x0010FFFF)
    {
      /* U+10FFFF is the largest code point of Unicode Character Set */
      u16content.push_back(0x0000FFFD);
    }
    else
    {
      /* u32_source is a character in range 0xFFFF - 0x10FFFF */
      u32_character -= 0x0010000UL;
      u16content.push_back((u32_character >> 10) + 0xD800);
      u16content.push_back((u32_character & 0x3FFUL) + 0xDC00);
    }
  }

  return u16content;
}

void Test_01_FileOnUtf8()
{
  // https://unicode.org/emoji/charts/full-emoji-list.html
  // https://unicode.org/emoji/charts/full-emoji-modifiers.html

  {
    ofstream file{ "test_file.txt", ios::out | ios::binary };
    file.imbue(locale("es_NI.utf8"));

    if (file.is_open())
    {
      file <<
        U"\U0001F44B\U0001F3FB \U0001F596\U0001F3FF \U0001F44C\U0001F3FB \U0001F447\U0001F3FF \n"
        U"\U0001F600 \U0001F601 \U0001F607 \U0001F911 \U0001F637 \U00002620 \U0001F47B \U0001F91F \U0001F448 \U0001F469 \n"
        U"\U0001F469\U0000200D\U0001F3A4 \U0001F9D1\U0000200D\U0001F3A8 \U0001F9D1\U0000200D\U00002708\U0000FE0F \U0001F468\U0000200D\U0001F468\U0000200D\U0001F466\U0000200D\U0001F466";

      file.close();
    }
  }

  {
    ifstream file{ "test_file.txt", ios::in | ios::binary };
    file.imbue(locale("es_NI.utf8"));

    if (file.is_open())
    {

      vector<u16string> contents;

      /*while (!file.eof())
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
      }*/

      while (!file.eof())
      {
        string ansi;
        getline(file, ansi);

        u32string u32content;
        mbstate_t mbstate{ };
        char32_t c32;

        string::const_pointer ptr = ansi.data();
        string::pointer end = ansi.data() + ansi.size() + /*'\0'*/1;

        while (size_t rc = mbrtoc32(&c32, ptr, end - ptr, &mbstate))
        {
          if (rc == (size_t)-1)
            break;

          u32content.push_back(c32);
          ptr += rc;
        }

        contents.push_back(ConvertUTF32ToUTF16(u32content));
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


