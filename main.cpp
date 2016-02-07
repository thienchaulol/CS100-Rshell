#include <iostream>
#include<boost/tokenizer.hpp>
#include<string>

using namespace std;
using namespace boost;

int main() {
  string s = "This is; a      test";
  tokenizer<> tok(s);
  for (tokenizer<>::iterator beg=tok.begin(); beg!=tok.end(); ++beg) {
    cout << *beg << "\n";
  }
  return 0;
}
