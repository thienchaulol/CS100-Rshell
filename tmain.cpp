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

vector<pair<char, bool> > hashAndSlash(string str) {
  vector<pair<char, bool> > v;
  bool isInWord = false;
  bool isAfterBackslash = false;

  for(int i = 0; i < str.size(); i++) {
    if(str.at(i) == ' ') {
      isInWord = false;
      isAfterBackslash = false;
    }
    else if(str.at(i) != ' ' && str.at(i) != '#') {
      isInWord = true;
      if(str.at(i) == '\\' && !isAfterBackslash) {
	v.push_back(pair<char, bool>(str.at(i), isAfterBackslash));
	isAfterBackslash = true;
	if(i + 1 <= str.size() - 1)
	  i++;
      }
    }
    else if(str.at(i) == '#' && !isInWord)
      break;
    v.push_back(pair<char, bool>(str.at(i), isAfterBackslash));
    isAfterBackslash = false;
  }

  return v;
}

vector<pair<string, bool> > parse(vector<pair<char, bool> > v) {
  vector<pair<string, bool> > s;
  
  while(!v.empty()){
    string command;
    string connector;
    
    while(v.at(0).first == ' ')
      v.erase(v.begin());

    int i = 0;
    for(i = 0; i < v.size(); i++) {
      if(v.at(i).first == '&' && v.at(i).second == false)
	break;
      else if(v.at(i).first == '|' && v.at(i).second == false)
	break;
      else if(v.at(i).first == ';' && v.at(i).second == false)
	break;
    }

    for(int j = 0; j < i; j++) {
      if(v.at(0).first == '\\' && v.at(0).second == false)
	v.erase(v.begin());
      else {
	command += v.at(0).first;
	v.erase(v.begin());
      }
    }

    if(!v.empty()) {
      if(v.at(0).first == '&' || v.at(0).first == '|') {
	connector += v.at(0).first;
	v.erase(v.begin());
      }
      if(v.at(0).first == '&' || v.at(0).first == '|') {
	connector += v.at(0).first;
	v.erase(v.begin());
      }
      else if(v.at(0).first == ';') {
	connector += v.at(0).first;
	v.erase(v.begin());
      }
    }

    if(!command.empty()) 
      s.push_back(pair<string, bool>(command, false));
    if(!connector.empty())
      s.push_back(pair<string, bool>(connector, true));
  }

  return s;
}

int main() {

  while(1) {
    string line;

    cout << "$ ";
    getline(cin, line);

    char *argv[line.size() + 1];
    vector<pair<char, bool> > specVec = hashAndSlash(line);

    cout << "specVec.first and specVec.second:" << endl;
    for(int i = 0; i < specVec.size(); i++)
      cout << specVec.at(i).first;
    cout << endl;
    for(int i = 0; i < specVec.size(); i++)
      cout<< specVec.at(i).second;
    cout << endl << endl;

    vector<pair<string, bool> > parsedVec = parse(specVec);

    cout << "parsedVec.second and parsedVec.first:" << endl;
    for(int i = 0; i < parsedVec.size(); i++) 
      cout << parsedVec.at(i).second << ' ' << parsedVec.at(i).first << endl;
    cout << endl;

    cout << "output:" << endl;

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
