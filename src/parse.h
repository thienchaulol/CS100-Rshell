#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <sys/types.h>
#include <pwd.h>

#include <sys/stat.h>	//assignment 3
#include <cstring>
#include <string>
#include <limits.h>
#include <string.h>

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

//---assign 3
	else if(v.at(i).first == '-' && !v.at(i).second)
		break;
	else if(v.at(i).first == '/' && !v.at(i).second)
		break;
	else if(v.at(i).first == '[' && !v.at(i).second)
		break;
	else if(v.at(i).first == ']' && !v.at(i).second)
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

//treat '-' as a connector
	if(!v.empty()){
		if(v.at(0).first == '-'){
			//add '-' to connector
			v.erase(v.begin());
			//add 'e', 'f', or 'd' to connector
			connector += v.at(0).first;
			//erase
			v.erase(v.begin());
		}
	}

//treat '/' as a connector
	if(!v.empty()){
		if(v.at(0).first == '/'){
			connector += v.at(0).first;
			v.erase(v.begin());
		}
	}

//treat '[' as a connector
	if(!v.empty()){
		if(v.at(0).first == '['){
			connector += v.at(0).first;
			v.erase(v.begin());
		}
	}
//treat ']' as a connector
	if(!v.empty()){
		if(v.at(0).first == ']'){
			connector += v.at(0).first;
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
      if(!connector.empty()){
		//assignment 3, treat '[' as a regular character for run()
		if(connector.at(0) == '['){
			s.push_back(pair<string, bool>(connector, false));
		}
		else{s.push_back(pair<string, bool>(connector, true));}
		}
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

bool run_test(vector<pair<string,bool> > vec){
	bool defaultCase = false;
	bool returnBack = false;
	bool returnForward = false;

	if(vec.at(1).first == "/"){
		defaultCase = true;	
		pair <string,bool> m ("e", false);
		vec.insert(vec.begin()+1 , m);
	}
	//fileName is the file to be tested
	string fileName = vec.at(vec.size() - 1).first;
	//testCase is the test flag(e, f, d, default case is e)	
	string testCase = vec.at(1).first;
	/*cout << "testCase: " << testCase << endl;*/
	char* cwd;
    char buff[PATH_MAX + 1];
    cwd = getcwd( buff, PATH_MAX + 1 );
	string currDirString = cwd;
	/*cout << "currDir: " << currDirString << endl;*/

	//push current directory into a vector
	vector<pair<string,bool> > currDir;
	string x = "";
	string backSlash = "/";
	for(unsigned int i = 0; i < currDirString.length(); i++){
		if(currDirString.at(i) != '/'){
			x = x + currDirString.at(i);
			if(i == currDirString.length() - 1){
				pair <string,bool> y (x,0);	
				currDir.push_back(y);
			}
		}
		else{
			if(i > 0){
				pair <string,bool> y (x,0);	
				currDir.push_back(y);
			}
			pair <string,bool> j (backSlash, 0);
			currDir.push_back(j);
			x.clear();
		}
	}
	/*cout << "currDir vector contents: " << endl;
	for(int i = 0; i < currDir.size(); i++){
		cout << currDir.at(i).first;
	}
	cout << endl;*/

	//steps for run_test()
	//	1: change to desired directory
	//	2: check for file
	//	3: return to original directory

	//remove "test" and flag from vec
	vec.erase(vec.begin(), vec.begin()+2);
	vector<pair<string,bool> > desiredDir = vec;
	desiredDir.pop_back();	//remove fileName from desiredDir
	desiredDir.pop_back();	//remove "/" from desiredDir
/*	cout << "desiredDir: ";
	for(int i = 0; i < desiredDir.size(); i++){
		cout << desiredDir.at(i).first;
	}
	cout << endl;
*/
	//desired directory is given in vec
	//find the difference in directories
	vector<string> changeInDir;
	vector<string> returnDir;
	if(desiredDir.size() > currDir.size()){
		//if desired directory is x amount of directories
		//ahead of current directory
		returnBack = true;	
		//store difference in directories in changeInDir
		for(unsigned int i = currDir.size(); i < desiredDir.size(); i++){
			changeInDir.push_back(desiredDir.at(i).first);
		}
	/*	cout << "changeInDir: ";
		for(int i = 0; i < changeInDir.size(); i++){
			cout << changeInDir.at(i);
		}
		cout << endl;
*/
		//change directory to desired
		//working directory
		for(unsigned int i = 0; i < changeInDir.size(); i++){
			if(changeInDir.at(i) != "/"){
				returnDir.push_back(changeInDir.at(i));
				//convert string to const char
				unsigned lineSize = changeInDir.at(i).size() + 1;
				//char **argv = new char*[lineSize];
				char *c = new char[lineSize];
				copy(changeInDir.at(i).begin(), changeInDir.at(i).end(), c);
				c[changeInDir.at(i).size()] = '\0';
				//change directory using chdir()
				chdir(c);
			}
		}

		//perform test in new
		//working directory
		struct stat buffer;
		unsigned lineSize =	fileName.size() + 1;
		//char** argv = new char*[lineSize];
		char *c = new char[lineSize];
		copy(fileName.begin(), fileName.end(), c);
		c[fileName.size()] = '\0';
	
		if(defaultCase){
			
			if(stat(c, &buffer) == 0){
				cout << "(True)" << endl;
			}
			else{
				cout << "(False)" << endl;
			}
		}
		else if(testCase == "e"){
			if(stat(c, &buffer) == 0){
				cout << "(True)" << endl;
			}
			else{
				cout << "(False)" << endl;
			}
		}
		else if(testCase == "f"){
			struct stat path_stat;
			stat(c, &path_stat);
			if(S_ISREG(path_stat.st_mode)){
				cout << "(True)" << endl;
			}
			else{
				cout << "(False)" << endl;
			}
		}
		else if(testCase == "d"){
			struct stat path_stat;
			stat(c, &path_stat);
			if(S_ISDIR(path_stat.st_mode)){
				cout << "(True)" << endl;
			}
			else{
				cout << "(False)" << endl;
			}
		}
	}
	else if(desiredDir.size() == currDir.size()){
		struct stat buffer;
		unsigned lineSize =	fileName.size() + 1;
		//char** argv = new char*[lineSize];
		char *c = new char[lineSize];
		copy(fileName.begin(), fileName.end(), c);
		c[fileName.size()] = '\0';

		if(defaultCase){
			if(stat(c, &buffer) == 0){
				cout << "(True)" << endl;
			}
			else{
				cout << "(False)" << endl;
			}
		}
		else if(testCase == "e"){
			if(stat(c, &buffer) == 0){
				cout << "(True)" << endl;
			}
			else{
				cout << "(False)" << endl;
			}
		}
		else if(testCase == "f"){
			struct stat path_stat;
			stat(c, &path_stat);
			if(S_ISREG(path_stat.st_mode)){
				cout << "(True)" << endl;
			}
			else{
				cout << "(False)" << endl;
			}
		}
		else if(testCase == "d"){
			struct stat path_stat;
			stat(c, &path_stat);
			if(S_ISDIR(path_stat.st_mode)){
				cout << "(True)" << endl;
			}
			else{
				cout << "(False)" << endl;
			}
		}
	}
	else{
		//desired directory is x amount of directories
		//behind of current directory
		returnForward = true;
		//store difference in directories in changeInDir
		for(unsigned int i = desiredDir.size(); i < currDir.size(); i++){
			changeInDir.push_back(currDir.at(i).first);
		}
	/*	cout << "changeInDir: ";
		for(int i = 0; i < changeInDir.size(); i++){
			cout << changeInDir.at(i);
		}
		cout << endl;
	*/
		//change directory to desired
		//working directory
		for(int i = changeInDir.size() - 1; i > 0; i--){
			if(changeInDir.at(i) != "/"){
				returnDir.push_back(changeInDir.at(i));
				string u = "..";
				//convert string to const char
				unsigned lineSize = changeInDir.at(i).size() + 1;
				//char **argv = new char*[lineSize];
				char *c = new char[lineSize];
				copy(u.begin(), u.end(), c);
				c[u.size()] = '\0';
				//change directory using chdir()
				chdir(c);
			}
		}
		
	/*	cout << "directory after change in smaller direc case: ";
    	char buff3[PATH_MAX + 1];
    	cwd = getcwd( buff3, PATH_MAX + 1 );
		cout << cwd << endl;
	*/
		//run tests in new directory
		struct stat buffer;
		unsigned lineSize =	fileName.size() + 1;
		//char** argv = new char*[lineSize];
		char *c = new char[lineSize];
		copy(fileName.begin(), fileName.end(), c);
		c[fileName.size()] = '\0';
	
		if(defaultCase){
			if(stat(c, &buffer) == 0){
				cout << "(True)" << endl;
			}
			else{
				cout << "(False)" << endl;
			}
		}
		else if(testCase == "e"){
			if(stat(c, &buffer) == 0){
				cout << "(True)" << endl;
			}
			else{
				cout << "(False)" << endl;
			}
		}
		else if(testCase == "f"){
			struct stat path_stat;
			stat(c, &path_stat);
			if(S_ISREG(path_stat.st_mode)){
				cout << "(True)" << endl;
			}
			else{
				cout << "(False)" << endl;
			}
		}
		else if(testCase == "d"){
			struct stat path_stat;
			stat(c, &path_stat);
			if(S_ISDIR(path_stat.st_mode)){
				cout << "(True)" << endl;
			}
			else{
				cout << "(False)" << endl;
			}
		}
	}
	/*cout << "returnDir contents: ";
	for(int i = 0; i < returnDir.size(); i++){
		cout << returnDir.at(i) << " ";
	}
	cout << endl;*/
	
	if(returnBack){
		for(int i = returnDir.size(); i > 0; i--){
				string u = "..";
				//convert string to const char
				unsigned lineSize = changeInDir.at(i).size() + 1;
				//char **argv = new char*[lineSize];
				char *c = new char[lineSize];
				copy(u.begin(), u.end(), c);
				c[u.size()] = '\0';
				//change directory using chdir()
		//		cout << "c: " << c << " ";
				chdir(c);
		}
	//	cout << endl;
	}
	
	if(returnForward){
		for(int i = returnDir.size() - 1; i >= 0; i--){
			//convert string to const char
			unsigned lineSize = returnDir.at(i).size() + 1;
		//	char** argv = new char*[lineSize];
			char* c = new char[lineSize];
			copy(returnDir.at(i).begin(), returnDir.at(i).end(), c);
			c[returnDir.at(i).size()] = '\0';
			//change directory using chdir()
			chdir(c);
		}
	}
	
	/*cout << "directory at end of run_test(): ";
    char buff2[PATH_MAX + 1];
    cwd = getcwd( buff2, PATH_MAX + 1 );
	cout << cwd << endl;*/
	return true;
}

bool run_bracketTest(vector<pair<string,bool> > vec){
	bool closingBracket = true;
	int openingBrackets = 1;
	for(unsigned int i = 0; i < vec.size(); i++){
		if(vec.at(i).first == "]" && openingBrackets <= 0){
			closingBracket = false;
		}
		if(vec.at(i).first == "]" && openingBrackets > 0){
			openingBrackets--;
			vec.erase(vec.begin()+i);
			closingBracket = true;
		}
	}

	if(closingBracket == false){
		return false;
	}
	else{
		vec.at(0).first = "test";
		cout << "vec. contents: ";
		for(unsigned int i = 0; i < vec.size(); i++){
			cout << vec.at(i).first << " ";
		}
		cout << endl;
		bool success = run_test(vec);
		if(success){
			return true;
		}
	}
	return true;
}

void run(vector<pair<string, bool> > v) {
  bool prevCom = false; 							
  bool doIRun = true;

	vector<pair<string, bool> > Test;
	vector<pair<string, bool> > restOfCommand;

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
	//if the user inputs test, go to seperate case that doesn't use execvp
	if(strcmp(*argv, "test") == 0){
		//call a seperate function to complete the command
		//run_test(vector<pair<string,bool>> vec) is a function that will
		//run our own implemented version of the bash test command

		//in vector v, need to seperate test command:
		//test -e /home/csmajs/tchau006/rshell/rshell/src/main.cpp
		//from "&& echo A". execute test seperately from connector command
		bool orStatement = false;
		bool noConnectorYet = true;
		int connectorCounter = 0;
		for(unsigned int i = 0; i < v.size(); i++){
			if((v.at(i).first == "&&") || (v.at(i).first == "||") || (v.at(i).first == ";")){
				noConnectorYet = false;
			}
			if(noConnectorYet){
				Test.push_back(v.at(i));
			}
			if(!noConnectorYet){
				if((v.at(i).first == "&&") || (v.at(i).first == "||") || (v.at(i).first == ";")){
					if(v.at(i).first == "||"){
						orStatement = true;
					}
					connectorCounter++;
				}
				restOfCommand.push_back(v.at(i));	
			}
		}
/*		cout << "Test contents: ";
		for(unsigned int i = 0; i < Test.size(); i++){
			cout << Test.at(i).first << " ";
		}
		cout << endl;*/
		
		if(!restOfCommand.empty() && (connectorCounter > 0)){
			connectorCounter--;
			restOfCommand.erase(restOfCommand.begin());
		}
	/*	cout << "restOfCommand contents: ";
		for(unsigned int i = 0; i < restOfCommand.size(); i++){
			cout << restOfCommand.at(i).first << " ";
		}
		cout << endl;*/
		prevCom = run_test(Test);
		if(orStatement){
			doIRun = false;
			return;
		}
		else if(prevCom && !restOfCommand.empty()){
			doIRun = true;
			v = restOfCommand;
			if((v.at(0).first == "&&") || (v.at(0).first == "||") || (v.at(0).first == ";")){
				v.erase(v.begin());
			}
			run(v);
		}
		else{
			doIRun = false;
		}
	}
	if(strcmp(*argv, "[") == 0){
		//run_bracketTest(vector<pair<string,bool>> vec) is a function that will 
		//run our own implemented version of the bash test command
		bool closingBracket = true;
		bool orStatement = false;

		int openingBrackets = 1;
		for(unsigned int i = 0; i < v.size(); i++){
			if(v.at(i).first == "]" && openingBrackets <= 0){
				closingBracket = false;
			}
			if(v.at(i).first == "]" && openingBrackets > 0){
				openingBrackets--;
				v.erase(v.begin()+i);
				closingBracket = true;
			}
		}
		if(closingBracket == false){
			prevCom = false;
			cout << "error: missing closing bracket" << endl;
			return;
		}
		else{
			v.at(0).first = "test";
	/*		cout << "v. contents: ";
			for(int i = 0; i < v.size(); i++){
				cout << v.at(i).first << " ";
			}
			cout << endl;*/
			bool noConnectorYet = true;
			int connectorCounter = 0;
			for(unsigned int i = 0; i < v.size(); i++){
				if((v.at(i).first == "&&") || (v.at(i).first == "||") || (v.at(i).first == ";")){
					noConnectorYet = false;
				}
				if(noConnectorYet){
					Test.push_back(v.at(i));
				}
				if(!noConnectorYet){
					if((v.at(i).first == "&&") || (v.at(i).first == "||") || (v.at(i).first == ";")){
						if(v.at(i).first == "||"){
							orStatement = true;
						}
						connectorCounter++;
					}
					restOfCommand.push_back(v.at(i));	
				}
			}
	/*		cout << "Test contents: ";
			for(unsigned int i = 0; i < Test.size(); i++){
				cout << Test.at(i).first << " ";
			}
			cout << endl;*/
		
			if(!restOfCommand.empty() && (connectorCounter > 0)){
				connectorCounter--;
				restOfCommand.erase(restOfCommand.begin());
			}
		
	/*		cout << "restOfCommand contents: ";
			for(unsigned int i = 0; i < restOfCommand.size(); i++){
				cout << restOfCommand.at(i).first << " ";
			}
			cout << endl;*/
			prevCom = run_test(Test);
			if(orStatement){
				doIRun = false;
				return;
			}
			if(prevCom && !restOfCommand.empty()){
				doIRun = true;
				v = restOfCommand;
				if((v.at(0).first == "&&") || (v.at(0).first == "||") || (v.at(0).first == ";")){
					v.erase(v.begin());
				}
				run(v);
			}
			else{
				doIRun = false;
			}
		}
	}
//----------------------------------------2/24/2016
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

