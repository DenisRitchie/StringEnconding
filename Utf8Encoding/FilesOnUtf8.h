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

void ConvertUTF32ToUTF16(const char32_t u32_character, char16_t u16_characters[2], size_t &u16_size) noexcept
{
  if (u32_character <= 0x0000FFFF)
  {
    /* UTF-16 surrogate values are illegal in UTF-32
       0xFFFF or 0xFFFE are both reserved values */
    if (u32_character >= 0xD800 && u32_character <= 0xDFFF)
    {
      *u16_characters = 0x0000FFFD;
      u16_size = 1;
    }
    else
    {
      /* u32_source is a BMP Character */
      *u16_characters = u32_character;
      u16_size = 1;
    }
  }
  else if (u32_character > 0x0010FFFF)
  {
    /* U+10FFFF is the largest code point of Unicode Character Set */
    *u16_characters = 0x0000FFFD;
    u16_size = 1;
  }
  else
  {
    /* u32_source is a character in range 0xFFFF - 0x10FFFF */
    u16_characters[0] = ((u32_character - 0x0010000UL) >> 10) + 0xD800;
    u16_characters[1] = ((u32_character - 0x0010000UL) & 0x3FFUL) + 0xDC00;
    u16_size = 2;
  }
}

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

void PrintOnConsoleWindow(const vector<u16string> &contents)
{
  HWND hconsole = GetConsoleWindow();
  HDC hdc = GetDC(hconsole);
  HFONT hfont = CreateFontW(50, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, L"Ink Free");
  HFONT oldfont = (HFONT)SelectObject(hdc, hfont);

  for (int index = 0; const auto & content : contents)
    TextOutW(hdc, 20, ++index * 100, (LPWSTR)content.c_str(), content.length());

  SelectObject(hdc, oldfont);
  DeleteObject(hfont);
  ReleaseDC(hconsole, hdc);
}

void WriteUtf8()
{
  ofstream file{ "TestFileUtf8.txt", ios::out | ios::binary };

  if (file.is_open())
  {
    u8string content =
      u8"\U0001F44B\U0001F3FB \U0001F596\U0001F3FF \U0001F44C\U0001F3FB \U0001F447\U0001F3FF \n"
      u8"\U0001F600 \U0001F601 \U0001F607 \U0001F911 \U0001F637 \U00002620 \U0001F47B \U0001F91F \U0001F448 \U0001F469 \n"
      u8"\U0001F469\U0000200D\U0001F3A4 \U0001F9D1\U0000200D\U0001F3A8 \U0001F9D1\U0000200D\U00002708\U0000FE0F \U0001F468\U0000200D\U0001F468\U0000200D\U0001F466\U0000200D\U0001F466";

    file << reinterpret_cast<cstr_t>(content.c_str());
    file.close();
  }
}

void ReadUtf8()
{
  ifstream file{ "TestFileUtf8.txt", ios::in | ios::binary };

  if (file.is_open())
  {
    vector<u16string> contents;

    file.seekg(0, ios::end);
    string u8content(file.tellg(), '\0');

    file.seekg(0, ios::beg);
    file.read(u8content.data(), u8content.size());
    file.close();

    u16string u16content;
    mbstate_t mbstate{ };
    char32_t c32;

    string::const_pointer u8ptr = u8content.data();
    string::pointer u8end = u8content.data() + u8content.size() + 1; //'\0'

    while (size_t rc = mbrtoc32(&c32, u8ptr, u8end - u8ptr, &mbstate))
    {
      if (rc == (size_t)-1)
        break;

      if (c32 == U'\n')
      {
        contents.emplace_back(move(u16content));
      }
      else
      {
        char16_t s16[2]; size_t size;
        ConvertUTF32ToUTF16(c32, s16, size);
        u16content.append(s16, size);
      }

      u8ptr += rc;
    }

    contents.emplace_back(move(u16content));
    PrintOnConsoleWindow(contents);
  }
}

void ReadUtf8v2()
{
  ifstream file{ "TestFileUtf8.txt", ios::in | ios::binary };

  if (file.is_open())
  {
    vector<u16string> contents;

    file.seekg(0, ios::end);
    u8string u8content(file.tellg(), u8'\0');

    file.seekg(0, ios::beg);
    file.read((LPSTR)(LPVOID)u8content.data(), u8content.size());
    file.close();

    auto &facet = use_facet<facet_utf16_t>(locale(".65001"));
    u16string u16content(u8content.size() / sizeof(char16_t) + sizeof(char16_t) * facet.max_length(), u'\0');

    mbstate_t mbstate{ };
    u8string::const_pointer from_next;
    u16string::pointer to_next;

    facet.in(mbstate,
             u8content.data(), u8content._Unchecked_end(), from_next,
             u16content.data(), u16content._Unchecked_end(), to_next);

    u16content.resize(to_next - u16content.data());
    u16string_view view = u16content;

    size_t pos;
    u16string_view::const_pointer end = view.data() + view.size() + 1;

    while ((pos = view.find(u'\n')) != u16string_view::npos)
    {
      u16string_view temp = view.substr(0, pos);
      contents.emplace_back(temp.data(), temp.size());
      view = u16string_view(temp._Unchecked_end() + 1, end - (temp._Unchecked_end() + 1));
    }

    contents.emplace_back(view.data(), view.size());
    PrintOnConsoleWindow(contents);
  }
}

void WriteUtf16()
{
  ofstream file{ "TestFileUtf16.txt", ios::out | ios::binary };

  if (file.is_open())
  {
    u16string content =
      u"\U0001F44B\U0001F3FB \U0001F596\U0001F3FF \U0001F44C\U0001F3FB \U0001F447\U0001F3FF \n"
      u"\U0001F600 \U0001F601 \U0001F607 \U0001F911 \U0001F637 \U00002620 \U0001F47B \U0001F91F \U0001F448 \U0001F469 \n"
      u"\U0001F469\U0000200D\U0001F3A4 \U0001F9D1\U0000200D\U0001F3A8 \U0001F9D1\U0000200D\U00002708\U0000FE0F \U0001F468\U0000200D\U0001F468\U0000200D\U0001F466\U0000200D\U0001F466";

    file.write(reinterpret_cast<cstr_t>(reinterpret_cast<LPCVOID>(content.c_str())), content.size() * sizeof(char16_t));
  }
}

void ReadUtf16()
{
  ifstream file{ "TestFileUtf16.txt", ios::in | ios::binary };

  if (file.is_open())
  {
    vector<u16string> contents;

    file.seekg(0, ios::end);
    u16string content(file.tellg() / sizeof(char16_t), u'\0');

    file.seekg(0, ios::beg);
    file.read((LPSTR)(LPVOID)content.data(), content.size() * sizeof(char16_t));
    file.close();

    u16string_view view = content;

    size_t pos = 0;
    char16_t *end = content.data() + content.size() + 1;

    while ((pos = view.find(u'\n')) != u16string::npos)
    {
      u16string_view temp = view.substr(0, pos);
      contents.emplace_back(temp.data(), temp.size());
      view = u16string_view(temp.data() + temp.size() + 1, end - (temp.data() + temp.size() + 1));
    }

    contents.emplace_back(view.data(), view.size());
    PrintOnConsoleWindow(contents);
  }
}

void WriteUtf32()
{
  ofstream file{ "TestFileUtf32.txt", ios::out | ios::binary };

  if (file.is_open())
  {
    u32string content =
      U"\U0001F44B\U0001F3FB \U0001F596\U0001F3FF \U0001F44C\U0001F3FB \U0001F447\U0001F3FF \n"
      U"\U0001F600 \U0001F601 \U0001F607 \U0001F911 \U0001F637 \U00002620 \U0001F47B \U0001F91F \U0001F448 \U0001F469 \n"
      U"\U0001F469\U0000200D\U0001F3A4 \U0001F9D1\U0000200D\U0001F3A8 \U0001F9D1\U0000200D\U00002708\U0000FE0F \U0001F468\U0000200D\U0001F468\U0000200D\U0001F466\U0000200D\U0001F466";

    file.write(reinterpret_cast<cstr_t>(reinterpret_cast<LPCVOID>(content.c_str())), content.size() * sizeof(char32_t));
  }
}

void ReadUtf32()
{
  ifstream file{ "TestFileUtf32.txt", ios::in | ios::binary };

  if (file.is_open())
  {
    vector<u16string> u16contents;

    file.seekg(0, ios::end);
    u32string u32content(file.tellg() / sizeof(char32_t), U'\0');

    file.seekg(0, ios::beg);
    file.read((LPSTR)(LPVOID)u32content.data(), u32content.size() * sizeof(char32_t));
    file.close();

    u32string_view view = u32content;

    size_t pos = 0;
    char32_t *end = u32content.data() + u32content.size() + 1;

    while ((pos = view.find(U'\n')) != u32string::npos)
    {
      u32string_view temp = view.substr(0, pos);
      u16contents.emplace_back(ConvertUTF32ToUTF16({ temp.data(), temp.size() }));
      view = u32string_view(temp.data() + temp.size() + 1, end - (temp.data() + temp.size() + 1));
    }

    u16contents.emplace_back(ConvertUTF32ToUTF16({ view.data(), view.size() }));
    PrintOnConsoleWindow(u16contents);
  }
}

void Test_01_FileOnUtf8()
{
  // https://unicode.org/emoji/charts/full-emoji-list.html
  // https://unicode.org/emoji/charts/full-emoji-modifiers.html

  WriteUtf8();
  ReadUtf8v2();

  /*WriteUtf16();
  ReadUtf16();*/

  /*WriteUtf32();
  ReadUtf32();*/
}


