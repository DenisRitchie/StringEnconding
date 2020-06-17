#pragma once
#include <io.h>
#include <fcntl.h>
#include <iostream>

using namespace std;

void TestSetMode()
{
  // https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/setmode?view=vs-2019
  _setmode(_fileno(stdout), _O_U16TEXT);
  wcout << L"zß水🍌" << endl;
}
