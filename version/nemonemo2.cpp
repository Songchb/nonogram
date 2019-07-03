#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <queue>
#include <string.h>
#include <iomanip>
using namespace std;

int PSIZE = 0;
vector< vector<int> > map;
vector< vector<int> > row;
vector< vector<int> > col;
vector< vector<int*> > cv;
bool mapFounded = false;

void printMap() {
  for(int i=0; i < map.size(); i++) {
    for(int j=0; j<PSIZE; j++) {
      if(j != 0 && j%5 == 0)
        cout << ((i+1)%5 != 0 ? "│" : "┼");
      if(map[i][j] != 0)
        cout << "⬛️";
      else cout << "⬜️";
    }
    cout << endl;
  }
  cout << endl;
}

void printVector(vector< vector<int> > doubleVec) {
  cout << "size : " << doubleVec.size() << endl;
  for(int i=0; i<doubleVec.size(); i++) {
    for(int j=0; j<doubleVec.at(i).size(); j++) {
      cout << setw(4) << doubleVec.at(i).at(j);
    }
    cout << endl;
  }
}

void fileRead(string inputfile) {
  bool isFileInput = (inputfile.size() != 0);

  string concatInputfile = "Input/" + inputfile;
  if(concatInputfile.find('.') == string::npos) concatInputfile += ".txt";
  ifstream input(concatInputfile);

  if(!input.is_open()) {
    if(isFileInput) {
      cout << "file open error" << endl;
      exit(1);
    }
  } else cin.rdbuf(input.rdbuf());

  if(!isFileInput) cout << "puzzle size : ";
  cin >> PSIZE;
  string tempLine;
  vector<int> tempVector;
  cin.ignore(1000, '\n');

  if(!isFileInput) cout << "****** row ******" << endl;
  for(int i=0; i<PSIZE; i++) {
    if(!isFileInput) cout << i+1 << "th row : ";
    getline(cin, tempLine);
    stringstream ss(tempLine);
    string word;
    tempVector.clear();
    while(ss >> word) tempVector.push_back(atoi(word.c_str()));
    row.push_back(tempVector);
  }

  if(!isFileInput) cout << "****** col ******" << endl;
  for(int i=0; i<PSIZE; i++) {
    if(!isFileInput) cout << i+1 << "th col : ";
    getline(cin, tempLine);
    stringstream ss(tempLine);
    string word;
    tempVector.clear();
    while(ss >> word) tempVector.push_back(atoi(word.c_str()));
    col.push_back(tempVector);
  }
}

bool checkCol(vector< vector<int> > map) {
  bool blockStart = false;
  int blockSum = 0;
  int cnt = 0;
  cout << "map SIZE : " << map.size() << endl;
  // check validation from each column
  for(int targetCol = 0; targetCol < PSIZE; targetCol++, blockStart = false, blockSum = 0, cnt = 0) {
    for(int i=0; i<map.size(); i++) {
      cout << "targetCol : " << targetCol << ", i : " << i << ", map.at("<<i<<").at("<<targetCol<<") : " << map.at(i).at(targetCol) << endl;
      getchar();
      if(map.at(i).at(targetCol) == 1) { // found block
        blockStart = true;
        blockSum += 1;
        if(col.at(targetCol).at(cnt) < blockSum)
          return false;
      }
      else if(blockStart) { // no block and previous cell was block
        if(col.at(targetCol).at(cnt) != blockSum)
          return false;
        cnt++;
        blockSum = 0;
        blockStart = false;
      }
    }
  }
  if(map.size() + 1 == PSIZE) {
    cout << " **** complete **** " << endl;
    mapFounded = true;
    printMap();
    return false;
  }
/*
  for(int targetCol = 0; targetCol < PSIZE; targetCol++) {
    int colCount = 0;
    for(int i = 1; i <= col[targetCol][0]; i++) {
      colCount += col[targetCol][i];
    }

    int mapColCount = 0;
    for(int i = 0; i < PSIZE; i++)
      if(map[i][targetCol]) mapColCount++;
    if(colCount < mapColCount) {
      return false;
    }
    */

  return true;
}

void generateCombination(int rowStep, int elementCount, int* tempRow, int position, bool previousCellPlaced) {
  if(position >= PSIZE) {
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

  if(PSIZE - position >= rowSum) {
    // place block at current position
    if(!previousCellPlaced && row[rowStep][0] > elementCount){
      int* tempRow1 = new int[PSIZE];
      for(int i=0; i<PSIZE; i++)
        tempRow1[i] = tempRow[i];

      int pos = position;
      for(int i=0; i<row[rowStep][elementCount+1]; i++)
        tempRow1[pos++] = 1;

      generateCombination(rowStep, elementCount+1, tempRow1, pos, true);
    }

    // put a space at current position
    int* tempRow2 = new int[PSIZE];
    for(int i=0; i<PSIZE; i++)
      tempRow2[i] = tempRow[i];
    tempRow2[position] = 0;
    generateCombination(rowStep, elementCount, tempRow2, position+1, false);
  }
}

void storeQueueVector() {
  int* tempRow = new int[PSIZE];
  for(int i=0; i<PSIZE; i++)
    tempRow[i]=0;

  for(int rowStep=0; rowStep<PSIZE; rowStep++) {
    vector<int*> q;
    cv.push_back(q);
    generateCombination(rowStep, 0, tempRow, 0, false);
  }

  delete[] tempRow;
}

void Nemonemo(int rowStep, int** map) {
  if(mapFounded)
    return;
  cout << endl << rowStep + 1 << "th step : ";
  for(int i=1; i<=row[rowStep][0]; i++)
    cout << row[rowStep][i] << " ";
  cout << endl;

  for(int q = 0; q < cv[rowStep].size(); q++) {
    // 조합의 수 만큼 block의 위치를 옮겨가며 다음 스텝으로 넘어감
    int ** nextMap = new int*[PSIZE];
    for(int i = 0; i < PSIZE; i++) {
      nextMap[i] = new int[PSIZE];
      for(int j = 0; j < PSIZE; j++)
      nextMap[i][j] = map[i][j];
    }
    for(int i = 0; i < PSIZE; i++) {
      nextMap[rowStep][i] = cv[rowStep][q][i];
    }
    if(checkCol(nextMap)) Nemonemo(rowStep + 1, nextMap);

    for(int i = 0; i < PSIZE; i++)
      delete[] nextMap[i];
    delete[] nextMap;
  }
}

int main(int argc, char* argv[]) {
  if(argc > 2) {
    cout << "wrong argument!" << endl;
    return -1;
  }
  fileRead(argc == 2 ? argv[1] : "");

  cout << "PSIZE : " << PSIZE << endl;

  // vector< vector<int> > temp;
  // vector<int> t1;
  // t1.push_back(0); t1.push_back(0);
  // t1.push_back(1); t1.push_back(1);
  // t1.push_back(0); t1.push_back(0);
  // t1.push_back(0); t1.push_back(1);
  // t1.push_back(1); t1.push_back(0);
  //
  // vector<int> t2;
  // t2.push_back(0); t2.push_back(1);
  // t2.push_back(0); t2.push_back(0);
  // t2.push_back(1); t2.push_back(0);
  // t2.push_back(1); t2.push_back(0);
  // t2.push_back(0); t2.push_back(1);
  // vector<int> t3;
  // for(int i=0; i<PSIZE; i++) t3.push_back(1);
  // temp.push_back(t1);
  // temp.push_back(t2);
  // temp.push_back(t3);
  // cout << (checkCol(temp) ? " correct! " : " incorrect ") << endl;

  // storeQueueVector();
  //
  // Nemonemo(0, map);


  return 0;
}
