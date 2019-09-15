#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>

using namespace std;

vector<int> global;

void func(vector<int> vec, int number){
  for(int i=0; i<vec.size(); i++)
    cout << vec.at(i) << " ";
  cout << endl;
  if(number < 10) {
    vec.push_back(number);
    func(vec, number+1);
  }
}

void printVector(vector< vector<int> > doubleVec) {
  cout << "size : " << doubleVec.size() << endl;
  for(int i=0; i<doubleVec.size(); i++) {
    for(int j=0; j<doubleVec.at(i).size(); j++) {
      cout << setw(8) << doubleVec.at(i).at(j);
    }
    cout << endl;
  }
}

int main()
{
  // string inputfile;
  // cin >> inputfile;
  // inputfile = "Input/" + inputfile;
  // ifstream input(inputfile);
  //
  // int hey;
  // input >> hey;
  //
  // cout << "input integer : " << hey << endl;
  //
  //
  // cin.rdbuf(input.rdbuf());
  // cin >> hey;
  // cout << "input integer : " << hey << endl;

  // string s1 = "input.txt";
  // cout << (s1.find('.') == string::npos) << endl;

  cout << true << " " << true << " " << false << endl;

  return 0;
}
