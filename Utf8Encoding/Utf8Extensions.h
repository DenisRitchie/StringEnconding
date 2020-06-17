#pragma once
#include <iostream>
#include <cuchar>
#include <ranges>
#include <concepts>
#include <span>
#include <utility>
#include <string>
#include <string_view>

using namespace std;

using cstr_t = const char *;
using facet_wchar_t = std::codecvt<wchar_t, char, std::mbstate_t>;
using facet_utf16_t = std::codecvt<char16_t, char8_t, std::mbstate_t>;
using facet_utf32_t = std::codecvt<char32_t, char8_t, std::mbstate_t>;


////////////////////////////////////////////////////////////////////////////

ostream &operator<<(ostream &out, wstring_view view) noexcept
{
  auto          &facet    = use_facet<facet_wchar_t>(locale());
  string         external = string(view.length() * facet.max_length(), '\0');
  mbstate_t      mbstate{ };
  const wchar_t *from_next;
  char          *to_next;

  if (facet_wchar_t::result result
      = facet.out(mbstate,
                  view.data(), view.data() + view.size(), from_next,
                  external.data(), external.data() + external.size(), to_next);
      result == facet_wchar_t::ok || result == facet_wchar_t::partial)
  {
    external.resize(to_next - external.data());
    return out << external;
  }

  return out;
}

ostream &operator<<(ostream &out, span<wchar_t> buffer) noexcept
{
  return out << wstring_view{ buffer.data(), buffer.size() };
}

ostream &operator<<(ostream &out, const wchar_t *const pointer) noexcept
{
  return out << wstring_view{ pointer };
}

ostream &operator<<(ostream &out, const wstring &text) noexcept
{
  return out << static_cast<wstring_view>(text);
}

ostream &operator<<(ostream &out, const wchar_t letter) noexcept
{
  const wchar_t text[2] = { letter, L'\0' };
  return out << wstring_view{ text, 1 };
}

////////////////////////////////////////////////////////////////////////////

ostream &operator<<(ostream &out, const u8string_view view) noexcept
{
  return out << reinterpret_cast<cstr_t>(view.data());
}

ostream &operator<<(ostream &out, const span<char8_t> buffer) noexcept
{
  return out << reinterpret_cast<cstr_t>(buffer.data());
}

ostream &operator<<(ostream &out, const char8_t *const pointer) noexcept
{
  return out << reinterpret_cast<cstr_t>(pointer);
}

ostream &operator<<(ostream &out, const u8string &text) noexcept
{
  return out << reinterpret_cast<cstr_t>(text.c_str());
}

ostream &operator<<(ostream &out, const char8_t letter) noexcept
{
  const char8_t text[2] = { letter, u8'\0' };
  return out << reinterpret_cast<cstr_t>(text);
}

////////////////////////////////////////////////////////////////////////////

ostream &operator<<(ostream &out, const u16string_view view) noexcept
{
  auto           &facet    = use_facet<facet_utf16_t>(locale(".65001"));
  u8string        external = u8string(view.length() * facet.max_length(), u8'\0');
  mbstate_t       mbstate{ };
  const char16_t *from_next;
  char8_t        *to_next;

  if (facet_utf16_t::result result
      = facet.out(mbstate,
                  view.data(), view.data() + view.size(), from_next,
                  external.data(), external.data() + external.size(), to_next);
      result == facet_utf16_t::ok || result == facet_utf16_t::partial)
  {
    external.resize(to_next - external.data());
    return out << external;
  }

  return out;
}

ostream &operator<<(ostream &out, const span<char16_t> buffer) noexcept
{
  return out << u16string_view{ buffer.data(), buffer.size() };
}

ostream &operator<<(ostream &out, const char16_t *const pointer) noexcept
{
  return out << u16string_view{ pointer };
}

ostream &operator<<(ostream &out, const u16string &text) noexcept
{
  return out << static_cast<u16string_view>(text);
}

ostream &operator<<(ostream &out, const char16_t letter) noexcept
{
  const char16_t text[2] = { letter, u'\0' };
  return out << u16string_view{ text, 1 };
}

////////////////////////////////////////////////////////////////////////////

ostream &operator<<(ostream &out, const u32string_view view) noexcept
{
  auto           &facet    = use_facet<facet_utf32_t>(locale(".65001"));
  u8string        external = u8string(view.length() * facet.max_length(), u8'\0');
  mbstate_t       mbstate{ };
  const char32_t *from_next;
  char8_t        *to_next;

  if (facet_utf32_t::result result
      = facet.out(mbstate,
                  view.data(), view.data() + view.size(), from_next,
                  external.data(), external.data() + external.size(), to_next);
      result == facet_utf32_t::ok || result == facet_utf32_t::partial)
  {
    external.resize(to_next - external.data());
    return out << external;
  }

  return out;
}

ostream &operator<<(ostream &out, const span<char32_t> buffer) noexcept
{
  return out << u32string_view{ buffer.data(), buffer.size() };
}

ostream &operator<<(ostream &out, const char32_t *const pointer) noexcept
{
  return out << u32string_view{ pointer };
}

ostream &operator<<(ostream &out, const u32string &text) noexcept
{
  return out << static_cast<u32string_view>(text);
}

ostream &operator<<(ostream &out, const char32_t letter) noexcept
{
  const char32_t text[2] = { letter, U'\0' };
  return out << u32string_view{ text, 1 };
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
