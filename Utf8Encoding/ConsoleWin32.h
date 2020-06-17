#pragma once
#include <Windows.h>
#include <string>

using namespace std;

DWORD WINAPI ThreadProc(LPVOID lpParameter)
{
  HWND hconsole = (HWND)lpParameter;
  u16string text = u"\U0001F630 \U00002620 \U0001F637 \U0001F9D9 \n\n";

  while (true)
  {
    HDC hdc = GetDC(hconsole);
    HFONT hfont, hfontold;

    hfont = CreateFontW(90, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (LPCWSTR)u"Times New Roman");

    hfontold = (HFONT)SelectObject(hdc, hfont);

    TextOutW(hdc, 100, 150, (LPCWSTR)text.c_str(), text.length());

    SelectObject(hdc, hfontold);
    DeleteObject(hfont);
    ReleaseDC(hconsole, hdc);

    Sleep(500);
  }
  return 0;
}

void ConsoleShowText()
{
  // https://unicode.org/emoji/charts/full-emoji-list.html

  HWND hconsole = GetConsoleWindow();
  DWORD threadid;
  CreateThread(0, 0, ThreadProc, hconsole, 0, &threadid);
  _getch();
}
