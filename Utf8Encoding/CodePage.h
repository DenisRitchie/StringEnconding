#pragma once
#include <Windows.h>
#include <iostream>

using namespace std;

// https://docs.microsoft.com/en-us/windows/win32/api/winnls/nf-winnls-getcpinfoexw
// https://docs.microsoft.com/en-us/windows/win32/intl/code-page-identifiers
BOOL CALLBACK CodePageEnumProc(LPWSTR CodePage)
{
  /*CPINFOEXW CpInfo;
  GetCPInfoExW(CP_OEMCP, NULL, &CpInfo);*/

  wcout
    << L"\tCode Page: " << CodePage
    << endl;
  return TRUE;
}

void SetCodePage()
{
  // https://docs.microsoft.com/en-us/windows/win32/intl/code-page-identifiers
  // EnumSystemCodePagesW(CodePageEnumProc, CP_INSTALLED);

  // Unicode UTF-16, little endian byte order (BMP of ISO 10646); available only to managed applications
  #define CP_UTF16_LITTLE 1200

  // Unicode UTF-16, big endian byte order; available only to managed applications
  #define CP_UTF16_BIG 1201

  // MAC Roman; Western European (Mac)
  #define CP_MACINTOSH 10000

  if (IsValidCodePage(CP_UTF16_LITTLE))
  {
    SetConsoleCP(CP_UTF16_LITTLE);
    SetConsoleOutputCP(CP_UTF16_LITTLE);
  }
  else if (IsValidCodePage(CP_UTF16_BIG))
  {
    SetConsoleCP(CP_UTF16_BIG);
    SetConsoleOutputCP(CP_UTF16_BIG);
  }
  /*else if (IsValidCodePage(CP_MACINTOSH))
  {
    SetConsoleCP(CP_MACINTOSH);
    SetConsoleOutputCP(CP_MACINTOSH);
  }*/
  else if (IsValidCodePage(CP_UTF8))
  {
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
  }
}
