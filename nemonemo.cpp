#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <queue>
#include <windows.h>
#include <locale>
#define forRecord 0

using namespace std;

void ClearScreen() {
  HANDLE                     hStdOut;
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  DWORD                      count;
  DWORD                      cellCount;
  COORD                      homeCoords = { 0, 0 };

  hStdOut = GetStdHandle( STD_OUTPUT_HANDLE );
  if (hStdOut == INVALID_HANDLE_VALUE) return;

  /* Get the number of cells in the current buffer */
  if (!GetConsoleScreenBufferInfo( hStdOut, &csbi )) return;
  cellCount = csbi.dwSize.X *csbi.dwSize.Y;

  /* Fill the entire buffer with spaces */
  if (!FillConsoleOutputCharacter(
    hStdOut,
    (TCHAR) ' ',
    cellCount,
    homeCoords,
    &count
    )) return;

  /* Fill the entire buffer with the current colors and attributes */
  if (!FillConsoleOutputAttribute(
    hStdOut,
    csbi.wAttributes,
    cellCount,
    homeCoords,
    &count
    )) return;

  /* Move the cursor home */
  SetConsoleCursorPosition( hStdOut, homeCoords );
}

int puzzleSize = 0;
int** row;
int** col;
bool found = false;
vector< vector<int*> > cv;

void printMap(int** map, int rowStep) {
  for(int i=0; i < rowStep + 1; i++) {
    for(int j=0; j<puzzleSize; j++) {
      if(forRecord == 1 && j != 0 && j % 5 == 0) {
        if((i+1)%5 != 0)  wprintf(L"│");
        else              wprintf(L"┼");
      }
      if(map[i][j] != 0)  wprintf(L"■");
      else                wprintf(L"□");
    }
    cout << "\n";
  }
  cout << "\n";
}

int** initializer(bool fileRead, int** map) {
  if(!fileRead) {
    cout << "puzzle size : ";
    cin >> puzzleSize;
  }

  map = new int*[puzzleSize];
  row = new int*[puzzleSize];
  col = new int*[puzzleSize];
  for(int i=0; i<puzzleSize; i++) {
    map[i] = new int[puzzleSize];
    row[i] = new int[puzzleSize/2 + 1];
    col[i] = new int[puzzleSize/2 + 1];
    for(int j=0; j<puzzleSize; j++) {
      map[i][j] = 0;  // initialize map
      if(j < puzzleSize/2 + 1) {
        row[i][j] = -1;
        col[i][j] = -1;
      }
    }
  }
  return map;
}

int** fileRead(string inputfile, int** map) {
  string concatInputfile = "Input/" + inputfile;
  if(concatInputfile.find('.') == string::npos) concatInputfile += ".txt";
  ifstream input(concatInputfile);
  if(input.is_open()) {
    input >> puzzleSize;
    map = initializer(true, map);

    string tempLine;
    input.ignore(1000, '\n');

    for(int i=0; i<puzzleSize; i++) {
      getline(input, tempLine);
      stringstream ss(tempLine);
      string word;
      int j = 0;
      while(ss >> word) row[i][++j] = atoi(word.c_str());
      row[i][0] = j;
    }

    for(int i=0; i<puzzleSize; i++) {
      getline(input, tempLine);
      stringstream ss(tempLine);
      string word;
      int j = 0;
      while(ss >> word) col[i][++j] = atoi(word.c_str());
      col[i][0] = j;
    }
  } else {
    cout << "file read fail!" << endl;
    exit(0);
  }
  return map;
}

void inputParameter() {
  string tempLine;
  cout << "****** row ******" << endl;
  cin.ignore(1000, '\n');
  for(int i=0; i<puzzleSize; i++) {
    cout << i+1 << "th row : ";
    getline(cin, tempLine);
    stringstream ss(tempLine);
    string word;
    int j = 0;
    while(ss >> word) row[i][++j] = atoi(word.c_str());
    row[i][0] = j;
  }

  cout << "****** col ******" << endl;
  for(int i=0; i<puzzleSize; i++) {
    cout << i+1 << "th col : ";
    getline(cin, tempLine);
    stringstream ss(tempLine);
    string word;
    int j = 0;
    while(ss >> word) col[i][++j] = atoi(word.c_str());
    col[i][0] = j;
  }
}

bool checkCol(int rowStep, int** map) {
    // printMap(map, rowStep);
    // getchar();
  bool start = false;
  int blockSum = 0;
  int cnt = 1;
  // check validation from each column
  for(int targetCol = 0; targetCol < puzzleSize; targetCol++, start = false, blockSum = 0, cnt = 1) {
    for(int i=0; i<=rowStep; i++) {
      // if(rowStep+1 == puzzleSize && targetCol == 5) {
      //   cout << "rowStep : " << i << ", map[" << i << "][5] : " << map[i][targetCol] << endl;
      // }
      if(map[i][targetCol] == 1) {
        start = true;
        blockSum += 1;
        // if(rowStep+1 == puzzleSize && targetCol == 5) {
        //   cout << "    col[5][0] : " << col[targetCol][0]
        //    << ", cnt : " << cnt
        //     << ", col[5][cnt] : " << col[targetCol][cnt]
        //      << ", blockSum : " << blockSum << endl;
        // }
        if(col[targetCol][cnt] < blockSum)
          return false;
      }
      else if(start) {
        if(col[targetCol][cnt] != blockSum)
          return false;
        cnt++;
        blockSum = 0;
        start = false;
      }
    }
  }
  if(rowStep + 1 == puzzleSize) {
    cout << " **** complete **** " << endl;
    found = true;
    printMap(map, rowStep);
    return false;
  }
/*
  for(int targetCol = 0; targetCol < puzzleSize; targetCol++) {
    int colCount = 0;
    for(int i = 1; i <= col[targetCol][0]; i++) {
      colCount += col[targetCol][i];
    }

    int mapColCount = 0;
    for(int i = 0; i < puzzleSize; i++)
      if(map[i][targetCol]) mapColCount++;
    if(colCount < mapColCount) {
      return false;
    }
    */

  return true;
}

void generateCombination(int rowStep, int elementCount, int* tempRow, int position, bool previousCellPlaced) {
  if(position >= puzzleSize) {
    if(row[rowStep][0] <= elementCount)
      cv[rowStep].push_back(tempRow);
    return;
  }

  int rowSum = 0;
  for(int i = elementCount+1; i<=row[rowStep][0]; i++) {
    if(i != elementCount+1)
      rowSum += 1;
    rowSum += row[rowStep][i];
  }

  if(puzzleSize - position >= rowSum) {
    // place block at current position
    if(!previousCellPlaced && row[rowStep][0] > elementCount){
      int* tempRow1 = new int[puzzleSize];
      for(int i=0; i<puzzleSize; i++)
        tempRow1[i] = tempRow[i];

      int pos = position;
      for(int i=0; i<row[rowStep][elementCount+1]; i++)
        tempRow1[pos++] = 1;

      generateCombination(rowStep, elementCount+1, tempRow1, pos, true);
    }

    // put a space at current position
    int* tempRow2 = new int[puzzleSize];
    for(int i=0; i<puzzleSize; i++)
      tempRow2[i] = tempRow[i];
    tempRow2[position] = 0;
    generateCombination(rowStep, elementCount, tempRow2, position+1, false);
  }
}

void storeQueueVector() {
  int* tempRow = new int[puzzleSize];
  for(int i=0; i<puzzleSize; i++)
    tempRow[i]=0;

  for(int rowStep=0; rowStep<puzzleSize; rowStep++) {
    vector<int*> q;
    cv.push_back(q);
    generateCombination(rowStep, 0, tempRow, 0, false);
  }

  delete[] tempRow;
}

void Nemonemo(int** map, int rowStep) {
  if(found)
    return;

  ClearScreen();
  printMap(map, rowStep);
  // cout << endl << rowStep + 1 << "th step : ";
  // for(int i=1; i<=row[rowStep][0]; i++)
  //   cout << row[rowStep][i] << " ";
  // cout << endl;

  for(int q = 0; q < cv[rowStep].size(); q++) {
    // 조합의 수 만큼 block의 위치를 옮겨가며 다음 스텝으로 넘어감
    int ** nextMap = new int*[puzzleSize];
    for(int i = 0; i < puzzleSize; i++) {
      nextMap[i] = new int[puzzleSize];
      for(int j = 0; j < puzzleSize; j++)
      nextMap[i][j] = map[i][j];
    }
    for(int i = 0; i < puzzleSize; i++) {
      nextMap[rowStep][i] = cv[rowStep][q][i];
    }
    if(checkCol(rowStep, nextMap)) Nemonemo(nextMap, rowStep + 1);

    for(int i = 0; i < puzzleSize; i++)
      delete[] nextMap[i];
    delete[] nextMap;
  }
}

int main(int argc, char* argv[]) {
  int** map;
  if(argc == 2)
    map = fileRead(argv[1], map);
  else {
    cout << " You can input text file next time\n";
    cout << "   ex) ./nemo input.txt\n";
    map = initializer(false, map);
    inputParameter();
  }
  setlocale(LC_ALL, "korean");

  storeQueueVector();

  Nemonemo(map, 0);

  for(int i=0; i<puzzleSize; i++) {
    delete[] map[i];
    if(i < puzzleSize/2 + 1) {
      delete[] row[i];
      delete[] col[i];
    }
  }
  delete[] map;
  delete[] row;
  delete[] col;

  return 0;
}
