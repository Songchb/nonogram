#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <windows.h>
#define RECORD false


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

int P_SIZE;
vector<vector<int> > row;
vector<vector<int> > col;

void printMap(vector<vector<int> > map, int depth) {
  for(int i = 0; i < depth + 1; i++) {
    for(int j = 0; j < P_SIZE; j++) {
      if(RECORD && j != 0 && j % 5 == 0) {
        if((i+1)%5 != 0)  wprintf(L"│");
        else              wprintf(L"┼");
      }
      if(map[i][j] != 0)  wprintf(L"■");
      else                wprintf(L"□");
    } cout << "\n";
  } cout << "\n";
}

class FileReader {
private:
  ifstream input;
  string tempLine;
  bool is_std;
public:
  FileReader() {
  }

  fileRead(string inputFileName) {
    is_std = (inputFileName.compare("std") == 0) ? true : false;

    if(is_std) {
      cout << "Puzzle Size : ";
      cin >> P_SIZE;
      cin.ignore(1000, '\n');
    } else {
      input.open(inputFileName);
      if(!input.is_open()) {
        cout << "file open error : " << inputFileName << endl;
        exit(1);
      }
      input >> P_SIZE;
      input.ignore(1000, '\n');
    }

    for(int i = 0; i < P_SIZE * 2; ++i) {
      if(is_std && i == 0) cout << "****** row ******\n";
      if(is_std && i == P_SIZE) cout << "****** col ******\n";
      if(is_std) getline(cin, tempLine);
      else getline(input, tempLine);
      stringstream ss(tempLine);
      string word;
      int count = 0;
      vector<int> tempVector;
      tempVector.push_back(count);
      while(ss >> word) {
        tempVector.push_back(atoi(word.c_str()));
        tempVector[0]++;
      }
      if(i < P_SIZE)
        row.push_back(tempVector);
      else
        col.push_back(tempVector);
    }
  }

  printRowCol() {
    cout << P_SIZE << "\n";
    for(int i=0; i<P_SIZE; ++i) {
      cout << row[i][0] << " : ";
      for(int j = 1; j<row[i].size(); ++j) {
        cout << row[i][j] << " ";
      }cout << "\n";
    }

    for(int i=0; i<P_SIZE; ++i) {
      cout << col[i][0] << " : ";
      for(int j = 1; j<col[i].size(); ++j) {
        cout << col[i][j] << " ";
      }cout << "\n";
    }
  }
};

int main(int argc, char* argv[]) {
  FileReader InputReader;
  if(argc == 1) {
    InputReader.fileRead("std");
  } else if(argc == 2) {
    InputReader.fileRead(argv[1]);
  } else {
    cout << "** Wrong execution format! need 1 or no argument **\n";
    exit(1);
  }
  cout << "\nfile read done\n";
  InputReader.printRowCol();

  return 0;
}
