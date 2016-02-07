#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>

using namespace std;

void finalParse(char *line, char **argv) {
  while(*line != '\0') {
    while(*line == ' ' || *line == '\t' || *line == '\n') {
      *line++ = '\0';
    }
    *argv++ = line;
    while(*line != '\0' && *line != ' ' && *line != '\t' && *line != '\n') {
      line++;
    }
  }
  *argv = '\0';
}

void execute(char **argv) {
  pid_t pid;
  int status;

  if((pid = fork()) < 0) {
    cout << "Error: Forking child process failed" << endl;
    exit(1);
  }
  else if(pid == 0) {
    if(execvp(*argv, argv) < 0) {
      cout << "Error: exec failed" << endl;
      exit(1);
    }
  }
  else {
    while(wait(&status) != pid) {
    }
  }
}

vector<pair<char, bool> > strToVec(string str) {
  vector<pair<char, bool> > specVec;
  bool isInWord = false;
  bool isAfterBackslash = false;

  for(int i = 0; i < str.size(); i++) {
    if(str.at(i) == ' ') {
      isInWord = false;
      isAfterBackslash = false;
    }
    else if(str.at(i) != ' ' && str.at(i) != '#') {
      isInWord = true;
      if(str.at(i) == '\\') {
	isAfterBackslash = true;
	specVec.push_back(pair<char, bool>(str.at(i), isAfterBackslash));
	if(i + 1 <= str.size() - 1)
	  i++;
      }
    }
    else if(str.at(i) == '#' && !isInWord)
      break;
    specVec.push_back(pair<char, bool>(str.at(i), isAfterBackslash));
    isAfterBackslash = false;
  }

  return specVec;
}

int main() {

  while(1) {
    string line;

    cout << "$ ";
    getline(cin, line);

    char *argv[line.size() + 1];
    vector<pair<char, bool> > specVec = strToVec(line);

    for(int i = 0; i < specVec.size(); i++)
      cout << specVec.at(i).first;
    cout << endl;

    for(int i = 0; i < specVec.size(); i++)
      cout<< specVec.at(i).second;
    cout << endl;

    char *c = new char[line.size() + 1];
    copy(line.begin(), line.end(), c);
    c[line.size()] = '\0';

    finalParse(c, argv);
    if(strcmp(argv[0], "exit") == 0) {
      exit(0);
    }
    execute(argv);
  }
  return 0;
}
