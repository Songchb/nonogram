// #include "nemo.h"
#include <iostream>
#include <fstream>
#include <sstream>  // parse input file

using namespace std;

class FileReader {
private:
  ifstream input;
  string tempLine;
  bool is_std;
  int rowSize, colSize; // length of row & column
  vector<vector<int>> rowParam;
  vector<vector<int>> colParam;
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
        cout << "file open error : " << inputFileName << "\n";
        exit(1);
      }
      cin.rdbuf(input.rdbuf());
    }
    if(is_std)
      cout << "row size, column size (without comma, just space) : ";
    cin >> rowSize >> colSize; // length of row & column
    cin.ignore(1000, '\n');
    for(int i = 0; i < rowSize + colSize; ++i) {
      if(is_std && i == 0) cout << "****** row ******\n";
      if(is_std && i == colSize) cout << "****** col ******\n";
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
      if(i < colSize)
        rowParam.push_back(tempVector);
      else
        colParam.push_back(tempVector);
    }
    if(colParam.size() != rowSize) {
      cout << "input file is invalid!\n";
      exit(1);
    }
  }

  void printRowCol() {
    cout << rowSize << " " << colSize << "\n";
    cout << "row\n";
    for(int i=0; i<colSize; ++i) {
      cout << rowParam[i][0] << " : ";
      for(int j = 1; j<row[i].size(); ++j) {
        cout << rowParam[i][j] << " ";
      }cout << "\n";
    }

    cout << "col\n";
    for(int i=0; i<rowSize; ++i) {
      cout << colParam[i][0] << " : ";
      for(int j = 1; j<col[i].size(); ++j) {
        cout << colParam[i][j] << " ";
      }cout << "\n";
    }
  }

  vector<vector<int>> getRow() {
    return rowParam;
  }

  vector<vector<int>> getCol() {
    return colParam;
  }
};
