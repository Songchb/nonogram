#include "nemo.h"
#include <iostream>
#include <vector>
#include <map>
#include <iomanip>
#include <algorithm>  // min, sort, reverse

bool sortByBlockCount(const pair<int, int> &a, const pair<int, int> &b) {
  return a.second - a.first > b.second - b.first;
}

/*

vector<vector<int>> board;
vector<vector<int>> prevBoard;
bool bfFound;
vector<vector<vector<int>>> boardCombVector;
vector<vector<int>> rowCombVector;
vector<int> boardState;

*/


Nemo::Nemo() {
  cout << "Nemo object initialize!" << endl;
  bfFound = false;
  /* initialize board */
  board = vector<vector<int>>(COL_SIZE, vector<int>(ROW_SIZE));
}

void Nemo::run() {
  if(BRUTEFORCE) {
    initCombVector();
    cout << "\n";
    bruteforce(board, 0);
    return;

  }
  for(int step = 0; step < ROW_SIZE + COL_SIZE; step++) {
    if(step < COL_SIZE)
      initialFill(row, step, true);
    else
      initialFill(col, step - COL_SIZE, false);
  }

  cout << "initialFill done\n";

  // 두번째 단계
  // 임시로 만든 board, while문을 돌다가 board에 변화가 없으면 (=수렴되면) 그만한다
  // 이 이후에는 세번째 단계 (=임시로 칠해보기, 모순 찾기, 조합 찾기) 를 시행한다
  do {
    // printBoard(board, COL_SIZE);
    // getchar();
    prevBoard = board;
    // → 오른쪽 방향
    for(int i=0; i<COL_SIZE; ++i) {
      board[i] = rules1(row[i], board[i]); // rules의 parameter로 각 row를 바로 넣는다
      board[i] = rules2(row[i], board[i], false);
      board[i] = rules4(row[i], board[i]);
      // rules3은 잠정적 보류
      // board[i] = rules3(row[i], board[i]);
    }
    boardState.push_back(0);
    // printBoard(board, COL_SIZE);
    // getchar();
    // ← 왼쪽 방향
    for(int i=0; i<COL_SIZE; ++i) {
      board[i] = rules2(row[i], board[i], true);
    }
    boardState.push_back(2);
    // printBoard(board, COL_SIZE);
    // getchar();
    // ↓ 아래쪽 방향
    for(int i=0; i<ROW_SIZE; ++i) {
      vector<int> temp;               // column에 대해서는 parameter를 바로 넣을 수 없기에
      for(int j=0; j<COL_SIZE; ++j)   // temp vector를 만들어서 똑같이 복사한 후 parameter로 넣어준다
        temp.push_back(board[j][i]);

      temp = rules1(col[i], temp);
      temp = rules2(col[i], temp, false);
      temp = rules4(col[i], temp);
      // rules3은 잠정적 보류
      // temp = rules3(col[i], temp);

      for(int j=0; j<COL_SIZE; ++j)
        board[j][i] = temp[j];
    }
    boardState.push_back(1);
    // printBoard(board, COL_SIZE);
    // getchar();
    // ↑ 위쪽 방향
    for(int i=0; i<ROW_SIZE; ++i) {
      vector<int> temp;
      for(int j=0; j<COL_SIZE; ++j)
        temp.push_back(board[j][i]);
      temp = rules2(col[i], temp, true);
      for(int j=0; j<COL_SIZE; ++j)
        board[j][i] = temp[j];
    }
    boardState.push_back(3);

  } while(prevBoard != board);  // rules를 반복했을 때, 변화가 없으면 loop를 중지한다
  cout << "board converged! " << endl;

  printBoard(board, COL_SIZE);

  getchar();
  // board가 수렴했으면 board를 기반으로 brute-force를 시작한다
  initCombVector();
  getchar();
  cout << "\n";
  bruteforce(board, 0);
  return;
}

/* 블록이 중복되어 확정되는 자리, parameter가 0인 곳, 자리가 딱 맞는 곳 */
void Nemo::initialFill(vector<vector<int> > rowcolInfo, int step, bool is_row) {
  if(rowcolInfo[step][1] == 0) {
    if(is_row) {
      for(int i=0; i<ROW_SIZE; ++i)
        board[step][i] = -1;
    } else {
      for(int i=0; i<COL_SIZE; ++i)
        board[i][step] = -1;
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
      position++; // == board[step][position++] = 0;

    for(int j = gap; j < rowcolInfo[step][i]; ++j) // 해당 블록만큼 칠하기
      if(is_row)  board[step][position++] = 1;
      else        board[position++][step] = 1;

    /* 칸이 딱 들어맞다면 검은 칸도, 빈칸도 확정이므로 빈칸을 -1로 표시해둔다 */
    /* if the blocks fit correctly in full puzzle, blank space is also fixed, so mark the blank space as -1 */
    if(gap == 0 && ((is_row && position < ROW_SIZE) || (!is_row && position < COL_SIZE))) {
      if(is_row)  board[step][position++] = -1;
      else        board[position++][step] = -1;
    }
    else position++;
  }
}

/* 전체 param.가 이미 다 칠해졌거나 (빈칸확정) */
/* 칠한 칸과 불확정칸의 합이 전체 param과 같은 경우 (칠할 칸 확정) */
vector<int> Nemo::rules1(vector<int> rowcolInfo, vector<int> line) {
  vector<int> result = line;

  // 이미 모든 블록을 칠했으면 (paramSum과 비교해서) 빈칸확정
  int paramSum = 0;
  int filledBlockCount = 0;
  int uncertainCount = 0;

  // sum of parameters
  for(int i=1; i<=rowcolInfo[0]; ++i)
    paramSum+= rowcolInfo[i];

  // count filled blocks and uncertain blocks respectively
  for(int i=0; i<result.size(); ++i) {
    if(result[i] == 1)
      filledBlockCount++;
    else if(result[i] == 0)
      uncertainCount++;
  }

  if(paramSum < filledBlockCount) {
    cout << "puzzle is invalid! wrong parameters!\n";

    for(int i=1; i <= rowcolInfo[0]; i++)
      cout << rowcolInfo[i] << " ";
    printLine(result);
    cout << "\n";

    printBoard(board, COL_SIZE);

    exit(1);
  }

  // param만큼 확정블록이 찾아졌으면 나머지 칸은 확정빈칸
  if(paramSum == filledBlockCount) {
    for(int i=0; i<result.size(); ++i)
      if(result[i] == 0)
        result[i] = -1;
    return result;
  }

  // 불확정 칸의 수와 확정블록의 수의 합이 paramSum과 같다면 불확정 칸은 모두 확정블록
  if(uncertainCount + filledBlockCount == paramSum) {
    for(int i=0; i<result.size(); ++i)
      if(result[i] == 0)
        result[i] = 1;
    return result;
  }

  return result;
}

/* 가장자리라서 확실해지는 자리 */
vector<int> Nemo::rules2(vector<int> rowcolInfo, vector<int> line, bool backward) {
    vector<int> result = line;
    if(!backward) {
      result = rules2_backNforth(rowcolInfo, result);
      return result;
    } else {
      // param도 거꾸로 넣어준다
      vector<int> tempRowCol;
      tempRowCol.push_back(rowcolInfo[0]);
      for(int i=rowcolInfo[0]; i>=1; --i)
        tempRowCol.push_back(rowcolInfo[i]);

      // reverse한 line을 넣어주고
      reverse(result.begin(), result.end());
      result = rules2_backNforth(tempRowCol, result);
      // 결과값도 reverse 해준다
      reverse(result.begin(), result.end());

    reverse(result.begin(), result.end());
    result = rules2_backNforth(tempRowCol, result);
    reverse(result.begin(), result.end());

vector<int> Nemo::rules2_backNforth(vector<int> rowcolInfo, vector<int> line) {
    int start = -1; // block이 시작하는 위치
    int blank = -1; // blank가 시작하는 위치
    int blockCount = 1;   // 첫번째 블록
    vector<int> result = line;

    for(int i = 0; i < result.size(); ++i) {
      //    ↓
      // 5 ⬛⬛⬛⬛⬜⬜⬜⬜⬜⬜
      if(result[i] == 1 && start == -1) {
        start = i; // 블록이 어디서부터 시작하는지 알아야 블록 수만큼 정확히 칠할 수 있다
      }

vector<int> Nemo::rules2_backNforth(vector<int> rowcolInfo, vector<int> line) {
  int start = -1; // block이 시작하는 위치
  int blank = -1; // blank가 시작하는 위치
  int blockCount = 1;   // 첫번째 블록
  vector<int> result = line;

  for(int i = 0; i < result.size(); ++i) {
    // cout << "i : " << i << ", ";
    // if(result[i] == 1)        wprintf(L"■");
    // else if(result[i] == -1)  wprintf(L"▨");
    // else                      wprintf(L"□");
    // cout << ", start : " << start << ", blank : " << blank << ", size : " << result.size() << endl;

    //    ↓
    // 5 ⬛⬛⬛⬛⬜⬜⬜⬜⬜⬜
    if(result[i] == 1 && start == -1) {
      start = i; // 블록이 어디서부터 시작하는지 알아야 블록 수만큼 정확히 칠할 수 있다
    }

      //           ↓
      // 5 ⬛⬛⬛⬛⬜⬜⬜⬜⬜⬜ → ⬛⬛⬛⬛⬛📕⬜⬜⬜
      else if(result[i] == 0 && blank == -1 && start != -1) { // 이전에 block들이 나왔었고
        for(int j = 0; j < rowcolInfo[blockCount] - (i - start); ++j)
          result[i + j] = 1;
        if(start + rowcolInfo[blockCount] < result.size()) { // 마감처리
          result[start + rowcolInfo[blockCount]] = -1;
          i = start + rowcolInfo[blockCount];
        }
        start = -1;
        blank = -1;
        blockCount++;

    //           ↓
    // 5 ⬛⬛⬛⬛⬜⬜⬜⬜⬜⬜ → ⬛⬛⬛⬛⬛✉⬜⬜⬜
    else if(result[i] == 0 && blank == -1 && start != -1) { // 이전에 block들이 나왔었고
      for(int j = 0; j < rowcolInfo[blockCount] - (i - start); ++j)
        result[i + j] = 1;
      if(start + rowcolInfo[blockCount] < result.size()) { // 마감처리
        result[start + rowcolInfo[blockCount]] = -1;
        i = start + rowcolInfo[blockCount];
      }
      start = -1;
      blank = -1;
      blockCount++;

    }
/*
      //  (1)      ↓
      // 5 ⬜⬜⬛⬛⬜⬜⬜⬜⬜⬜ → ⬜⬜⬛⬛⬛⬜⬜⬜⬜⬜
      //  (1)        ↓
      // 5 4 ⬜⬜⬛⬛⬜⬜⬜⬜⬜⬜ → ⬜⬜⬛⬛⬛⬜⬜⬜⬜⬜
      //  (2)        ↓
      // 2 ⬜⬜⬜⬜⬛⬜⬜⬜⬜⬜ → 📕📕📕⬜⬛⬜⬜⬜⬜⬜
      //  (2)        ↓
      // 5 1 ⬜⬜⬜⬜⬛⬛⬛⬜⬜⬜ → 📕📕⬜⬜⬛⬛⬛⬜⬜⬜
      //  (3)      ↓
      // 5 ⬜⬜⬛⬛⬜⬜📕⬜⬜⬜ → ⬜⬛⬛⬛⬛⬜📕⬜⬜⬜
      //  (4)      ↓
      // 2 4 ⬜⬜⬛⬜⬛⬜⬜⬜⬜⬜ → 📕⬛⬛📕⬛⬜⬜⬜⬜⬜ → 📕⬛⬛📕⬛⬛⬛⬛📕⬜
      //  (5)      ↓
      // 3 4 ⬜⬜⬛⬜⬜⬛⬜⬜⬜⬜ → ⬜⬛⬛⬜⬜⬛⬜⬜⬜⬜
      //  (5)        ↓
      // 4 2 ⬜⬜⬜⬛⬜⬜⬛⬜⬜⬜ → x
      //  (5)        ↓
      // 3 3 ⬜⬜⬛⬛⬜⬜⬛⬛⬜⬜ → ⬜⬜⬛⬛⬜⬜⬛⬛⬜⬜
      //  (6)    ↓
      // 6 1 ⬜⬛⬜⬛⬛⬜⬛⬜⬛⬜ → 📕⬛⬛⬛⬛⬛⬛📕⬛⬜
      //  (7)    ↓
      // 6 1 ⬜⬛⬜⬛⬛⬛⬜⬜⬛⬜ → ⬜⬛⬛⬛⬛⬛⬜⬜⬛⬜
      //  (8)    ↓
      // 6 2 ⬜⬛⬜⬛⬛⬛⬜⬛⬛⬜ → ⬛⬛⬛⬛⬛⬛📕⬛⬛⬜
      //  (9)            ↓
      // 2 2 ⬜⬜⬜⬜⬜⬛⬜⬜⬜⬜ → x
      //  (10)
      // 2 6 ⬜⬜⬜⬛⬛⬛⬜⬜⬜⬜ → ⬛⬛📕⬛⬛⬛⬜⬜⬜⬜ → ⬛⬛📕⬛⬛⬛⬛⬛⬛📕
      //  (11)
      // 3 7 1 ⬜⬜⬜⬜⬜⬛⬛⬛⬛⬛⬛⬜⬜⬜⬜ → ⬜⬛⬛⬜⬜⬛⬛⬛⬛⬛⬛⬜⬜⬜⬜
      //  (12)
      // 4 4 ⬜⬜⬜⬜⬜⬛⬜📕⬜📕⬜⬜⬜⬜⬜ → ⬜⬜⬜⬛⬛⬛⬜📕⬜📕⬜⬜⬜⬜⬜
      //  (13)
      // 3 6 4 ⬜⬜⬜⬜📕⬜⬜⬜⬜⬜📕⬜⬜⬜⬛⬜⬜📕⬜⬜⬜⬜⬜⬜⬜
*/
    else if(result[i] == 0 && blank != -1 && start != -1) {
      // 불확정 칸이 param 보다 많으면 start의 블록이 param이 아닐 수도 있다 (9)
      if(rowcolInfo[0] != blockCount && start - 1 - blank >= rowcolInfo[blockCount]) {
        // cout << ", start : " << start << ", blank : " << blank << ", blockCount : " << blockCount << ", rowcolInfo[blockCount] : " << rowcolInfo[blockCount] << endl;

        // 이미 센 start 블록이 param보다 많다면 start 블록이 param이 아닌 것이 확실하다 (10)
        // 불확정 칸 안에 param을 넣을 수 있는 경우의 수 중에 겹쳐지는 곳은 확정블록이 된다
        if(i - start > rowcolInfo[blockCount]) {
          for(int j = start - 1 - rowcolInfo[blockCount]; j < blank + rowcolInfo[blockCount]; ++j) {
            result[j] = 1;

            // for(int i=1; i<=rowcolInfo[0]; ++i)
            //   cout << rowcolInfo[i]<<" ";
            // cout << ": " << rowcolInfo[blockCount] << " | ";
            // cout << "filled at " << j << ", blank : " << blank << ", start : " << start << ", i : " << i << endl;
          }

          // 이미 센 start 블록이 param보다 많다면 start 블록이 param이 아닌 것이 확실하다 (10)
          // 불확정 칸 안에 param을 넣을 수 있는 경우의 수 중에 겹쳐지는 곳은 확정블록이 된다
          if(i - start > rowcolInfo[blockCount]) {
            for(int j = start - 1 - rowcolInfo[blockCount]; j < blank + rowcolInfo[blockCount]; ++j) {
              result[j] = 1;
            }

            // 다 채워진 경우
            if(blank + rowcolInfo[blockCount] + 1 == start) {
              result[start - 1] = -1;
              if(blank - 1 >= 0)
                result[blank - 1] = -1;
              blank = -1;
            }
            blank = -1;
          }
          blockCount++;
        }
        break;
      }

      // 블록 수와 param이 같으면 확정빈칸 처리
      if(i - start == rowcolInfo[blockCount]) {
        for(int j = blank; j < start; ++j)
          result[j] = -1;
        result[i] = -1;
        blank = -1;
        start = -1;
        blockCount++;
        continue;
      }

      int rightFill = i;
      int leftFill = start - 1;
      // i 바로 전부터 하나씩 왼쪽으로 rowcolInfo[blockCount]만큼
      for(int j = 1; j <= rowcolInfo[blockCount]; ++j) {
        if(i - j < blank) { // blank보다 더 전으로 넘어와버리면 반대편(블록의 오른쪽)에 색칠
          result[rightFill++] = 1;
        }
        else if(j == rowcolInfo[blockCount]) { // rowcolInfo[blockCount]만큼 다 왔다면 (2)의 경우로 왼쪽은 확정빈칸이다
          for(int r = blank; r < i - j; ++r)
            result[r] = -1;
        }
      }

        int mergeStart = start;
        // int mergeEnd = start;
        int nextBlockCnt = 0;
        // start부터 rowcolInfo[blockCount]만큼 오른쪽으로 간다
        for(int j = 0; j < rowcolInfo[blockCount] - (i - start); ++j) {
          // 5 ⬜⬜⬛⬛⬜⬜📕⬜⬜⬜ → ⬜⬛⬛⬛⬛⬜📕⬜⬜⬜
          // 5 📕⬜⬜⬜⬜⬜⬜⬛⬛⬜
          if(i + j >= result.size() || result[i + j] == -1)  { // 확정빈칸 나오면 블록의 반대편(블록의 왼쪽)에 색칠
            for(int r = 0; r < rowcolInfo[blockCount] - (i - start) - j; r++) {
              result[leftFill--] = 1;
            }
            break;
          }
          break;
        }

          /*
          // 4 2 📕⬜⬜⬜⬜⬛⬜⬜⬛⬜ // 뒤까지 확인? 뒤 블록을 병합해서 2가 자리가 충분한지?
          //   → 📕⬜⬜⬛⬛⬛⬜⬜⬛⬜ // 병합될 가능성이 없으므로 4블록에 대한 확정블록을 칠할 수 있다
          // 4 2 📕⬜⬜⬜⬜⬛⬜⬜⬛⬜⬜⬜⬜⬜⬜
          //   → x                               // 뒤 블록이 4와 병합될 가능성이 있다
          // 4 2 📕⬜⬜⬜⬜⬛⬜⬜⬛⬜📕⬜📕⬜📕      //  2가 뒤에 자리가 충분한지 아닌지를 어떻게 판별하는가?
          // 2 4 ⬜⬛⬜⬜⬛⬜⬜⬜⬜📕 // 4 2를 거꾸로 봤을 때 2부터 봐도 확정되는 자리가 없다

          // 확정블록 판별은 마지막 위치에서도 그 다음 위치가 확정블록인지 판별해야함

          // merge 할 수 있더라도 다음 블록이 연속적이어서 merge 할 수 없는 케이스 핸들링
          // merge 변수를 사용해야 한다
          */
          if(mergeStart != start && result[i + j] == 0) {
            mergeStart = -1;
            nextBlockCnt = -1;
          }
          else if(result[i + j] == 1) {
            if(nextBlockCnt != -1)
              nextBlockCnt++;
            if(mergeStart == -1 || mergeStart == start)
              mergeStart = i + j;
          }
/*
        cout << "start + j : " << start + j << ", result[start + j] : " << result[start + j] <<  ", mergeStart : " << mergeStart << ", mergeEnd : " << mergeEnd << endl;
        if(mergeStart != -1 && result[start + j] == 0) {// merge 초기화
          nextBlockCnt = 0; // nextBlock counting 비활성화
          mergeStart = -1;
        }
        else if(mergeStart != start && result[start + j] == 1) {// start 블록 뒤에 새로운 블록이 나왔으며 그 위치를 표시한다
          if(nextBlockCnt != 0 || mergeEnd == -1) { // i 다음으로 나오는 merge 블록을 센다
            nextBlockCnt++;
          }
          if(mergeStart == -1) {
            mergeStart = start + j;
          }
          mergeEnd = start + j;
        }
*/
          /*
          // 6 2 5 ⬜⬜⬜⬜⬜⬛⬜⬛⬛⬛⬛⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜
          // 6 2 5 ⬜⬜⬜⬜⬜⬛⬜⬜⬛⬛⬛⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜
          // 5 1 5 ⬜⬜⬜⬜⬜⬛⬛⬜⬛⬛⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜
          // 7 2 5 ⬜⬜⬜⬜⬜⬛⬜⬜⬛⬛⬛⬛⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜
          // 6 2 5 ⬜⬜⬜⬜⬜⬛⬜⬜⬜⬜⬜⬜⬜⬜⬜⬛⬛⬛⬛⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜
          */
          // i 바로 다음으로 나오는 merge 블록의 count를 세서
          // 다음 blockCount의 rowcolInfo보다 많다면 그 merge 블록은 merge될 수 밖에 없다!
          if(blockCount + 1 < result.size() && nextBlockCnt > rowcolInfo[blockCount + 1])
          {
            for(int r = i; r < mergeStart; r++)
              result[r] = 1;
          }

          // 끝의 다음 자리에 색칠되어 있으면 병합할 수 없으며 반대편(블록의 왼쪽)에 색칠한다
          // merge를 이용해 위치를 파악한다
          //                  →           ↓
          // 6 4 📕📕⬜⬜⬜⬛⬜⬛⬜⬛⬛⬛⬜⬜⬜
          //  → 📕📕⬛⬛⬛⬛⬜⬛⬜⬛⬛⬛⬜⬜⬜
          //         →              ↓
          // 6 2 ⬜⬛⬜⬛⬛⬛⬜⬛⬛⬜
          //  → ⬛⬛⬜⬛⬛⬛⬜⬛⬛⬜
          // 다음 위치에 블록이 있고 그 블록과 병합하지 못하는 거리에 닿아있다

        // 끝의 다음 자리에 색칠되어 있으면 병합할 수 없으며 반대편(블록의 왼쪽)에 색칠한다
        // merge를 이용해 위치를 파악한다
        //               →                        ↓ ↓ ↓
        // 6 4 ✉✉⬜⬜⬜⬛⬜⬛⬜⬛⬛⬛⬜⬜⬜ → ✉✉⬛⬛⬛⬛⬜⬛⬜⬛⬛⬛⬜⬜⬜
        //        →                  ↓
        // 6 2 ⬜⬛⬜⬛⬛⬛⬜⬛⬛⬜ → ⬛⬛⬜⬛⬛⬛⬜⬛⬛⬜
        // 다음 위치에 블록이 있고 그 블록과 병합하지 못하는 거리에 닿아있다

        if(j == rowcolInfo[blockCount] - (i - start) - 1 && i + j + 1 < result.size() && result[i + j + 1] == 1) {
          // cout << "blank : " << blank << ", start : "<< start << ", i : " << i << ", j : " << j << ", mergeStart : " << mergeStart << endl;
          result[leftFill--] = 1;
          for(int r = mergeStart; mergeStart != start && mergeStart != -1 && r <= i + j; ++r)
            result[leftFill--] = 1;
        }

          // 끝의 다음 자리에 확정빈칸이 있으므로 merge하고
          if(j == rowcolInfo[blockCount] - (i - start) - 1 && i + j + 1 < result.size() && result[i + j + 1] == -1) {

        }
      }

      // 모든 처리가 끝난 후 블록이 param과 일치하면 마감처리
      int magam = -1;
      for(int j = start; j < rowcolInfo[blockCount]; ++j) {
        if(result[j] != 1) {                                // i부터 가면서 색칠되지 않은 블록을 만났을 때
          if(j - leftFill - 1 == rowcolInfo[blockCount]) {  // leftFill부터 세서 param과 같다면 마감 처리
            result[j] = -1;
            magam = j;
            for(int r = leftFill - 1; r > blank; --r)
              if(r >= 0)
                result[r] = -1;
          }
          j = rowcolInfo[blockCount];
        }
      }
      if(magam != -1) {
        i = magam + 1;
        blockCount++;
        blank = -1;
        start = -1;
      }
      else
        break;

        /*
        // 6 1 ⬜⬛⬜⬛⬛⬛⬜⬜⬛⬜ → ⬜⬛⬛⬛⬛⬛⬜⬜⬛⬜
        // 6 2 ⬜⬛⬜⬛⬛⬛⬜⬛⬛⬜ → ⬛⬛⬛⬛⬛⬛📕⬛⬛⬜
        // 6 2 ⬜⬜⬜⬛⬜⬛⬜⬜⬜⬛⬛⬜📕📕📕 → ⬜⬜⬛⬛⬛⬛⬜⬜⬜⬛⬛⬜📕📕📕
        // 6 2 ⬜⬜⬜⬛⬜⬜⬛⬜⬜⬛⬛⬜📕📕📕 → ⬜⬜⬛⬛⬛⬛⬛⬜⬜⬛⬛⬜📕📕📕
        // 6 2 ⬜⬜⬜⬛⬜⬜⬛⬜⬛⬛⬜⬜📕📕📕 → ⬜⬜⬜⬛⬛⬛⬛⬜⬛⬛⬜⬜📕📕📕 // 합병을 할지 말지 판별 ★

        // 6 2 ⬜⬜⬜⬛⬜⬜⬜⬛⬜⬛⬛⬜📕📕📕 → ⬜⬜⬛⬛⬛⬛⬛⬛⬜⬛⬛⬜📕📕📕


        //  5 ⬜⬜⬛⬛⬜⬜📕⬜⬜⬜
        // → ⬜⬛⬛⬛⬛⬜📕⬜⬜⬜
        //  5 ⬜⬜⬛⬛⬜⬜⬛⬜⬜⬜
        // → ⬜⬜⬛⬛⬛⬛⬛⬜⬜⬜
        // 5 2 ⬜⬜⬛⬛⬜⬜⬛⬜⬜⬜
        //  → ⬜⬜⬛⬛⬛⬜⬛⬜⬜⬜
        // 5 2 ⬜⬜⬛⬛⬜⬜⬛⬛⬜⬜
        //  → ⬜⬛⬛⬛⬛⬜⬛⬛⬜⬜
        // 2 4 ⬜⬜⬛⬜⬛⬜⬜⬜⬜⬜
        //   → 📕⬛⬛📕⬛⬜⬜⬜⬜⬜
        //   → 📕⬛⬛📕⬛⬛⬛⬛📕⬜
        // 6 2 1 ⬜⬜⬛⬜⬜⬜⬛⬛⬜⬛⬜📕📕📕⬜
        //    → ⬜⬜⬛⬛⬛⬛⬛⬛⬜⬛⬜📕📕📕⬜
        */
      }

      //              ↓
      //  4 ⬜⬜⬛⬛⬛📕⬜⬜⬜⬜⬜ → 📕⬛⬛⬛⬛📕⬜⬜⬜⬜⬜
      //  4 ⬜⬜⬜⬜⬜⬛⬛⬛📕⬜⬜⬜⬜⬜ → ⬜⬜⬜📕⬛⬛⬛⬛📕⬜⬜⬜⬜⬜
      //  4 3 ⬜⬜⬜⬜⬜⬛⬛⬛📕⬜⬜⬜⬜⬜ → x
      //  2 2 1 ⬜⬜⬜⬜⬜⬛⬛📕⬜⬜⬜⬜⬜ → x
      //  3 1 1 ⬜⬜⬜⬜⬜⬛⬛📕⬜⬜⬜⬜⬜ → 📕📕📕📕⬛⬛⬛📕⬜⬜⬜⬜⬜
      //  3 3 1 ⬜⬜⬜⬜⬜⬛⬛📕⬜⬜⬜⬜⬜ → x
      //  3 1 3 ⬜⬜⬜⬜⬜⬛⬛📕⬜⬜⬜⬜⬜ → 📕📕📕📕⬛⬛⬛📕⬜⬜⬜⬜⬜
      //  3 1 3 ⬜⬜⬜⬜⬜⬛⬛📕⬜⬜⬜⬜⬜ → 📕📕📕📕⬛⬛⬛📕⬜⬜⬜⬜⬜

/*
                                 0, 1, 2, 3, 4
      rowcolInfo[0] = param의 수 {4, 13, 23, 12, 2} = row[5]
      blockCount - 몇번째 param인지,
      i = 5
      blank = 0
      rowcolInfo[blockCount] = 2
      rowcolInfo[blockCount+1] = 2

      j = 1 ~ j = rowcol[blockCount]까지
      result[i-j] = 1;

  */

      //                 ↓
      //  5 ⬛⬛⬛⬛⬛📕⬜⬜⬜⬜⬜ → x
      else if(result[i] == -1 && start != -1 && blank == -1) {
        if(i - start != rowcolInfo[blockCount]) {
          cout << "not matching from certain blank" << endl;
          cout << "i : " << i << ", start : " << start << ", blockCount : " << blockCount << ", rowcolInfo[blockCount] : " << rowcolInfo[blockCount] << endl;

          printLine(result);
          cout << "------------------------------------------------------------------------------------------------\n";
          printBoard(board, COL_SIZE);
          exit(1);
        }
        start = -1;
        blockCount++;
        continue;
      }

      //  2 2 3 ⬜⬜⬜⬜⬜⬜⬜⬛⬛📕⬜⬜⬜⬜⬜ → x
      else if(result[i] == -1 && start != -1 && blank != -1) {
        /* 확정빈칸이 나오면 지금까지 센 블록들 판별하고 안맞으면 칠하기 */
        /* i - blank 빈칸과 start block이 꽤 넓다면 blockCount의 param이 아닐 수도 있다 */
        if(rowcolInfo[0] == blockCount || i - blank < rowcolInfo[blockCount] + rowcolInfo[blockCount + 1] + 1) {
          // cout << "i : " << i << ", blank : " << blank << ", blockCount : " << blockCount << ", rowInfo[blockCount] : " << rowcolInfo[blockCount] << ", rowInfo[blockCount + 1] : "<< rowcolInfo[blockCount+1]<< endl;
          for(int j = 1; j <= rowcolInfo[blockCount]; ++j)
            result[i - j] = 1;
          if(i - rowcolInfo[blockCount] - 1 >= 0)
            result[i - rowcolInfo[blockCount] - 1] = -1;
          start = -1;
          blank = -1;
          blockCount++; // 블록이 다 칠해졌다면 타겟을 다음 블록을 변경한다
        }
        else break;
      }

      //             ↓
      // 4 ⬜⬜⬜⬜⬜📕⬜⬜⬜⬜ → x
      // 4 ⬜⬜⬜⬜⬜📕⬜📕⬜⬜ → x
      //       ↓
      // 4 ⬜⬜📕⬜⬜⬜⬜⬜⬜⬜ → 📕📕📕⬜⬜⬜⬜⬜⬜⬜
      // 4 ⬜⬜📕⬜📕⬜⬜⬜⬜⬜ → 📕📕📕⬜⬜⬜⬜⬜⬜⬜
      //             ↓
      // 2 ⬜⬜⬜⬜⬜📕⬜⬜⬜⬜ → x

      // 5 ⬜⬜📕⬜⬛⬜⬜⬜⬜⬜ → x

    //  2 2 3 ⬜⬜⬜⬜⬜⬜⬜⬛⬛✉⬜⬜⬜⬜⬜ → x
    else if(result[i] == -1 && start != -1 && blank != -1) {
      /* 확정빈칸이 나오면 지금까지 센 블록들 판별하고 안맞으면 칠하기 */
      /* i - blank 빈칸과 start block이 꽤 넓다면 blockCount의 param이 아닐 수도 있다 */
      if(rowcolInfo[0] == blockCount || i - blank < rowcolInfo[blockCount] + rowcolInfo[blockCount + 1] + 1) {
        // cout << "i : " << i << ", blank : " << blank << ", blockCount : " << blockCount << ", rowInfo[blockCount] : " << rowcolInfo[blockCount] << ", rowInfo[blockCount + 1] : "<< rowcolInfo[blockCount+1]<< endl;
        for(int j = 1; j <= rowcolInfo[blockCount]; ++j)
          result[i - j] = 1;
        if(i - rowcolInfo[blockCount] - 1 >= 0)
          result[i - rowcolInfo[blockCount] - 1] = -1;
        start = -1;
        blank = -1;
        blockCount++; // 블록이 다 칠해졌다면 타겟을 다음 블록을 변경한다
      }
      //     ↓
      //  5 📕📕⬛⬛⬜⬜⬜⬜⬜⬜

    //             ↓
    // 4 ⬜⬜⬜⬜⬜✉⬜⬜⬜⬜ → x
    // 4 ⬜⬜⬜⬜⬜✉⬜✉⬜⬜ → x
    //       ↓
    // 4 ⬜⬜✉⬜⬜⬜⬜⬜⬜⬜ → ✉✉✉⬜⬜⬜⬜⬜⬜⬜
    // 4 ⬜⬜✉⬜✉⬜⬜⬜⬜⬜ → ✉✉✉⬜⬜⬜⬜⬜⬜⬜
    //             ↓
    // 2 ⬜⬜⬜⬜⬜✉⬜⬜⬜⬜ → x

    // 5 ⬜⬜✉⬜⬛⬜⬜⬜⬜⬜ → x

    else if(result[i] == -1 && blank != -1 && start == -1) {        if(i - blank < rowcolInfo[blockCount]) {
        for(int j = blank; j < i; ++j)
          result[j] = -1;
        blank = -1;
      } else break;
    }
    //     ↓
    //  5 ✉✉⬛⬛⬜⬜⬜⬜⬜⬜

    else;


  }

  return result;
}

/* 위치를 통해, 순서를 통해 확정할 수 있는 블록은 그 앞뒤로 빈칸 확정? */
vector<int> Nemo::rules3(vector<int> rowcolInfo, vector<int> line) {
    vector<int> result = line;
    vector<int> paramBlocks = rowcolInfo;
    vector<pair<int, int>> countedBlocks;
    bool end = true;
    for(int i=0; i<result.size(); ++i) {
      if(result[i] == 1) {
        if(end) {
          countedBlocks.push_back(pair<int, int>(i, 0));  // block의 첫 위치와 block수를 pair로 만들어서 push
          end = false;
        }
        countedBlocks[countedBlocks.size() - 1].second++; // pair < position, count >
      } else if(result[i] != 1) { // 나오던 블록이 끝나면 그 다음에 나올 새로운 블록을 푸시할 준비
        end = true;
      }
      countedBlocks[countedBlocks.size() - 1].second++; // pair < position, count >
    } else if(result[i] != 1) { // 나오던 블록이 끝나면 그 다음에 나올 새로운 블록을 푸시할 준비
      end = true;
    }
  }

    /*
     * 버그 발견
     * 수가 같다고 블록이 매칭 되는 것은 아님
     * ex) 2 5 2 📕⬛⬛⬜⬜⬜⬜⬛⬜⬜⬛⬜⬜⬜⬜
     * ex) 6 1 📕📕⬜⬜⬛⬛⬛⬜⬛⬜⬜⬜⬜⬜⬜
     *
     * 또다른 버그 발견
     * 수가 같고 블록 수가 같지만 param이 아닐 수도 있다
     * ex) 1 7 2 ⬜⬜⬜⬜⬛⬜⬛⬛⬛⬛⬜⬜⬜⬛⬜
     *         → ⬜⬜⬜⬜⬛⬜⬛⬛⬛⬛⬜📕⬜⬛⬜ O
     *         → ⬜⬜⬜📕⬛📕⬛⬛⬛⬛⬜⬜⬜⬛⬜ X
     * ex) 1 7 2 ⬜⬛⬜⬜⬜⬛⬛⬛⬛⬜⬜⬜⬜⬛⬜
     *         → 📕⬛📕⬜⬜⬛⬛⬛⬛⬛⬜⬜📕⬛⬛ O
     * ex) 1 7 2 ⬜⬜⬜⬛⬜⬛⬛⬜⬛⬜⬜⬜⬜⬜⬜ → x
     */

    int paramCount = paramBlocks[0];
    if(countedBlocks.size() == paramCount) {  // 만약 수가 같다면? 모든 블록이 매칭이 되므로 sort하기 전에 위치 확정
      paramBlocks.erase(paramBlocks.begin());           // paramBlocks의 맨 앞의 meta data(param 개수)를 삭제
      for(int i = 0; i < paramCount; ++i) {
        if(paramBlocks[i] == countedBlocks[i].second) { // 블록 수가 같아야 확정할 수 있다
          if(countedBlocks[i].first - 1 >= 0)
            result[countedBlocks[i].first - 1] = -1;
          if(countedBlocks[i].first + paramBlocks[i] < result.size())
            result[countedBlocks[i].first + paramBlocks[i]] = -1;
        } else break; // 급한대로 bug fix
                      // 앞에서 순서대로 확인하니까 불록의 수가 맞다면 맞지만 아니면 break를 한다


        // else if(line[countedBlocks[i].first - 1] == -1) { // 무엇?
        //   ;
        // } else if(line[countedBlocks[i].first + countedBlocks[i].second] == -1) { // 무엇?
        //   ;
        // }
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

vector<int> Nemo::rules4(vector<int> rowcolInfo, vector<int> line) {
    vector<int> result = line;

    // 협소한 칸은 빈칸으로 간주
    // 3 ⬜⬜⬜📕⬜⬜📕⬜⬜⬜ → ⬜⬜⬜📕📕📕📕⬜⬜⬜
    // 확정빈칸으로 인해 param이 분리되어 겹치는 곳에 생성되는 확정블록
    // 3 3 ⬜⬜⬜⬜⬜📕⬜⬜⬜⬜ → ⬜⬜⬛⬜⬜📕⬜⬛⬛⬜
    // 4 📕📕📕⬜⬜⬜⬜⬜⬜⬜ → 📕📕📕⬜⬜⬜⬛⬜⬜⬜
    // 2 2 4 7 3 ⬛⬛📕⬜⬜⬜⬜⬜⬛⬜⬜📕⬜⬜⬜⬜⬜⬜⬜⬜⬜📕⬜⬜⬜
    //        → ⬛⬛📕⬜⬛⬜⬜⬛⬛⬛⬜📕⬜⬜⬛⬛⬛⬛⬛⬜⬜📕⬛⬛⬛


    // line이 하나 이상의 확정빈칸으로 분리되어 있어야 한다
    // ★ 분리된 empty space 중 min(param)보다 작은 것이 있다면 확정빈칸처리 한다

  // (1) 중간에 확정블록이 있으면 이를 통해 param을 유추한다,
  // (2) empty space의 수 param의 분포를 통해 각 empty space에 어떤 param이 들어갈 수 있는지 유추한다
  // (1)과 (2)를 통해 유추가 된 <param, empty space> pair가 있다면 겹치는 곳을 확정블록처리

  // (1) (2)는 어렵다고 판단

    // 오른쪽으로 가면서 확정빈칸으로 둘러쌓인 확정블록을 param과 비교해가며
    // 해당 param을 찾았다고 마킹한다
    // 마찬가지로 왼쪽에서도 오면서 찾은 param은 마킹한다
    // 불확정 칸으로 인해 애매하면 거기서 stop
    // 불확정 칸이 있어도 해당 param인지 아닌지는 판별할 수 있다
    // ex) 2 3 6 1 2 ⬛⬛📕⬜⬛⬛⬜⬜⬛⬜⬜⬜⬜⬜⬜⬜⬜📕⬛⬛
    //                ↑ 2    ↑ 3    ↑ 6      ↑ 1        ↑ 2

    // 확정빈칸으로 쌓인 블록 중에 unique한 param이 있으면 matcing 된다
    // 순서도 확인한다
    // ex) 2 2 1 3 6 1 2 ⬛⬛📕⬜⬜⬜⬜⬜⬜📕⬛📕⬜⬜⬜⬜📕⬛⬛⬛⬛⬛⬛⬛📕⬜⬜⬜⬜⬜

    int minParam = 0;
    for(int i = 1; i <= rowcolInfo[0]; ++i) {
      if(minParam == 0 || minParam > rowcolInfo[i])
        minParam = rowcolInfo[i];
    }

    vector<pair<int, int> > blanks;
    int start = 0;
    int end = line.size();
    for(int i=0; i<line.size(); ++i) {
      if(result[i] == -1) { // 확정빈칸
        if(i != start) // 연속된 확정빈칸인 경우를 핸들링
          blanks.push_back(pair<int, int>(start, i)); // 확정빈칸으로 분리된 불확정 칸의 범위
        start = i + 1;
      }
    }
    if(start != 0 && end - start != 1) // 끝에 도달했을 경우
      blanks.push_back(pair<int, int>(start, end));

  // blanks를 sort하는데 분리된 구획들이 큰 것부터 작은 순으로
  sort(blanks.begin(), blanks.end(), sortByBlockCount);

    for(int i = 0; i < blanks.size(); ++i) {
      // cout << blanks[i].first << " " << blanks[i].second;
      if(blanks[i].second - blanks[i].first < minParam) {
        for(int j = blanks[i].first; j < blanks[i].second; ++j) {
          result[j] = -1;
        }
        // cout << " -> consider blanks";
      }
      // cout << " -> consider blanks";
    }
    // cout << endl;
  }

void Nemo::bruteforce(vector<vector<int>> candiBoard, int step) {
  if(bfFound) {
    return;
  }

  if(CLEAR) {
    ClearScreen();
    printBoard(candiBoard, step);
  } else {
    cout << step << "\n";
  }

  // 다음 step에 따라서
  for(int i = 0; i < boardCombVector[step].size(); ++i) {
    vector<vector<int> > nextBoard = candiBoard;
    nextBoard[step] = boardCombVector[step][i];
    if(colCheckForBF(nextBoard, step + 1))
      bruteforce(nextBoard, step + 1);
  }
}

void Nemo::initCombVector() {
  // 빈 row만들어서
  vector<int> tempRow(ROW_SIZE, 0);
  // 위에서 아래로 각 row를 돌면서
  for(int i = 0; i < COL_SIZE; ++i) {
    // 각 row에서 만들어질 수 있는 경우의 수를 넣는 벡터, rowCombVector를 비워준다
    rowCombVector.clear();
    // cout << "step : " << i << "\n";
    // generateCombination을 하고나면 여러 가지 조합이 만들어진 tempRow들이 rowCombVector에 들어온다
    generateCombination(i, 0, tempRow, 0, false);
    // rowCombVector를 전체 조합 vector에 넣는다
    boardCombVector.push_back(rowCombVector);
  }
}

void Nemo::generateCombination(int step, int elementCount, vector<int> curVector, int position, bool previousCellPlaced) {
  if(position >= ROW_SIZE) { // end of the list
    /* count of the block of list is not matched with elementCount */
    if(row[step][0] == elementCount) {
      // cout << "\t";
      // printLine(curVector);
      rowCombVector.push_back(curVector);
    }
    return;
  }
  int leftBlockSum = 0;
  for(int i = elementCount + 1; i <= row[step][0]; i++) {
    if(i != elementCount + 1)
      leftBlockSum += 1;
    leftBlockSum += row[step][i];
  }

  if(ROW_SIZE - position >= leftBlockSum) {
    // place block at current position
    if(!previousCellPlaced && row[step][0] > elementCount){
      vector<int> nextRow1 = curVector;
      int pos = position;
      bool discard = false;
      for(int i=0; i<row[step][elementCount + 1]; i++) {
        if(board[step][pos] == -1) {
          discard = true;
          break;
        }
        nextRow1[pos++] = 1;
      }
      if(!discard)
        generateCombination(step, elementCount + 1, nextRow1, pos, true);
    }

    // put a space at current position
    vector<int> nextRow2 = curVector;
    if(board[step][position] != 1) {
      nextRow2[position] = -1;
      generateCombination(step, elementCount, nextRow2, position + 1, false);
    }

    // put a space at current position
    vector<int> nextRow2 = curVector;
    nextRow2[position] = 0;
    generateCombination(step, elementCount, nextRow2, position + 1, false);
  }
}

bool Nemo::colCheckForBF(vector<vector<int>> candiBoard, int rowStep) {
  /* 왼쪽부터 오른쪽으로 각 column을 돌면서 확인 */
  for(int i = 0; i < ROW_SIZE; ++i) {
    bool blockStart = false;
    int blockSum = 0;
    int blockCount = 1;

    /* 위에서 아래로 내려오면서 rowStep까지만 확인 */
    for(int j = 0; j < rowStep; ++j) {
      if(candiBoard[j][i] == 1) { /* 이전에 블록이 없었든, 있었든, 블록 카운트 */
        blockStart = true;
        blockSum++;
        if(col[i][blockCount] < blockSum)
          return false;
      } else if(blockStart) { /* 현재 블록이 나오지 않았으며 그 전에는 블록이 있었다! */
        if(col[i][blockCount] != blockSum)
          return false;
        blockCount++;
        blockSum = 0;
        blockStart = false;
      }
    }
  }
  if(rowStep == COL_SIZE) {
    bfFound = true;
    board = candiBoard;
  }
  return true;
}

void Nemo::printBoard(vector<vector<int>> targetBoard, int rowStep) {
  /* initialize */
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

  if(!BRUTEFORCE) {
    cout << "state : ";
    if(boardState.size() == 0)
    cout << "initialFill" << endl;
    else for(int i = 0; i < boardState.size(); ++i)
    wprintf(L"%c", direc[boardState[i]]);
    cout << endl;
  }

  /* print upper part */
  if(PARAM) {
    for(int i=0; i<maxCountCol; ++i) {
      /* print blanks as row parameters */
      for(int r=0; r<maxLengthRow + 1; ++r) cout << " ";
      /* print column parameters */
      for(int j=0; j<ROW_SIZE; ++j) {
        if(GRID && j != 0 && j % 5 == 0)
          cout << " ";
        if(maxCountCol - i <= col[j][0])
          cout << setw(2) << col[j][(i + 1) - (maxCountCol - col[j][0])];
        else
          cout << "  ";
      }
      cout << "\n";
    }
  }
  /* print lower part */
  for(int i = 0; i < rowStep; ++i) {
    if(PARAM) {
      int lengthParam = 0;
      for(int j = 1; j <= row[i][0]; ++j)
        lengthParam += (row[i][j] > 9 ? 3 : 2);

      /* print row parameters */
      for(int r = 0; r <= maxLengthRow - lengthParam; ++r)
        cout << " ";
      for(int j = 1; j <= row[i][0]; ++j)
        cout << row[i][j] << " ";
    }
    /* print board */
    for(int j = 0; j < ROW_SIZE; ++j) {
      if(GRID && j != 0 && j % 5 == 0) {
        if((i+1)%5 != 0)  wprintf(L"┃");
        else              wprintf(L"┫");
      }
      if(targetBoard[i][j] == 1)        wprintf(L"■");
      else if(targetBoard[i][j] == -1)  wprintf(L"▨");
      else                      wprintf(L"□");
    } cout << "\n";
  }
}

void Nemo::printLine(vector<int> line) {
  for(int i = 0; i < line.size(); ++i) {
    if(line[i] == 1)        wprintf(L"■");
    else if(line[i] == -1)  wprintf(L"▨");
    else                    wprintf(L"□");
  }
  cout << " size : " << line.size() << "\n";
}
