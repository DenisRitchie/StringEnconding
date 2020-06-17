#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <CommCtrl.h>
#include <strsafe.h>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <thread>
#include <locale>
#include <ctime>

#include <string>

#include <cuchar>
#include <codecvt>

#include <memory>
#include <bit>

#include <conio.h>

#include "Utf8Extensions.h"
#include "CodePage.h"
#include "TestFacetOut.h"
#include "TestFacetIn.h"
#include "TestFacetOutUtf16.h"
#include "DetectEndian.h"
#include "InformacionNicaragua.h"
#include "TestSetMode.h"
#include "ConsoleWin32.h"
#include "Utf8Functions.h"
#include "FilesOnUtf8.h"

using namespace std;

#pragma comment(lib, "Comctl32.lib")
#pragma comment(lib, "user32.lib") 
#pragma comment(lib, "gdi32.lib")

void SmallInfo();
void DrawBorder();

int main()
{
  SetCodePage();
  // DetectEndian();
  // TestSetMode();
  // SmallInfo();
  // DrawBorder();

  // Utf16FunctionsTest_Utf16ToAnsi();
  // Utf32FunctionsTest_Utf32ToAnsi();
  // Utf16FunctionsTest_AnsiToUtf16();
  // Utf32FunctionsTest_AnsiToUtf32();

  // TestFacetOut();
  // TestFacetIn();
  // TestFacetOutUtf16();
  // ConsoleShowText();

  Test_01_FileOnUtf8();

  return EXIT_SUCCESS;
}


void SmallInfo()
{
  // https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/setlocale-wsetlocale?view=vs-2019
  std::locale::global(std::locale(".65001"));

  cout.imbue(locale("es_NI.utf-8"));

  cout << u8"El código de página de la consola es: " << GetConsoleCP() << endl;
  cout << u"El código de página de la consola es: " << GetConsoleCP() << endl;
  cout << L'\n';
  cout << U"La localidad usada es: " << cout.getloc().name() << L'\n';
}

void DrawBorder()
{
}
