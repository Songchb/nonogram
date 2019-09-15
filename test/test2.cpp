#include <iostream>
#include <cstdio>
#include <string.h>

#include <Windows.h>
#include <iomanip>
#include <locale>

using namespace std;

int main() {
  setlocale(LC_ALL, "korean");

  wchar_t character[] = L" │ ┼ 안녕하세요";
  wprintf(L"%s\n", character);
  wprintf(L"1★\n");
  wprintf(L"2▩▩▩□□■▩□▩▩\n");
  wprintf(L"3■■▩▩▩▩▩▩▩□\n");
  wprintf(L"4▣\n");
  wprintf(L"5▤\n");
  wprintf(L"6▥\n");
  wprintf(L"7▨\n");
  wprintf(L"8▧\n");
  wprintf(L"9▦\n");
  wprintf(L"10▩\n");
  wprintf(L"11▒\n");

  return 0;
}
