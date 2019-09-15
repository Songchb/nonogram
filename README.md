# Implementing Steps of Nonogram Solver
=============
* nemo2
  - emplemented on the Mac
  - Brute Force (worked fine)
  - need to encode character since OS is changed from Mac to Windows

* nemo3
  - handling output problem of character
  - merge the branch code of input data
    + input puzzle directly
    + input text file

* nemo4
  - optimize fileRead using rdbuf()

* nemo5
  - memory allocated *tempVector* using new is modified to *vector<int>*
  - invalid vector index access bug in line 189 modified
    + *map[step]* is modified to *map[step - P_SIZE]*
  - execution time is output

* nemo6
  - boolean flag *certain*
  - if there's no overlapping block between any pair of two combinations
  - pass the leftover

* nemo7
  - 블록이 중복되어 확실한 자리 구하는 알고리즘 최적화
  - 끝 부분에 블록이 자리하여, 그 밑으로 또 확실해지는 블록들 색칠하는 알고리즘

* nemo8
  - 확실한 빈칸도 표시
  - optimized function *printMap()*
  - 가장자리부터 안으로 count가 확실하면 색칠하기
  - bug fixed
    - 칠한 칸의 합이 param의 합과 같으면 불확정칸 모두 빈칸처리
  - execution option 구현
    - -np, --no-parameters
    - -g, --grid

* nemo9
  - 블록 수가 param 수와 같을 때 블록 한 쪽에 빈칸 확정이면 다른 쪽에 블록 칠하고 빈칸처리까지 하기
  - 블록 수가 param 수와 같지 않더라도 블록 수가 커서 param이 확정지어질 때 다른 한 쪽에 빈칸 확정이면 다른 쪽에 블록 칠하고 빈칸처리 하기
  - 협소한 곳 빈칸 확정하기
  - main함수 while문으로 지속적으로 입력받게 하기?
  - BRUTE FORCE 할 수 있게 만들기
  - 확정된 빈칸 사이의 param들만 따로 빼서 initialFill처럼 중복된 자리 다시 찾기

* TODO  
  - 이미 색칠된 맵에 대해서 combination pruning 진행
  + 생성된 조합이 현재 맵의 색칠된 곳과 맞지 않으면 해당 조합은 삭제
  - checkCol, checkRow 함수 만들기
---------------------------------------
* pruning rules
  + 모든 검은 칸, 모든 빈칸이 확정된 예시의 처리
    (예시1)
    1 ⬜⬜⬜⬛⬜⬜⬜⬜⬜⬜
    1 ✉✉✉⬛✉✉✉✉✉✉
    (예시2)
    1 1 1 1 ✉⬜✉⬛✉⬜✉✉✉⬜
    1 1 1 1 ✉⬛✉⬛✉⬛✉✉✉⬛

  + 검은 칸의 이웃 칸 처리
    (예시1)
    1 2 1 ⬜⬜⬜⬛⬛⬜⬜⬜⬜⬜
    1 2 1 ⬜⬜✉⬛⬛✉⬜⬜⬜⬜

  + 가장자리의 처리
    (예시1)
    3 2 ⬛⬜⬜⬜⬜⬜⬜⬜⬜⬜
    3 2 ⬛⬛⬛✉⬜⬜⬜⬜⬜⬜
    (예시2)
    3 2 ⬜⬜⬜⬜⬜⬜⬜⬜⬜⬛
    3 2 ⬜⬜⬜⬜⬜⬜⬜✉⬛⬛

  + 협소한 칸의 처리 (빈칸으로 간주)
    (예시2)
    3 ⬜⬜⬜⬜⬜⬜⬜✉⬜⬜
    3 ⬜⬜⬜⬜⬜⬜⬜✉✉✉
    (예시1)
    3 ⬜⬜⬜✉⬜⬜✉⬜⬜⬜
    3 ⬜⬜⬜✉✉✉✉⬜⬜⬜

  + 확실히 검은 칸이 닿는 칸, 닿지 않는 칸을 처리
    (예시1)
    2 ⬜⬛⬜⬜⬜⬜⬜⬜⬜⬜
    2 ⬜⬛⬜✉✉✉✉✉✉✉
    (예시2)
    5 ⬜⬜⬜⬜⬜⬜⬜⬛⬜⬜
    5 ✉✉✉⬜⬜⬛⬛⬛⬜⬜
    (예시3)
    3 4 ⬜⬜⬛⬜⬜⬜⬛⬜⬜⬜
    3 4 ⬜⬜⬛⬜⬜⬜⬛⬛⬜⬜

  + 가장자리나 최고값의 갱신에 대한 처리
    (예시1)
    5 ⬜⬜⬜⬜⬜⬜⬜⬜⬜✉
    5 ⬜⬜⬜⬜⬛⬜⬜⬜⬜✉
    (예시2)
    1 1 1 1 ⬜⬜⬜⬜⬜⬜⬜✉✉✉
    1 1 1 1 ⬛⬜⬛⬜⬛⬜⬛✉✉✉

  + 나눠진 칸의 처리
    (예시)
    3 3 ⬜⬜⬜⬜⬜✉⬜⬜⬜⬜
    3 3 ⬜⬜⬛⬜⬜✉⬜⬛⬛⬜



  - 칠하면 안됨
      5  ⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜
      5  ✉✉✉⬜⬜⬜⬜⬜⬜⬜
      5  ⬜⬜⬛⬜⬜⬜⬜⬜⬜⬜
      5  ✉✉⬛✉⬜⬜⬜⬜⬜⬜

  - 칠합시다
      5  ⬛⬜⬜⬜⬜⬜⬜⬜⬜⬜
      5  ⬛⬛⬜⬜⬜⬜⬜⬜⬜⬜
    5 2  ⬛⬜⬜⬜⬛✉⬛⬜⬜⬜
      5  ✉✉✉⬛⬜⬜⬜⬜⬜⬜
      5  ✉✉✉⬛⬛⬜⬜⬜⬜⬜
    2 5  ✉⬛⬛✉⬛⬛⬜⬜⬜⬜

* mergeStart, mergeEnd, nextBlockCnt
  - don't need *mergeEnd* anymore

         ⬜⬜⬜⬜⬜⬛⬜⬜⬛⬛⬛⬜⬜⬛⬛⬛⬜⬜⬜⬜⬜⬜⬜

         ⬜⬜⬜⬜⬜⬛ ⬜ ⬜ ⬛⬛ ⬛ ⬜ ⬜ ⬛ ⬛ ⬛ ⬜ ⬜⬜⬜⬜⬜⬜
         0 1 2 3 4 5  6  7 8 9 10 11 12 13 14 15 16
mgStart               s  s 8 8  8 -1 -1 13 13 13 -1
nxtBCnt               0  0 1 2  3  3  3  3  3  3  3


mgEnd              s -1 -1 8 9 10 10 10 13 14 15 15

        ⬜⬜⬛⬛⬛⬛⬜ ⬜⬛⬛ ⬛ ⬜ ⬜ ⬛ ⬛ ⬛ ⬜⬜⬜⬜⬜⬜⬜
        0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16        
mgStart             s s 8 8  8 -1 -1 13 13 13 -1
nxtBCnt             0 0 1 2  3  3  3  3  3  3  3

mgEnd       1 2 3 4 4 4 8 9 10 10 10 13 14 15 15

* bug of rules3
  - 0부터 가면서 블록들 셈,

* rules3를 없앤다?
  - 기존의 rules는 현재의 블록들과 param을 match시켜서 맞으면 각 블록에 대해 마감처리만 해준다
  - param들 중에
    블록들 세서 가장 많은게 param들 중 max와 비교 같으면 마감처리?
  - rules3는 보류

  1 2 1 ⬜⬜⬜⬛⬛⬜⬜⬜⬜⬜
  1 2 1 ⬜⬜✉⬛⬛✉⬜⬜⬜⬜

  1 4 5 1 ⬜⬜⬜⬜⬛⬛⬛⬛⬜⬜⬜⬜⬛⬛⬛⬛⬜⬜⬜⬜
  1 4 5 1 ⬜⬜⬜✉⬛⬛⬛⬛✉⬜⬜⬜⬛⬛⬛⬛⬜⬜⬜⬜

  1 4 5 1 ⬜⬜⬜⬜⬛⬛⬛⬛⬜⬜⬜⬜⬛⬛⬛⬜⬜⬜⬜⬜
  max 블록부터 param이랑 비교
  <position, blockCount>
  <4, 4>
  <12, 3>
  순으로

  param은 5, 4, 1, 1순으로 비교 (같으면 앞에서부터)

  <4, 4> == param 5
  position (4) 앞에 param의 순서로 5의 앞에 있는 1, 4가 올 수 있는지 확인
    (즉, 블록이 param 5라면 param 순서로 5 앞에 있는 1, 4가 해당 블록 앞에 올 수 있어야 한다)
  position이 4니까 1, 4가 올 수 없다

  param의 다음 순서로 넘어감 4
  if(blockCount > param)이면 break;

  position 4 앞에 1이 올 수 있다
  있다면 블록 수 세서 일치하면 앞뒤 마감 처리!

  if( max(blockCount) > descending order 두번째 param )
    해당 block은 max param이 확실하므로
    왼쪽으로 이동, 오른쪽으로 이동 하는 처리
  - 이것도 문제인게 블록이 또 나오면 merge할지 말지, param을 보면서 판단해야 하는데
  - 확정빈칸이 나온다고 해도  


  1 2 11 8 ⬜⬜⬜⬜⬜✉⬛⬜⬛⬛⬛⬛⬛⬛⬛⬛⬜⬜✉⬛⬛⬜⬛⬛⬛⬛⬛⬜⬜⬜


  1 2 11 8 ⬜⬜⬜⬜⬜⬜⬛⬜⬛⬛⬛⬛⬛⬛⬛⬛⬜⬜✉⬛⬛⬜⬛⬛⬛⬛⬛⬜⬜⬜

  1 2 8 5 ⬜⬜⬜⬜⬜⬜✉⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜✉⬜⬜⬜⬜⬜⬜⬜
  1 2 8 5 ⬜⬜⬜⬜⬜⬜✉⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜✉⬜⬜⬜⬜⬜⬜⬜
  1 2 8 5 ⬜⬛⬜⬜⬜⬜✉⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜✉⬜⬜⬜⬜⬜⬜⬜
