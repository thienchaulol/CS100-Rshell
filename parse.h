#include<vector>
#include<string>
#include<boost/tokenizer.hpp>
#include<boost/algorithm/string.hpp>

using namespace std;
using namespace boost;

vector<string> parse(string text) {
  vector<string> tokenList;
  split(tokenList, text, is_any_of(" "), token_compress_on);
  return tokenList;
}
