#ifndef NEMO_H
#define NEMO_H

#include <vector>
#include <utility> // pair
#include <fstream>
#include <string>
#include <windows.h>

using namespace std;

int ROW_SIZE, COL_SIZE; // length of row & column
bool GRID = false;
bool PARAM = true;
bool BRUTEFORCE = false;
vector<vector<int>> row; // parameters as global variable
vector<vector<int>> col;

wchar_t direc[] = L"→↓←↑";

void ClearScreen();

class Nemo {
public:
  vector<vector<int>> map;
  vector<vector<int>> prevMap;
  bool bfFound;
  vector<vector<vector<int>>> mapCombVector;
  vector<vector<int>> rowCombVector;
  vector<int> mapState;
  Nemo();
  void run();
  void initialFill(vector<vector<int>> rowcolInfo, int step, bool is_row);
  vector<int> rules1(vector<int> rowcolInfo, vector<int> line);
  vector<int> rules2(vector<int> rowcolInfo, vector<int> line, bool backward);
  vector<int> rules2_backNforth(vector<int> rowcolInfo, vector<int> line);
  vector<int> rules3(vector<int> rowcolInfo, vector<int> line);
  vector<int> rules4(vector<int> rowcolInfo, vector<int> line);
  void bruteforce(vector< vector<int>> candiMap, int step, bool initialize);
  void generateCombination(int step, int elementCount, vector<int> curVector, int position, bool previousCellPlaced);
  bool colCheckForBF(vector<vector<int>> candiMap, int rowStep);
  void printMap(vector<vector<int>> targetMap, int rowStep);
  void printLine(vector<int> line);
};

#endif
