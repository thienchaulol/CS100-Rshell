#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <sys/types.h>
#include <pwd.h>

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

bool execute(char **argv){
  pid_t pid;
  int status;
  bool succeeded = true; 			
  if((pid = fork()) < 0) {
    cout << "Error: Forking child process failed" << endl;
    exit(1);
  }
  else if(pid > 0) {
    while (wait(&status) != pid);

    if(WIFEXITED(status)) {
      if(WEXITSTATUS(status) != 0) {
	succeeded = false;
      }
    }
    else {
      succeeded = false;
    }
  }
  else {
    if(execvp(*argv, argv) < 0) {
      perror(*argv);
      succeeded = false;
      kill(getpid(), 2);
    }
  }
  return succeeded;
}

bool paranChecker(vector<pair<char, bool> > v) {
  bool isClosed = true;
  vector<pair<char, bool> >::iterator open = v.begin();
  vector<pair<char, bool> >::iterator closed = v.end();

  for(closed = v.begin(); closed != v.end(); closed++) {
    if(!closed->second && closed->first == ')') {
      isClosed = false;
      for(open = closed; open != v.begin(); open--) {
	if(!open->second && open->first == '(') {
	  cout << "test" << endl;
	  open->second = true;
	  isClosed = true;
	  break;
	}
      }
    }
  }
  if(!isClosed && !v.at(0).second && v.at(0).first == '(')
    return true;
  return isClosed;
}

vector<pair<char, bool> > hashAndSlash(string str) {
  vector<pair<char, bool> > v;

  bool isInWord = false;
  bool isAfterBackslash = false;
  bool allQuotesFound = true;
  bool isNormalChar = false;
  bool doWhile = true;
  int paranCount = 0;
  string line = str;

  while(doWhile) { 
    for(unsigned i = 0; i < line.size(); i++) {
      if(line.at(i) == ' ') {
	isInWord = false;
	v.push_back(pair<char, bool>(line.at(i), isNormalChar));
      }
      else {
	if(line.at(i) == '#' && !isInWord && !isNormalChar)
	  break;
	else if(line.at(i) == '\\' && (!isAfterBackslash || !isNormalChar)) {
	  if(i + 1 <= line.size() - 1) {
	    i++;
	    v.push_back(pair<char, bool>(line.at(i), true));
	  }
	}
	else if(line.at(i) == '"') {
	  if(allQuotesFound) {
	    allQuotesFound = false;
	    isNormalChar = true;
	    if(i + 1 <= line.size() - 1) {
	      i++;
	      v.push_back(pair<char, bool>(line.at(i), true));
	    }
	  }
	  else if(!allQuotesFound) {
	    allQuotesFound = true;
	    isNormalChar = false;
	    if(i + 1 <= line.size() - 1) {
	      i++;
	      v.push_back(pair<char, bool>(line.at(i), false));
	    }
	  }
	}
	else if(line.at(i) == '(' && !isNormalChar) {
	  paranCount++;
	  v.push_back(pair<char, bool>(line.at(i), false));
	}
	else if(line.at(i) == ')' && !isNormalChar) {
	  paranCount--;
	  v.push_back(pair<char, bool>(line.at(i), false));
	}
	else
	  v.push_back(pair<char, bool>(line.at(i), isNormalChar));
	isInWord = true;
      }
    }
    if(!paranChecker(v)) {
      while(!v.empty())
	v.pop_back();
      return v;
    }
    if(allQuotesFound && paranCount == 0) {
      doWhile = false;
    }
    else{
      cout << "> ";
      getline(cin, line);
    }
  }
  return v;
}

vector<pair<string, bool> > parse(vector<pair<char, bool> > specVec) {
  vector<pair<char, bool> > v = specVec;
  vector<pair<string, bool> > s;
  bool doWhile = true;

  while(doWhile) {
    while(!v.empty()) {
      string command;
      string connector;

      while(v.at(0).first == ' '
	    || v.at(0).first == '\t'
	    || v.at(0).first == '\n')
	v.erase(v.begin());

      while(v.at(v.size() - 1).first == ' '
	    || v.at(v.size() - 1).first == '\t'
	    || v.at(v.size() - 1).first == '\n')
	v.erase(v.end());
	 
      unsigned i = 0;
      for(i = 0; i < v.size(); i++) {
	if(v.at(i).first == '&' && !v.at(i).second)
	  break;
	else if(v.at(i).first == '|' && !v.at(i).second)
	  break;
	else if(v.at(i).first == ';' && !v.at(i).second)
	  break;
	else if(v.at(i).first == '(' && !v.at(i).second)
	  break;
	else if(v.at(i).first == ')' && !v.at(i).second)
	  break;
      }

      for(unsigned j = 0; j < i; j++) {
	if(v.at(0).first == '\\' && !v.at(0).second)
	  v.erase(v.begin());
	else if(v.at(0).first == '"' && !v.at(0).second) {
	  command += v.at(0).first;
	  v.erase(v.begin());
	}
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
	if(!v.empty()) {
	  if(v.at(0).first == '&' || v.at(0).first == '|') {
	    connector += v.at(0).first;
	    v.erase(v.begin());
	  }
	  else if(v.at(0).first == ';') {
	    connector += v.at(0).first;
	    v.erase(v.begin());
	  }
	  else if(v.at(0).first == '(') {
	    connector += v.at(0).first;
	    v.erase(v.begin());
	  }
	  else if(v.at(0).first == ')') {
	    connector += v.at(0).first;
	    v.erase(v.begin());
	  }
	}
      }
      
      if(!command.empty()) {
	while(command.at(command.size() - 1) == ' '
	      || command.at(command.size() - 1) == '\t'
	      || command.at(command.size() - 1) == '\n')
	  command.erase(command.size() - 1, 1);
	if(!command.empty())
	  s.push_back(pair<string, bool>(command, false));
      }
      if(!connector.empty())
	s.push_back(pair<string, bool>(connector, true));
    }

    if(!((s.at(s.size() - 1).second && s.at(s.size() - 1).first == "&&")
	 || (s.at(s.size() - 1).second && s.at(s.size() - 1).first == "||")
	 || (s.at(s.size() - 1).second && s.at(s.size() - 1).first == "|")))
      doWhile = false;
    else {
      string line;

      cout << "> ";
      getline(cin, line);

      v = hashAndSlash(line);
    }
  }
  return s;
}

void run(vector<pair<string, bool> > v) {		
  bool prevCom = true; 							
  bool doIRun = true;						
  
  if((v.at(0).second && v.at(0).first.at(0) == '&')
     || (v.at(0).second && v.at(0).first.at(0) == '|')
     || (v.at(0).second && v.at(0).first.at(0) == ';')) {
    cout << "syntax error near unexpected token `" << v.at(0).first << "'" << endl;
    return;
  }
  while(!v.empty()) {
    if(!v.at(0).second && doIRun) {	
      unsigned lineSize = v.at(0).first.size() + 1;
      char **argv = new char*[lineSize];
      
      char *c = new char[lineSize];
      copy(v.at(0).first.begin(), v.at(0).first.end(), c);
      c[v.at(0).first.size()] = '\0';
      
      finalParse(c, argv);
      char curDir[100];
      getcwd(curDir, 100);
      if(strcmp(*argv, "exit") == 0) 
	exit(0);
      if(strcmp(*argv, "cd") == 0) 
	chdir(argv[1]);
      /*
//----------------------------------------2/24/2016
	//if the user inputs test, go to seperate case that doesn't use execvp
	if(strcmp(*argv, "test") == 0){
		//call a seperate function to complete the command
		
		//cout << "Test case" << endl;
		//exit(0);	
		
		//run_test(vector<pair<string,bool>> vec) is a function that will
		//run our own implemented version of the bash test command
		run_test(v);
	}
	if(strcmp(*argv, "[") == 0){
		//call a seperate function to complete the command
	
		//cout << "Bracket case" << endl;
		//exit(0);
		
		//run_bracketTest(vector<pair<string,bool>> vec) is a function that will 
		//run our own implemented version of the bash test command
		run_bracketTest(v);
	}
//----------------------------------------2/24/2016
*/
      else {
	prevCom = execute(argv);		
	v.erase(v.begin());				
      }
    }
    else if(v.at(0).second && !doIRun && v.at(0).first == "(") {
      int paranCount = 0;
      do {
	if(v.at(0).second && v.at(0).first == "(")
	  paranCount++;
	else if(v.at(0).second && v.at(0).first == ")")
	  paranCount--;
	v.erase(v.begin());
      } while(paranCount != 0);
    }
    else if(!v.at(0).second && !doIRun) 
      v.erase(v.begin());					
    if(!v.empty()) {					
      if(v.at(0).first == "&&") {			
	if(prevCom)									
	  doIRun = true;							
	else
	  doIRun = false;						
      } 
      else if(v.at(0).first == "||" || v.at(0).first == "|") {	
	if(prevCom)	
	  doIRun = false;		
	else
	  doIRun = true;
      }
      else if(v.at(0).first == "&") {					
	cout << "syntax error near unexpected token '" << v.at(0).first << "'" << endl;	
	return;
      }
      else if(v.at(0).first == ";") 			
	doIRun = true;								
      v.erase(v.begin());						
    }
  }//why is git so difficult!
}
//----------------------------------------2/24/2016
void run_test(vector<pair<string,bool> > vec){
	//the user's parsed input will be passed in
	//if it begins with "test"
}

void run_bracketTest(vector<pair<string,bool> > vec){
	//the user's parsed input will be passed in
	//if it begins with "["
}
//----------------------------------------2/24/2016
