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

class FileReader {
private:
  ifstream input;
  string tempLine;
  bool is_std;
public:
  FileReader() {
    is_std = true;
  }

  fileRead(string inputFileName) {
    if(inputFileName.compare("std") != 0) { // file input
      is_std = false;
      input.open(inputFileName);
      if(!input.is_open()) {
        cout << "file open error : " << inputFileName << endl;
        exit(1);
      }
      cin.rdbuf(input.rdbuf());
    }
    if(is_std)
      cout << "Puzzle Size : ";
    cin >> P_SIZE;
    cin.ignore(1000, '\n');

    for(int i = 0; i < P_SIZE * 2; ++i) {
      if(is_std && i == 0) cout << "****** row ******\n";
      if(is_std && i == P_SIZE) cout << "****** col ******\n";
      getline(cin, tempLine);
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
    if(col.size() != P_SIZE) {
      cout << "input file is invalid!\n";
      exit(1);
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

class PMap {
public:
  vector<vector<int> > map;
  PMap() {
    vector<int> temp;
    for(int i=0; i<P_SIZE; ++i) {
      for(int j=0; j<P_SIZE; ++j)
        temp.push_back(0);
      map.push_back(temp);
    }
  }

  PMap(const PMap& a) {
    map = a.map;
  }


};

class Nemo {
public:
  // PMap p;
  vector<int> combVector;
  int combCount;
  vector<vector<int> > map;

  Nemo() {
    combCount = 0;
    for(int i=0; i<P_SIZE; ++i) {
      combVector.push_back(0);
      vector<int> temp;
      for(int j=0; j<P_SIZE; ++j)
        temp.push_back(0);
      map.push_back(temp);
    }
  }

  void run() {
    initialFill();

  }

  // PMap initialFill() {
    // PMap temp;
  void initialFill() {
    int* tempVector = new int[P_SIZE];
    for(int i=0; i<P_SIZE; i++)
      tempVector[i]=0;  // initialize tempVector with 0
                        // tempVector is used for making candidate of combination of
    for(int step = 0; step < P_SIZE*2; step++) {
      combCount = 0;
      for(int i=0; i<P_SIZE; ++i)
        combVector[i] = 0;  // initialize combVector with 0
                            // combVector is used for summation of all of the combination
      cout << "step : " << step << " // tempVector : ";
      for(int i=0; i<P_SIZE; ++i)
        if(tempVector[i]) wprintf(L"■");
        else wprintf(L"□");
      cout << "\n";

      if(step < P_SIZE)
        generateCombination(row, step, 0, tempVector, 0, false);
      else {
        generateCombination(col, step - P_SIZE, 0, tempVector, 0, false);
      }

      for(int i = 0; i < P_SIZE; ++i) {
        if(combVector[i] == combCount) {
          if(step < P_SIZE)
            map[step][i] = 1;
          else
            map[i][step] = 1;
        }
      }
      cout << "line is over\n";
    }

    delete[] tempVector;
  }

  void generateCombination(vector<vector<int> > rowcol, int step, int elementCount, int* tempVector, int position, bool previousCellPlaced) {
    if(position >= P_SIZE) { // end of the list
      if(rowcol[step][0] <= elementCount){ // count of the block of list is not matched with elementCount
        combCount++;
        for(int i=0; i<P_SIZE; ++i) {
          if(tempVector[i]) wprintf(L"■");
          else wprintf(L"□");
          if(tempVector[i]) // iterating result of the combination, count up the combVector
            combVector[i]++;
        } cout << "\n";
      }
      return;
    }

    int rowSum = 0;
    for(int i = elementCount+1; i<=rowcol[step][0]; i++) {
      if(i != elementCount+1)
        rowSum += 1;
      rowSum += rowcol[step][i];
    }

    if(P_SIZE - position >= rowSum) {
      // place block at current position
      if(!previousCellPlaced && rowcol[step][0] > elementCount){
        int* tempVector1 = new int[P_SIZE];
        for(int i=0; i<P_SIZE; i++)
          tempVector1[i] = tempVector[i];

        int pos = position;
        for(int i=0; i<rowcol[step][elementCount+1]; i++)
          tempVector1[pos++] = 1;

        generateCombination(rowcol, step, elementCount+1, tempVector1, pos, true);
      }

      // put a space at current position
      int* tempVector2 = new int[P_SIZE];
      for(int i=0; i<P_SIZE; i++)
        tempVector2[i] = tempVector[i];
      tempVector2[position] = 0;
      generateCombination(rowcol, step, elementCount, tempVector2, position+1, false);
    }
  }

  bool check(vector<vector<int> > map) {
    bool result = true;
    for(int i = 0; result && i < P_SIZE; ++i) {
      checlRowCol(map, row, i);
      checlRowCol(map, col, i);
    }

    return result;
  }

  bool checlRowCol(vector<vector<int> > map, vector<vector<int> > rowcol, int target) {
    bool blockStart = false;
    int blockSum = 0;
    int cnt = 0;

    for(int i = 0; i < P_SIZE; ++i) {
      if(map[i][target] == 1) { // block founded
        blockStart = true;
        blockSum += 1;
        if(rowcol[target][cnt] < blockSum)
          return false;
      } else if(blockStart) { // no block and previous cell was block
        if(rowcol[target][cnt] != blockSum)
          return false;
        cnt++;
        blockSum = 0;
        blockStart = false;
      }
    }
    return true;
  }

  void printMap() {
    for(int i = 0; i < P_SIZE; i++) {
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
  // InputReader.printRowCol();
  setlocale(LC_ALL, "korean");

  Nemo nemo;
  nemo.printMap();
  nemo.run();
  cout << "\n\n";
  nemo.printMap();

  return 0;
}
