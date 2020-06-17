#pragma once
#include <iostream>
#include <cuchar>
#include <ranges>
#include <concepts>
#include <span>

using namespace std;

using cstr_t = const char *;
using facet_wchar_t = codecvt<wchar_t, char, mbstate_t>;
using facet_utf16_t = codecvt<char16_t, char8_t, mbstate_t>;
using facet_utf32_t = codecvt<char32_t, char8_t, mbstate_t>;

////////////////////////////////////////////////////////////////////////////

ostream &operator<<(ostream &out, const wchar_t *const pointer)
{
  auto &facet = use_facet<facet_wchar_t>(locale());
  const size_t size = char_traits<wchar_t>::length(pointer);

  string external = string(size * facet.max_length(), '\0');
  mbstate_t mbstate{ };
  const wchar_t *from_next;
  char *to_next;

  if (facet_wchar_t::result result
      = facet.out(mbstate,
                  pointer, pointer + size, from_next,
                  external.data(), external.data() + external.size(), to_next);
      result == facet_wchar_t::ok)
  {
    external.resize(to_next - external.data());
    return out << external;
  }

  return out;
}

ostream &operator<<(ostream &out, const wstring &text)
{
  auto &facet = use_facet<facet_wchar_t>(locale());

  string external = string(text.length() * facet.max_length(), '\0');
  mbstate_t mbstate{ };
  const wchar_t *from_next;
  char *to_next;

  if (facet_wchar_t::result result
      = facet.out(mbstate,
                  text.data(), text.data() + text.size(), from_next,
                  external.data(), external.data() + external.size(), to_next);
      result == facet_wchar_t::ok)
  {
    external.resize(to_next - external.data());
    return out << external;
  }

  return out;
}

ostream &operator<<(ostream &out, const wchar_t letter)
{
  const wchar_t text[2] = { letter, L'\0' };
  return out << text;
}

////////////////////////////////////////////////////////////////////////////

ostream &operator<<(ostream &out, const char8_t *const pointer)
{
  return out << reinterpret_cast<cstr_t>(pointer);
}

ostream &operator<<(ostream &out, const u8string &text)
{
  return out << text.c_str();
}

ostream &operator<<(ostream &out, const char8_t letter)
{
  const char8_t text[2] = { letter, u8'\0' };
  return out << text;
}

////////////////////////////////////////////////////////////////////////////

ostream &operator<<(ostream &out, const char16_t *const pointer)
{
  auto &facet = use_facet<facet_utf16_t>(locale(".65001"));
  const size_t size = char_traits<char16_t>::length(pointer);

  u8string external = u8string(size * facet.max_length(), u8'\0');
  mbstate_t mbstate{ };
  const char16_t *from_next;
  char8_t *to_next;

  if (facet_utf16_t::result result
      = facet.out(mbstate,
                  pointer, pointer + size, from_next,
                  external.data(), external.data() + external.size(), to_next);
      result == facet_utf16_t::ok)
  {
    external.resize(to_next - external.data());
    return out << external;
  }

  return out;
}

ostream &operator<<(ostream &out, const u16string &text)
{
  auto &facet = use_facet<facet_utf16_t>(locale(".65001"));

  u8string external = u8string(text.length() * facet.max_length(), u8'\0');
  mbstate_t mbstate{ };
  const char16_t *from_next;
  char8_t *to_next;

  if (facet_utf16_t::result result
      = facet.out(mbstate,
                  text.data(), text.data() + text.size(), from_next,
                  external.data(), external.data() + external.size(), to_next);
      result == facet_utf16_t::ok)
  {
    external.resize(to_next - external.data());
    return out << external;
  }

  return out;
}

ostream &operator<<(ostream &out, const char16_t letter)
{
  const char16_t text[2] = { letter, u'\0' };
  return out << text;
}

////////////////////////////////////////////////////////////////////////////

ostream &operator<<(ostream &out, const char32_t *const pointer)
{
  auto &facet = use_facet<facet_utf32_t>(locale(".65001"));
  const size_t size = char_traits<char32_t>::length(pointer);

  u8string external = u8string(size * facet.max_length(), u8'\0');
  mbstate_t mbstate{ };
  const char32_t *from_next;
  char8_t *to_next;

  if (facet_utf32_t::result result
      = facet.out(mbstate,
                  pointer, pointer + size, from_next,
                  external.data(), external.data() + external.size(), to_next);
      result == facet_utf16_t::ok)
  {
    external.resize(to_next - external.data());
    return out << external;
  }

  return out;
}

ostream &operator<<(ostream &out, const u32string &text)
{
  auto &facet = use_facet<facet_utf32_t>(locale(".65001"));

  u8string external = u8string(text.length() * facet.max_length(), u8'\0');
  mbstate_t mbstate{ };
  const char32_t *from_next;
  char8_t *to_next;

  if (facet_utf32_t::result result
      = facet.out(mbstate,
                  text.data(), text.data() + text.size(), from_next,
                  external.data(), external.data() + external.size(), to_next);
      result == facet_utf16_t::ok)
  {
    external.resize(to_next - external.data());
    return out << external;
  }

  return out;
}

ostream &operator<<(ostream &out, const char32_t letter)
{
  const char32_t text[2] = { letter, U'\0' };
  return out << text;
}

////////////////////////////////////////////////////////////////////////////

std::string to_bytes(const std::u8string_view &str_view)
{
  std::string bytes;

  return bytes;
}

std::string to_bytes(const std::u16string_view &str_view)
{
  std::string bytes;

  return bytes;
}

std::string to_bytes(const std::u32string_view &str_view)
{
  std::string bytes;

  return bytes;
}

std::string to_bytes(const std::wstring_view &str_view)
{
  std::string bytes;

  return bytes;
}
