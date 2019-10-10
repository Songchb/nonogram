#include "nemo.h"
#include "fileReader.cpp"

#include <iostream>
#include <time.h> // calculate execution time

using namespace std;

int ROW_SIZE, COL_SIZE; // length of row & column
bool GRID = false;
bool PARAM = true;
bool BRUTEFORCE = false;
bool CLEAR = true;
vector<vector<int> > row; // parameters as global variable
vector<vector<int> > col;
wchar_t direc[] = L"→↓←↑";

int main(int argc, char* argv[]) {
  clock_t start, end;
  FileReader InputReader;

  if(argc == 1 || argv[1][0] == '-')  // 가장 첫 아규먼트가 파일 이름이 아니라 옵션이면
    InputReader.fileRead("std");      // 퍼즐을 직접 입력 받는다
  else
    InputReader.fileRead(argv[1]);    // 파일로 입력 받는다

  row = InputReader.getRow();
  col = InputReader.getCol();
  ROW_SIZE = col.size();
  COL_SIZE = row.size();

  for(int i=2; i<argc; ++i) {
    if(strcmp(argv[i], "-np") == 0 || strcmp(argv[i], "--no-parameters") == 0)
      PARAM = false;
    else if(strcmp(argv[i], "-g") == 0 || strcmp(argv[i], "--grid") == 0)
      GRID = true;
    else if(strcmp(argv[i], "-bf") == 0 || strcmp(argv[i], "--brute-force") == 0)
      BRUTEFORCE = true;
    else if(strcmp(argv[i], "-nc") == 0 || strcmp(argv[i], "--no-clear") == 0)
      CLEAR = false;
    else
      cout << "wrong option format : " << argv[i] << "\n";
  }

  cout << "\nfile read done\n";
  setlocale(LC_ALL, "korean");
  // InputReader.printRowCol();

  start = clock();
  Nemo nemo;
  // nemo.printBoard(nemo.board, COL_SIZE);

/*
  int list1[30] = {
    -1, 1, 1, -1, 1,
    1, -1, -1, 1, 0,
    1, 0, 1, 1, 1,
    1, 1, 0, 0, 0,
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0
  };

  int list2[30] = {
    0, 0, -1, 1, 1,
    1, 1, 1, 1, 1,
    -1, 1, 1, 1, 1,
    1, 1, 1, 1, 1,
    1, 1, -1, 1, -1,
    -1, 0, 0, 0, 0
  };

  int list3[30] = {
    0, 0, 0, 0, 0,
    -1, 1, 0, 1, 1,
    1, 1, 1, 1, 1,
    1, 0, 0, -1, 1,
    1, 0, 1, 1, 1,
    1, 1, 0, 0, 0
  };

  int list4[30] = {
    0, 0, 0, 0, -1,
    -1, 0, 0, 0, 0,
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0,
    -1, 0, -1, 0, 1,
    0, 0, 0, 0, 0
  };

  int list5[25] = {
    0, 0, 0, -1,
    0, 0, 0, 0, 0,
    -1, 0, 0, 0, 1,
    0, 0, -1, 0, 0,
    0, 0, 0, 0, 0, 0
  };

  int list6[30] = {
    -1, 1, 1, 1, 1,
    1, 1, 1, -1, 0,
    0, 0, 1, 1, -1,
    0, 0, 0, 0, 0,
    0, 0, 0, 0, -1,
    -1, 1, 1, -1, 1
  };

  int list7[30] = {
    -1, -1, -1, 1, 1,
    1, 1, 1, 1, 1,
    1, -1, -1, 1, 1,
    1, 1, 1, 1, 1,
    1, 1, 1, 1, -1,
    1, 0, 0, 0, 0
  };

  vector<int> temp;
  for(int i = 0; i < 30; ++i)
    temp.push_back(list7[i]);

  int listRow1[5] = {
    4, 2, 3, 9, 2
  };
  int listRow2[5] = {
    4, 8, 11, 2, 1
  };
  int listRow3[5] = {
    4, 1, 2, 11, 8
  };

  int listRow4[5] = {
    4, 2, 2, 10, 4
  };

  int listRow5[4] = {
    3, 3, 6, 4
  };

  int listRow6[7] = {
    6, 7, 2, 2, 2, 2, 1
  };

  int listRow7[5] = {
    4, 8, 11, 2, 1
  };


  vector<int> tempRow;
  for(int i=0; i<5; ++i)
    tempRow.push_back(listRow7[i]);

  for(int i = 1; i <= tempRow[0]; ++i)
    cout << tempRow[i] << " ";
  nemo.printLine(temp);

  // temp = nemo.rules1(tempRow, temp);
  temp = nemo.rules2(tempRow, temp, false);
  // temp = nemo.rules4(tempRow, temp);

  for(int i = 1; i <= tempRow[0]; ++i)
    cout << tempRow[i] << " ";
  nemo.printLine(temp);

*/
  nemo.run();
  // cout << "\n";
  end = clock();
  nemo.printBoard(nemo.board, COL_SIZE);

  cout << "execution time : " << (double)(end-start) / 1000 << "s\n";

  return 0;
}
