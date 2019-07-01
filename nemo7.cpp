#include <iostream>
#include <fstream>
#include <sstream>  // parse input file
#include <vector>
#include <string>
#include <windows.h>
#include <time.h> // calculate execution time
#include <algorithm>  // min
#define RECORD false


using namespace std;

int ROW_SIZE, COL_SIZE;
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
      if(inputFileName.find('.') == string::npos)
        inputFileName += ".txt";
      input.open(inputFileName);
      if(!input.is_open()) {
        cout << "file open error : " << inputFileName << endl;
        exit(1);
      }
      cin.rdbuf(input.rdbuf());
    }
    if(is_std)
      cout << "row size, column size (without comma, just space) : ";
    cin >> ROW_SIZE >> COL_SIZE;
    cin.ignore(1000, '\n');

    for(int i = 0; i < ROW_SIZE + COL_SIZE; ++i) {
      if(is_std && i == 0) cout << "****** row ******\n";
      if(is_std && i == COL_SIZE) cout << "****** col ******\n";
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
      if(i < COL_SIZE)
        row.push_back(tempVector);
      else
        col.push_back(tempVector);
    }
    if(col.size() != ROW_SIZE) {
      cout << "input file is invalid!\n";
      exit(1);
    }
  }

  printRowCol() {
    cout << ROW_SIZE << " " << COL_SIZE << "\n";
    for(int i=0; i<COL_SIZE; ++i) {
      cout << row[i][0] << " : ";
      for(int j = 1; j<row[i].size(); ++j) {
        cout << row[i][j] << " ";
      }cout << "\n";
    }

    for(int i=0; i<ROW_SIZE; ++i) {
      cout << col[i][0] << " : ";
      for(int j = 1; j<col[i].size(); ++j) {
        cout << col[i][j] << " ";
      }cout << "\n";
    }
  }
};

class Nemo {
public:
  vector<int> combVector;
  int combCount;
  vector<vector<int> > map;
  bool certain;

  Nemo() {
    certain = false;
    combCount = 0;
    for(int i=0; i<COL_SIZE; ++i) {
      combVector.push_back(0);

      // initialize map
      vector<int> temp;
      for(int j=0; j<ROW_SIZE; ++j)
        temp.push_back(0);
      map.push_back(temp);
    }
  }

  void run() {
    for(int step = 0; step < ROW_SIZE + COL_SIZE; step++) {
      if(step < COL_SIZE)
        initialFill(row, step, true);
      else
        initialFill(col, step - COL_SIZE, false);
    }

    // 두번째 단계
  }

  void initialFill(vector<vector<int> > rowcol, int step, bool is_row) {
    int sum = rowcol[step][0] - 1;
    for(int i = 1; i <= rowcol[step][0]; ++i)
      sum += rowcol[step][i];

    int position = 0;
    int gap = (is_row ? ROW_SIZE : COL_SIZE) - sum;
    for(int i = 1; i <= rowcol[step][0]; ++i) { // 개수만큼 돌면서
      for(int j = 0; j < min(rowcol[step][i], gap); ++j)
        position++;
      for(int j = gap; j < rowcol[step][i]; ++j) { // 해당 블록만큼 칠하기
        if(is_row)
          map[step][position++] = 1;
        else
          map[position++][step] = 1;
      }
      if(gap == 0) { // 칸이 딱 들어맞다면 빈칸이 확정이므로 -1로 표시해둔다
        if(is_row)
          map[step][position++] = -1;
        else
          map[position++][step] = -1;
      }
      else
        position++;
    }
  }

  void generateCombination(vector<vector<int> > rowcol, int step, int elementCount, vector<int> tempVector, int position, bool previousCellPlaced) {
    if(certain) return;
    if(position >= ROW_SIZE) { // end of the list
      if(rowcol[step][0] <= elementCount){ // count of the block of list is not matched with elementCount
        combCount++;
        for(int i=0; i<ROW_SIZE; ++i) {
          // if(tempVector[i]) wprintf(L"■");
          // else wprintf(L"□");
          if(tempVector[i]) // iterating result of the combination, count up the combVector
            combVector[i]++;
        } //cout << endl;

        certain = true;
        for(int i=0; i<ROW_SIZE; ++i) {
          if(combVector[i] == combCount)
            certain = false;
        }
      }
      return;
    }

    int rowSum = 0;
    for(int i = elementCount+1; i<=rowcol[step][0]; i++) {
      if(i != elementCount+1)
        rowSum += 1;
      rowSum += rowcol[step][i];
    }

    if(ROW_SIZE - position >= rowSum) {
      // place block at current position
      if(!previousCellPlaced && rowcol[step][0] > elementCount){
        vector<int> tempVector1;
        for(int i=0; i<ROW_SIZE; i++)
          tempVector1.push_back(tempVector[i]);

        int pos = position;
        for(int i=0; i<rowcol[step][elementCount+1]; i++)
          tempVector1[pos++] = 1;

        generateCombination(rowcol, step, elementCount+1, tempVector1, pos, true);
      }

      // put a space at current position
      vector<int> tempVector2;
      for(int i=0; i<ROW_SIZE; i++)
        tempVector2.push_back(tempVector[i]);
      tempVector2[position] = 0;
      generateCombination(rowcol, step, elementCount, tempVector2, position+1, false);
    }
  }

  bool check(vector<vector<int> > map) {
    bool result = true;
    for(int i = 0; result && i < ROW_SIZE; ++i) {
      checlRowCol(map, row, i);
      checlRowCol(map, col, i);
    }

    return result;
  }

  bool checlRowCol(vector<vector<int> > map, vector<vector<int> > rowcol, int target) {
    bool blockStart = false;
    int blockSum = 0;
    int cnt = 0;

    for(int i = 0; i < ROW_SIZE; ++i) {
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
    for(int i = 0; i < COL_SIZE; i++) {
      for(int j = 0; j < ROW_SIZE; j++) {
        if(RECORD && j != 0 && j % 5 == 0) {
          if((i+1)%5 != 0)  wprintf(L"│");
          else              wprintf(L"┼");
        }
        if(map[i][j] == 1)        wprintf(L"■");
        else if(map[i][j] == -1)  wprintf(L"▨");
        else                      wprintf(L"□");
      } cout << "\n";
    } cout << "\n";
  }
};

int main(int argc, char* argv[]) {
  clock_t start, end;
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
  setlocale(LC_ALL, "korean");
  // InputReader.printRowCol();

  start = clock();
  Nemo nemo;
  // nemo.printMap();
  nemo.run();
  // cout << endl;
  end = clock();
  nemo.printMap();

  cout << (double)(end-start) / 1000 << "s" << endl;

  return 0;
}
