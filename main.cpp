#include<iostream>
#include<string>
#include<vector>
#include<boost/foreach.hpp> 

#include"parse.h"

using namespace std;
using namespace boost;

int main() {
  string text = "This is; a      test";
  vector<string> tokenList = parse(text);
  BOOST_FOREACH(string t, tokenList) {
    cout << t << endl;
  }
  return 0;
}
