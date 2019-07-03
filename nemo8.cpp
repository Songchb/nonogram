#include <iostream>
#include <fstream>
#include <sstream>  // parse input file
#include <vector>
#include <utility> // pair
#include <string>
#include <windows.h>
#include <time.h> // calculate execution time
#include <algorithm>  // min, sort
#include <iomanip>
// #define BRUTEFORCE false

using namespace std;

int ROW_SIZE, COL_SIZE; // 가로길이 세로길이
bool GRID = false;
bool PARAM = true;
vector<vector<int> > row;
vector<vector<int> > col;

bool sortByBlockCount(const pair<int, int> &a, const pair<int, int> &b) { return a.second > b.second; }

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
    cin >> ROW_SIZE >> COL_SIZE; // 가로길이 세로길이
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
    cout << "row" << endl;
    for(int i=0; i<COL_SIZE; ++i) {
      cout << row[i][0] << " : ";
      for(int j = 1; j<row[i].size(); ++j) {
        cout << row[i][j] << " ";
      }cout << "\n";
    }

    cout << "col" << endl;
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
  vector<vector<int> > prevMap;
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

    cout << "initialFill done" << endl;
    // 두번째 단계
    // 임시로 만든 map, while문을 돌다가 map에 변화가 없으면 (=수렴되면) 그만한다
    // 이 이후에는 세번째 단계 (=임시로 칠해보기, 모순 찾기, 조합 찾기) 를 시행한다

    do {
      // printMap();
      // getchar();
      prevMap = map;
      for(int i=0; i<COL_SIZE; ++i) {
        map[i] = rules1(row[i], map[i]); // rules의 parameter로 각 row를 바로 넣는다
        map[i] = rules2(row[i], map[i]);
        map[i] = rules3(row[i], map[i]);
      }

      for(int i=0; i<ROW_SIZE; ++i) {
        vector<int> temp;               // column에 대해서는 parameter를 바로 넣을 수 없기에
        for(int j=0; j<COL_SIZE; ++j)   // temp vector를 만들어서 똑같이 복사한 후 parameter로 넣어준다
          temp.push_back(map[j][i]);
        temp = rules1(col[i], temp);
        temp = rules2(col[i], temp);
        temp = rules3(col[i], temp);

        for(int j=0; j<COL_SIZE; ++j)
          map[j][i] = temp[j];
      }
    } while(prevMap != map);  // rules를 반복했을 때, 변화가 없으면 loop를 중지한다

    return;
  }

// 블록이 중복되어 확정되는 자리, parameter가 0인 곳, 자리가 딱 맞는 곳
  void initialFill(vector<vector<int> > rowcolInfo, int step, bool is_row) {
    if(rowcolInfo[step][1] == 0) {
      if(is_row) {
        for(int i=0; i<ROW_SIZE; ++i)
          map[step][i] = -1;
      } else {
        for(int i=0; i<COL_SIZE; ++i)
          map[i][step] = -1;
      }
      return;
    }

    int sum = rowcolInfo[step][0] - 1;
    for(int i = 1; i <= rowcolInfo[step][0]; ++i)
      sum += rowcolInfo[step][i];

    int position = 0;
    int gap = (is_row ? ROW_SIZE : COL_SIZE) - sum;

    for(int i = 1; i <= rowcolInfo[step][0]; ++i) { // param. 수만큼 돌면서
      for(int j = 0; j < min(rowcolInfo[step][i], gap); ++j)  // gap이 크면 블록 수 만큼, 아니면 gap만큼 불확정칸으로
        position++; // == map[step][position++] = 0;

      for(int j = gap; j < rowcolInfo[step][i]; ++j) { // 해당 블록만큼 칠하기
        if(is_row)
          map[step][position++] = 1;
        else
          map[position++][step] = 1;
      }
      if(gap == 0 && ((is_row && position < ROW_SIZE) || (!is_row && position < COL_SIZE))) {
        // 칸이 딱 들어맞다면 검은 칸도, 빈칸도 확정이므로 빈칸을 -1로 표시해둔다
        // if the blocks fit correctly in full puzzle, blank space is also fixed, so mark the blank space as -1.
        if(is_row)
          map[step][position++] = -1;
        else
          map[position++][step] = -1;
      }
      else
        position++;
    }
  }

// 전체 param.가 이미 다 칠해졌거나 (빈 칸 확정)
// 칠한 칸과 불확정칸의 합이 전체 param과 같은 경우 (칠할 칸 확정)
  vector<int> rules1(vector<int> rowcolInfo, vector<int> line) {
    vector<int> result = line;

    // 이미 모든 블록을 칠했으면 (paramSum과 비교해서) 빈 칸 확정
    int paramSum = 0;
    int filledBlockCount = 0;
    int uncertainCount = 0;

    // sum of parameters
    for(int i=1; i<=rowcolInfo[0]; ++i)
      paramSum+= rowcolInfo[i];

    // count filled blocks and uncertain blocks respectively
    for(int i=0; i<line.size(); ++i) {
      if(line[i] == 1)
        filledBlockCount++;
      else if(line[i] == 0)
        uncertainCount++;
    }

    if(paramSum == filledBlockCount) {
      for(int i=0; i<line.size(); ++i)
        if(line[i] == 0)
          result[i] = -1;
      return result;
    }

    // 불확정 칸의 수와 확정 칸의 수의 합이 paramSum과 같다면 불확정 칸 칠하기
    if(uncertainCount + filledBlockCount == paramSum) {
      for(int i=0; i<line.size(); ++i)
        if(line[i] == 0)
          result[i] = 1;
      return result;
    }

    return result;
  }

// 가장자리라서 확실해지는 자리
  vector<int> rules2(vector<int> rowcolInfo, vector<int> line) {
    int start = -1;
    int blockCount = 1;   // 첫번째 블록
    vector<int> result = line;

    for(int i=0; i<line.size(); ++i) {
      if(start == -1 && line[i] == 1) { //
        start = i; // 몇번째 블록인지 알아야 그 수만큼 칠할 수 있다
      }
      else if(start != -1 && line[i] == -1) {
        start = -1;
        blockCount++; // 만약 블록이 이미 다 칠해졌다면 타겟을 다음 블록을 변경한다
      }
      else if(line[i] == 0) {
        if(i != 0 && line[i-1] == 1) { // loop 끝! 색칠 시작!
          // 만약 색칠 시작이면 색 다 칠하고 지금 자리에 -1하기
          for(int j = 0; j < rowcolInfo[blockCount]; ++j)
            result[start + j] = 1;
          if(start + rowcolInfo[blockCount] < line.size())
            result[start + rowcolInfo[blockCount]] = -1;
        }
        break;
      }
    }

    int end = line.size() - 1;
    start = -1;
    blockCount = rowcolInfo[0];

    for(int i = line.size() - 1; i >= 0; --i) {
      if(start == -1 && line[i] == 1)
        start = i;
      else if(start != -1 && line[i] == -1) {
        start = -1;
        blockCount--;
      }
        else if(line[i] == 0) {
        if(i != line.size() - 1 && line[i + 1] == 1) {
          for(int j = 0; j < rowcolInfo[blockCount]; ++j)
            result[start - j] = 1;
          if(start - rowcolInfo[blockCount] >= 0)
            result[start - rowcolInfo[blockCount]] = -1;
        }
        break;
      }
    }

    return result;
  }

// 위치를 통해, 순서를 통해 확정할 수 있는 블록은 그 앞뒤로 빈칸 확정?
  vector<int> rules3(vector<int> rowcolInfo, vector<int> line) {
    vector<int> result = line;
    vector<int> paramBlocks = rowcolInfo;
    vector<pair<int, int>> countedBlocks;
    bool end = true;
    for(int i=0; i<line.size(); ++i) {
      if(line[i] == 1) {
        if(end) {
          countedBlocks.push_back(pair<int, int>(i, 0));  // block의 첫 위치와 block수를 pair로 만들어서 push
          end = false;
        }
        countedBlocks[countedBlocks.size() - 1].second++; // pair < position, count >
      } else if(line[i] != 1) { // 나오던 블록이 끝나면 그 다음에 나올 새로운 블록을 푸시할 준비
        end = true;
      }
    }

    int paramCount = paramBlocks[0];
    if(countedBlocks.size() == paramCount) {  // 만약 수가 같다면? 모든 블록이 매칭이 되므로 sort하기 전에 위치 확정
      paramBlocks.erase(paramBlocks.begin());           // paramBlocks의 맨 앞의 meta data(param 개수)를 삭제
      for(int i = 0; i < paramCount; ++i) {
        if(paramBlocks[i] == countedBlocks[i].second) { // 블록 수가 같아야 확정할 수 있다
          if(countedBlocks[i].first - 1 >= 0)
            result[countedBlocks[i].first - 1] = -1;
          if(countedBlocks[i].first + paramBlocks[i] < line.size())
            result[countedBlocks[i].first + paramBlocks[i]] = -1;
        }
      }
    }
    /*
    sort(countedBlocks.begin(), countedBlocks.end(), sortByBlockCount); // 블록 수로 내림차순
    sort(paramBlocks.begin(), paramBlocks.end(), greater<int>() ); // 파라미터 내림차순

    int bigBlock = 0;
    for(int i=0; i<paramBlocks.size(); ++i) {
      // 내림차순으로 정렬된 블록 수를 돌면서
      // 실제로 블록의 수를 count해서 내림차순 정렬한 countedBlocks를 대조해보면서
      // 같은 블록이 있으면 확정짓고 앞뒤로 빈칸처리
      if(paramBlocks[i] == countedBlocks[bigBlock].second) {
        // 일치! 앞뒤 빈칸 처리
        if(countedBlocks[bigBlock].first - 1 >= 0)
          result[countedBlocks[bigBlock].first - 1] = -1;
        if(countedBlocks[bigBlock].first + paramBlocks[i] < line.size())
          result[countedBlocks[bigBlock].first + paramBlocks[i]] = -1;
        // 다음으로 넘어감
        bigBlock++;
      } else break;

    }*/

    return result;
  }

  // 협소한 칸은 빈칸으로 간주
  vector<int> rules4(vector<int> rowcolInfo, vector<int> line) {

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
    // initialize
    int maxLengthRow = 0;
    int maxCountCol = 0;
    for(int i=0; i<COL_SIZE; ++i) {
      int lengthParam = 0;
      for(int j=1; j<=row[i][0]; ++j)
        lengthParam += (row[i][j] > 9 ? 3 : 2);

      if(maxLengthRow < lengthParam)
        maxLengthRow = lengthParam;
    }
    for(int i=0; i<ROW_SIZE; ++i) {
      if(maxCountCol < col[i][0])
        maxCountCol = col[i][0];
    }

    // print upper part
    if(PARAM) {
      for(int i=0; i<maxCountCol; ++i) {
        // print blanks as row parameters
        for(int r=0; r<maxLengthRow + 1; ++r) cout << " ";
        // print column parameters
        for(int j=0; j<ROW_SIZE; ++j) {
          if(GRID && j != 0 && j % 5 == 0)
            cout << " ";
          if(maxCountCol - i <= col[j][0])
            cout << setw(2) << col[j][(i + 1) - (maxCountCol - col[j][0])];
          else
            cout << "  ";
        }
        cout << endl;
      }
    }
    // print lower part
    for(int i = 0; i < COL_SIZE; ++i) {
      if(PARAM) {
        int lengthParam = 0;
        for(int j = 1; j <= row[i][0]; ++j)
          lengthParam += (row[i][j] > 9 ? 3 : 2);

        // print row parameters
        for(int r = 0; r <= maxLengthRow - lengthParam; ++r)
          cout << " ";
        for(int j = 1; j <= row[i][0]; ++j)
          cout << row[i][j] << " ";
      }
      // print map
      for(int j = 0; j < ROW_SIZE; ++j) {
        if(GRID && j != 0 && j % 5 == 0) {
          if((i+1)%5 != 0)  wprintf(L"┃");
          else              wprintf(L"┫");
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

  if(argc == 1 || argv[1][0] == '-')         // 가장 첫 아규먼트가 파일 이름이 아니라 옵션이면
    InputReader.fileRead("std");  // 퍼즐을 직접 입력 받는다
  else
    InputReader.fileRead(argv[1]);  // 파일로 입력 받는다

  for(int i=1; i<argc; ++i) {
    if(strcmp(argv[i], "-np") == 0 || strcmp(argv[i], "--no-parameters") == 0)
      PARAM = false;
    else if(strcmp(argv[i], "-g") == 0 || strcmp(argv[i], "--grid") == 0)
      GRID = true;
    else
      cout << "wrong option foramt : " << argv[i] << endl;
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

  cout << "execution time : " << (double)(end-start) / 1000 << "s" << endl;

  return 0;
}
