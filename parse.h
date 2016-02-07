#include<iostream>
#include<vector>
#include<string>
#include<boost/tokenizer.hpp>
#include<boost/algorithm/string.hpp>

std::vector<std::string> parse(std::string text) {
  
  std::vector<std::string> tokenList;

  split(tokenList, text, boost::is_any_of(" "), boost::token_compress_on);

  int tokenListSize = tokenList.size();
  for(int i = 0; i < tokenList.size(); i++) {
    if(tokenList.at(i).at(tokenList.at(i).size() - 1) == ';' && tokenList.at(i).size() != 1) {
      tokenList.at(i).erase(tokenList.at(i).size() - 1, 1);
      tokenList.insert(tokenList.begin() + i + 1, ";");
    }
  }

  return tokenList;
}
