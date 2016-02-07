#include<iostream>
#include<boost/tokenizer.hpp>
#include<string>

using namespace std;
using namespace boost;

int main() {
  string text = "This is; a      test";
  char_separator<char> sep(" ");

  tokenizer<char_separator<char> > tokens(text, sep);
  for (tokenizer<char_separator<char> >::iterator beg=tokens.begin(); beg!=tokens.end(); ++beg) {
    cout << *beg << "\n";
  }
  return 0;
}
