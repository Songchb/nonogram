#include <iostream>
#include <vector>

using namespace std;

int P_SIZE = 10;

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

  void printMap() {
    for(int i=0; i<P_SIZE; ++i) {
      for(int j=0; j<P_SIZE; ++j)
        cout << map[i][j] << " ";
      cout << "\n";
    }
    cout << "*******************\n";
  }
};

int main() {
  PMap a;
  for(int i = 0; i<P_SIZE; ++i)
    for(int j=0; j<P_SIZE; ++j)
      a.map[i][j] = i*P_SIZE + j;

  a.printMap();
  PMap b = a;
  b.printMap();

  b.map[5][5] = 99999;
  b.printMap();
  a.printMap();
  vector<int> temp;
  for(int i=0; i<P_SIZE; i++)
    temp.push_back(0);
  a.map[3] = temp;
  a.printMap();
}
