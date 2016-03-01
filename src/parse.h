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
			//connector += v.at(0).first;
			//erase '-'
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

//----------------------------------------2/24/2016
bool run_test(vector<pair<string,bool> > vec){
	//the user's parsed input will be passed in
	//if it begins with "test"
	bool isADirectory = false;
	bool isARegularFile = false;
	bool exists = false;	
	bool nonDefaultCase = false;
	//*******IMPORTANT:implement check to see if user entered "test" in the portion "/test/file/path"

	//implement change directory code to change directories
	//if it is not the default case, directory's locatioin will be
	//specified from vec.at(7).first to vec.end()
	//store that in a string

			//*******ONLY WORKS FOR NON-DEFAULT CASE: MUST BE IN FORMAT: "test -e /test/main/path/path1/path2/...."
	//desired path stored in pathVec. const char* in order to use chdir later

	vector<const char*> pathVec;
	for(int i = 7; i < vec.size(); i++){

		unsigned lineSize = vec.at(i).first.size() + 1;
		char **argv = new char*[lineSize];
		char *c = new char[lineSize];
		copy(vec.at(i).first.begin(), vec.at(i).first.end(), c);
		c[vec.at(i).first.size()] = '\0';

		//cout << "Path being pushed back: " << c << endl;
		pathVec.push_back(c);
	}
	
	//output current directory	
	char* cwd;
    char buff[PATH_MAX + 1];
	//getcwd() returns a pointer to a string
	//containing the path name of the current working directory
    cwd = getcwd( buff, PATH_MAX + 1 );

	int initWorkDirecSize = strlen(cwd);
//	cout << "Initial directory char* array size: " << initWorkDirecSize << endl;

	char* initWorkDirec = cwd;
//	if( cwd != NULL ) {
//		cout  << "The initial working directory is : " << initWorkDirec << endl;
//	}
	
	//change to desired directory via a for loop
	//once done with check, change back to initial directory
	//increment by two to avoid "/"
	for(int i = 0; i < pathVec.size(); i = i + 2){
		//cout << "Paths: " << pathVec.at(i) << endl;
		chdir(pathVec.at(i));
	}
	char buff2[PATH_MAX + 1];
	cwd = getcwd( buff2, PATH_MAX + 1 );
//	if( cwd != NULL ) {
//		cout  << "The desired working directory is : " << cwd << endl;
//	}
	int finWorkDirecSize = strlen(cwd);
//*****************-----------------------------11:23PM 2/27----------------*********************
	//code to return to initial directory
	//initial working directory stored in initWorkDirec
	//and then chdir() in back to the initial working directory

	//number of characters to copy from source
	//starting from offset
	int numCharToCopy = (initWorkDirecSize - strlen(cwd)) - 1;
	//returnDir will have (numCharToCopy) characters + 1 for null char '\0'
//cout << "strlen(initWorkDirec): " << initWorkDirecSize << endl;
//cout << "strlen(cwd): " << strlen(cwd) << endl;
//cout << "numCharToCopy: " << numCharToCopy << endl;	

	int currWorkDirecSize = strlen(cwd);
	//offset is strlen(cwd) + 1 because want
	//to copy everything after cwd/(.........)
	int offset = strlen(cwd) + 1;
//cout << "offset: " << offset << endl;

	unsigned Size = initWorkDirecSize - (currWorkDirecSize + 2);
	char* returnDir = new char[Size];
	/*declaring i outside of for loop and inside prevents seg fault*/
	unsigned int i = 0;
	for(unsigned int i = 0; i < (initWorkDirecSize - currWorkDirecSize) - 1; i++){
		returnDir[i] = initWorkDirec[offset + i];
	}
	returnDir[Size + 1] = '\0';
//cout << "Path to return to initial directory: " << returnDir <<endl;
				//*******ONLY WORKS FOR NON-DEFAULT CASE: MUST BE IN FORMAT: "test -e /test/main/path/path1/path2/...."
	
//*****************-----------------------------11:23PM 2/27----------------*********************
//	return;
	
	//***********default case*************
	if(vec.size() == 7 && vec.at(3).first == "test"){
		string x = vec.at(4).first;
		//cout << "DEFAULT VALUE FIRST: " << vec.at(0).first << endl;
		struct stat buffer;
		//yanked from other run()
		//need to convert string to c_str for reasons unknown
		unsigned lineSize = vec.at(4).first.size() + 1;
		char **argv = new char*[lineSize];

		char *c = new char[lineSize];
		copy(vec.at(4).first.begin(), vec.at(4).first.end(), c);
		c[vec.at(4).first.size()] = '\0';
		
		//use stat() to check if file exists, adapted from sources
		if(stat (c, &buffer) == 0){
			cout << "(True)" << endl;
			exists = true;
		}
		else{
			cout << "(False)" << endl;
		}
		return true;
	}
	
//3:25 PM "-f"
	if(vec.at(1).first == "f" && vec.at(3).first == "test"){
		//f checks if file/directory exists AND is a "regular" file
		//REGULAR FILE MEANING : NOT A DIRECTORY OR A "PIPE". couldn't implement pipe check
		//d checks if file/directory exists AND is a directory
		struct stat buffer;
		//yanked from other run()
		//need to convert string to c_str for reasons unknown
		unsigned lineSize = vec.at(5).first.size() + 1;
		char **argv = new char*[lineSize];
      
		char *c = new char[lineSize];
		copy(vec.at(5).first.begin(), vec.at(5).first.end(), c);
		c[vec.at(5).first.size()] = '\0';

		//checking if file is a directory, if it isn't then it's a regular file
		//fails to check if file is a "pipe". got errors using:
		//				S_ISREG(c.st_mode);
		//		to check whether is a file is a regular file
		//		S_ISREG was not declared, but I included the correct libraries..idk
		if(stat(c, &buffer) != 0){
			cout << "cannot access" << endl;
		}
		else if(buffer.st_mode & S_IFDIR){
			isADirectory = true;
		}
		else if(stat (c, &buffer) == 0){
			exists = true;
		}
		else{
			isADirectory = false;
		}
		if(!isADirectory && exists){
			cout << "(True)" << endl;
		}
		else{
			cout << "(False)" << endl;
		}

	}
	else if(vec.at(1).first == "d" && vec.at(3).first == "test"){
		//d checks if file/directory exists AND is a directory
		struct stat buffer;
		//yanked from other run()
		//need to convert string to c_str for reasons unknown
		unsigned lineSize = vec.at(5).first.size() + 1;
		char **argv = new char*[lineSize];
      
		char *c = new char[lineSize];
 		copy(vec.at(5).first.begin(), vec.at(5).first.end(), c);
		c[vec.at(5).first.size()] = '\0';
		
		if(stat(c, &buffer) != 0){
			cout << "cannot access" << endl;
		}
		if(buffer.st_mode & S_IFDIR){
			cout << "(True)" << endl;
			isADirectory = true;
		}
		else{
			cout << "(False)" << endl;
		}
	}

	else if(vec.at(1).first == "e" && vec.at(3).first == "test"){
		//run existential case
		struct stat buffer;
		//yanked from other run()
		//need to convert string to c_str for reasons unknown
      	unsigned lineSize = vec.at(5).first.size() + 1;
      	char **argv = new char*[lineSize];
      
     	char *c = new char[lineSize];
	    copy(vec.at(5).first.begin(), vec.at(5).first.end(), c);
     	c[vec.at(5).first.size()] = '\0';
		
		//use stat() to check if file exists, adapted from sources
		if(stat (c, &buffer) == 0){
			cout << "(True)" << endl;
			exists = true;
		}
		else{
			cout << "(False)" << endl;
		}
	}
/*add this snippet of code to the end of runTest to return to initial directory*/
		/*12:24 PM 2/28*/
			/*snippet only currently works for a single change in directory*/
				/*if path is set to "/../.." a nasty pointer error pops up*/
				/*the error is: munmap_chunk()..... */
			/*--->the initial directory is restored but program crashes<---*/
		/*1:05 PM 2/28*/
			/*snippet works for multiple changes in directory*/
				/*had to allocate memory for returnDir with "new char[Size]*/
	chdir(returnDir);

	char buff3[PATH_MAX + 1];
	cwd = getcwd( buff3, PATH_MAX + 1);
//	cout << "Final directory char* array size: " << strlen(cwd) << endl;
//cout << "char* array at [34] , [35]: " << cwd[34] << " " << cwd[35] << endl;
//	if( cwd != NULL ) {
//		cout  << "The final working directory is : " << cwd << endl;
///	}
/*add this snippet of code to the end of runTest to return to initial directory*/
	return true;
}

bool run_bracketTest(vector<pair<string,bool> > vec){
	int j = vec.size();
	if(vec.at(j - 1).first != "]"){
		return false;
	}
	else{
		vec.pop_back();	
		vec.at(0).first = "test";
		bool success = run_test(vec);
		if(success){
			return true;
		}
	}
}
//----------------------------------------2/24/2016

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
//----------------------------------------2/24/2016
	//if the user inputs test, go to seperate case that doesn't use execvp
	if(strcmp(*argv, "test") == 0){
		//call a seperate function to complete the command
		//run_test(vector<pair<string,bool>> vec) is a function that will
		//run our own implemented version of the bash test command
		run_test(v);
		return;
	}
			/*bracket case isn't detected because "[" is treated as a connector */
			/* and this entire if statement is entered only if: (!v.at(0).second && doIRun) */
	if(strcmp(*argv, "[") == 0){
		//run_bracketTest(vector<pair<string,bool>> vec) is a function that will 
		//run our own implemented version of the bash test command
		run_bracketTest(v);
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

