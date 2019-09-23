#ifndef NEMO_H
#define NEMO_H

#include <vector>
#include <utility> // pair
#include <fstream>
#include <string>
#include <windows.h>

using namespace std;

extern int ROW_SIZE, COL_SIZE; // length of row & column
extern bool GRID;
extern bool PARAM;
extern bool BRUTEFORCE;
extern bool CLEAR;
extern vector<vector<int>> row; // parameters as global variable
extern vector<vector<int>> col;
extern wchar_t direc[];

void ClearScreen();

class Nemo {
public:
  vector<vector<int>> board;
  vector<vector<int>> prevBoard;
  bool bfFound;
  vector<vector<vector<int>>> boardCombVector;
  vector<vector<int>> rowCombVector;
  vector<int> boardState;
  Nemo();
  void run();
  void initialFill(vector<vector<int>> rowcolInfo, int step, bool is_row);
  vector<int> rules1(vector<int> rowcolInfo, vector<int> line);
  vector<int> rules2(vector<int> rowcolInfo, vector<int> line, bool backward);
  vector<int> rules2_backNforth(vector<int> rowcolInfo, vector<int> line);
  vector<int> rules3(vector<int> rowcolInfo, vector<int> line);
  vector<int> rules4(vector<int> rowcolInfo, vector<int> line);
  void bruteforce(vector< vector<int>> candiBoard, int step);
  void initCombVector();
  void generateCombination(int step, int elementCount, vector<int> curVector, int position, bool previousCellPlaced);
  bool colCheckForBF(vector<vector<int>> candiBoard, int rowStep);
  void printBoard(vector<vector<int>> targetBoard, int rowStep);
  void printLine(vector<int> line);
};

#endif
