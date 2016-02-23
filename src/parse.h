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

vector<pair<string, bool> > parse(vector<pair<char, bool> > v) {
  vector<pair<string, bool> > s;								

  while(!v.empty()){
    string command;														
    string connector;													
    
    while(v.at(0).first == ' ')										
      v.erase(v.begin());											

    unsigned i = 0;
    for(i = 0; i < v.size(); i++){										
      if(v.at(i).first == '&' && !v.at(i).second)				
	break;
      else if(v.at(i).first == '|' && !v.at(i).second)				
	break;
      else if(v.at(i).first == ';' && !v.at(i).second)			
	break;
    }

    for(unsigned j = 0; j < i; j++) {									
      if(v.at(0).first == '\\' && !v.at(0).second)		
	v.erase(v.begin());
      else if(v.at(0).first == '\"' && !v.at(0).second) {
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
      }
    }

    if(!command.empty()) {
      while(command.at(command.size() - 1) == ' '
	    || command.at(command.size() - 1) == '\t'
	    || command.at(command.size() - 1) == '\n') 
	command.erase(command.size() - 1, 1);											
      s.push_back(pair<string, bool>(command, false));
    }						
    if(!connector.empty())											
      s.push_back(pair<string, bool>(connector, true));						
  }

  while((s.at(s.size() - 1).second && s.at(s.size() - 1).first == "&&")
	|| (s.at(s.size() - 1).second && s.at(s.size() - 1).first == "||")
	|| (s.at(s.size() - 1).second && s.at(s.size() - 1).first == "|")) {	
    string line;
    
    cout << "> ";
    getline(cin, line);												

    vector<pair<char, bool> > v2 = hashAndSlash(line);				

    while(!v2.empty()){													
      string command;														
      string connector;														
    
      while(v2.at(0).first == ' ')
	v2.erase(v2.begin());

      unsigned i = 0;
      for(i = 0; i < v2.size(); i++) {
	if(v2.at(i).first == '&' && !v2.at(i).second)
	  break;
	else if(v2.at(i).first == '|' && !v2.at(i).second)
	  break;
	else if(v2.at(i).first == ';' && !v2.at(i).second)
	  break;
      }

      for(unsigned j = 0; j < i; j++) {
	if(v2.at(0).first == '\\' && !v2.at(0).second)
	  v2.erase(v2.begin());
	else if(v.at(0).first == '\"' && !v.at(0).second) {
	  v.erase(v.begin());
	  command += v.at(0).first;
	}
	else {
	  command += v2.at(0).first;
	  v2.erase(v2.begin());
	}
      }

      if(!v2.empty()) {
	if(v2.at(0).first == '&' || v2.at(0).first == '|') {
	  connector += v2.at(0).first;
	  v2.erase(v2.begin());
	}
	if(v2.at(0).first == '&' || v2.at(0).first == '|') {
	  connector += v2.at(0).first;
	  v2.erase(v2.begin());
	}
	else if(v2.at(0).first == ';') {
	  connector += v2.at(0).first;
	  v2.erase(v2.begin());
	}
      }
      
      if(!command.empty()) {
	while(command.at(command.size() - 1) == ' '
	      || command.at(command.size() - 1) == '\t'
	      || command.at(command.size() - 1) == '\n') 
	  command.erase(command.size() - 1, 1);
	s.push_back(pair<string, bool>(command, false));
      }
      if(!connector.empty())
	s.push_back(pair<string, bool>(connector, true));      
    }
  }

  return s;			
}

void run(vector<pair<string, bool> > v) {		
  bool prevCom = true; 							
  bool doIRun = true;						
  
  if(v.at(0).second == 1) {
    cout << "syntax error near unexpected token `" << v.at(0).first << "'" << endl;
    return;
  }
  while(!v.empty()) {							
    if(v.at(0).second == 0 && doIRun) {	
      unsigned lineSize = v.at(0).first.size() + 1;
      char **argv = new char*[lineSize];
      
      char *c = new char[lineSize];
      copy(v.at(0).first.begin(), v.at(0).first.end(), c);
      c[v.at(0).first.size()] = '\0';
      
      finalParse(c, argv);
      char curDir[100];
      getcwd(curDir, 100);
      if(strcmp(argv[0], "exit") == 0) 
	exit(0);
      if(strcmp(*argv, "cd") == 0) 
	chdir(argv[1]);
      else {
	prevCom = execute(argv);		
	v.erase(v.begin());				
      }
    }  
    else if(v.at(0).second == 0 && !doIRun) 
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
