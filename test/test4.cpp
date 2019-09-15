#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;

void printVector(vector<vector<int> > v) {
  for(int i=0; i<v.size(); ++i) {
    for(int j=0; j<v[i].size(); ++j) {
      cout << setw(3) << v[i][j] << " ";
    } cout << endl;
  } cout << "----print done----" << endl;
}

int main() {
  vector<vector<int> > a;
  vector<vector<int> > b;
  printVector(a);
  printVector(b);
  if(a == b)
    cout << "a == b" << endl;
  else cout << "a != b " << endl;

  vector<int> temp;
  a.push_back(temp);
  cout << "*** after push_back temp ***" << endl;

  printVector(a);
  printVector(b);
  if(a == b)
    cout << "a == b" << endl;
  else cout << "a != b " << endl;

  cout << "*** c and d ***" << endl;

  vector<vector<int> > c;
  for(int i=0; i<10; ++i) {
    vector<int> temp2;
    for(int j=0; j<10; ++j)
      temp2.push_back(i*10 + j);
    c.push_back(temp2);
  }
  vector<vector<int> > d = c;

  printVector(c);
  printVector(d);
  if(c == d)
    cout << "c == d" << endl;
  else cout << "c != d " << endl;

  vector<int> temp3;
  for(int i=0; i<10; ++i)
    temp3.push_back(0);
  c[3] = temp3;

  cout << "*** after modifying c***" << endl;
  printVector(c);
  printVector(d);
  if(c == d)
    cout << "c == d" << endl;
  else cout << "c != d " << endl;

  return 0;
}
