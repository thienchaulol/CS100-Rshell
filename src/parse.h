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

void finalParse(char *line, char **argv) {						//final parse function. called in run()
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

bool execute(char **argv){				//function to execute commands from user
  pid_t pid;
  int status;
  bool succeeded = true; 				//boolean value to check if function was able to execute

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
  return succeeded;					//execute() will return true if function was able to execute. else false.
									//execute() called in run() function later on.
}

vector<pair<char, bool> > hashAndSlash(string str) {					//hash and slash function used to "hash and slash" user input
  vector<pair<char, bool> > v;											//vector v will be made of pairs of characters and bools
  bool isInWord = false;												//isInWord used to determine if character is a part of "word"---argument/command
  bool isAfterBackslash = false;
  bool allQuotesFound = true;										//isAfterBackslash to determine if character is "special"---after a backslash
  bool isNormalChar = false;

  for(unsigned i = 0; i < str.size(); i++) {					//for loop to iterate through user input
    if(str.at(i) == ' ') {										//whitespace not part of commands
      isInWord = false;												//set isInWord to false
      v.push_back(pair<char, bool>(str.at(i), isNormalChar));		//pushback character into vector v, store character with isNormalChar set to false
    }
    else {
      if(str.at(i) == '#' && !isInWord && !isNormalChar)		//if character "#" symbol, isInWord is true, and isNormalChar is true, break out of hashAndSlash
	break;
      else if(str.at(i) == '\\' && (!isAfterBackslash || !isNormalChar)) { //if character is "\\", and is after a back slash, or a normal character
	if(i + 1 <= str.size() - 1) {											//if character index + 1 is less than or equal to length of string - 1. (is not last or second to last character)
	  i++;																		//increment index value
	  v.push_back(pair<char, bool>(str.at(i), true));							//push back index value and bool val true
	}
      }
      else if(str.at(i) == '"') {		//dealing with Quotes. if character is a quotation mark
	if(allQuotesFound == true) { 			//if all quotes are found
	  allQuotesFound = false;					//set allQuotesFound to false
	  isNormalChar = true;
	  v.push_back(pair<char, bool>(str.at(i), false));
	  						//set isNormalChar to true
	}
	else if(!allQuotesFound) {				//if not all quotes are found
	  allQuotesFound = true;					//set all quotes found to true
	  isNormalChar = false;
	  v.push_back(pair<char, bool>(str.at(i), false));						//set is normal character to false
	}									//Quotes dealt with.
      }
      else
	v.push_back(pair<char, bool>(str.at(i), isNormalChar));
      isInWord = true;
    }
  }

  while(!allQuotesFound) {				//while not all quotes are found
    string line;
    
    cout << "> ";						//re-prompty user for input
    getline(cin, line);					//get input from user 

    for(unsigned i = 0; i < line.size(); i++) {										// repeat beginning functionality.........
      if(line.at(i) == ' ') {
	isInWord = false;
	v.push_back(pair<char, bool>(line.at(i), isNormalChar));
      }
      else {
	if(line.at(i) == '#' && !isInWord)
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
	    v.push_back(pair<char, bool>(str.at(i), false));
	  }
	  else if(!allQuotesFound) {
	    allQuotesFound = true;
	    isNormalChar = false;
	    v.push_back(pair<char, bool>(str.at(i), false));
	  }
	}
	else
	  v.push_back(pair<char, bool>(line.at(i), isNormalChar));
	isInWord = true;
      }
    } 
  }
  return v;				//return vector v, vector v will be composed of pairs of characters and bools
}

vector<pair<string, bool> > parse(vector<pair<char, bool> > v) {		//parse function takes in specVec which is a vector of characters and bools created from user input
  vector<pair<string, bool> > s;										//vector s will be made of strings and booleans. will eventually store individual arguments/commands

  while(!v.empty()){
													//while vector of characters (specVec), v, is not empty do......
    string command;															//command string to hold argument(s)/command(s)
    string connector;														//connector string to hold connector(s)
    
    while(v.at(0).first == ' ')											//if specVec's first character is whitespace....
      v.erase(v.begin());													//erase whitespace until you get to the "good stuff" (argument/command or connectors)

    unsigned i = 0;
    for(i = 0; i < v.size(); i++){										//for loop to iterate through specVec and check for single connectors
      if(v.at(i).first == '&' && !v.at(i).second)					//if first character is connector: & and bool value is false---break out of while loop
	break;
      else if(v.at(i).first == '|' && !v.at(i).second)				//if first character is connector: | and bool value is false---break out of while loop
	break;
      else if(v.at(i).first == ';' && !v.at(i).second)				//if first character is connector: ; and bool value is false---break out of while loop
	break;
    }

    for(unsigned j = 0; j < i; j++) {										//for loop to iterate through vector again, this time using i as v.size	
      if(v.at(0).first == '\\' && !v.at(0).second)					//if first character is: '\\' and bool value is false.....
	v.erase(v.begin());
      else if(v.at(0).first == '\"' && !v.at(0).second) {
	command += v.at(0).first;
	v.erase(v.begin());
      }															//erase the first value
      else {																//if first character is not: '\\' and bool value is true......
	command += v.at(0).first;													//"increment"(add) character to command string. that is, build the command
	v.erase(v.begin());															//erase original command, character by character
      }
    }

    if(!v.empty()) {													//if new vector v is not empty
      if(v.at(0).first == '&' || v.at(0).first == '|') {					//if first character is: & or |
	connector += v.at(0).first;													//"increment"(add) character to connector string. that is, build the connector
	v.erase(v.begin());															//erase original connector, character by character
      }
      if(!v.empty()) {
	if(v.at(0).first == '&' || v.at(0).first == '|') {				//same as lines above because connectors are: "&&" and "||"
	  connector += v.at(0).first;
	  v.erase(v.begin());
	}
	else if(v.at(0).first == ';') {										//if first character is: ;
	  connector += v.at(0).first;													//"increment"(add) character to connector string.....
	  v.erase(v.begin());															//erase original connector......
	}
      }
    }

    if(!command.empty()) {
      while(command.at(command.size() - 1) == ' '
	    || command.at(command.size() - 1) == '\t'
	    || command.at(command.size() - 1) == '\n') 
	command.erase(command.size() - 1, 1);												//if command is not empty(means we were able to retrieve commands from specVec which is userinput), do....
      s.push_back(pair<string, bool>(command, false));
    }						//push back command to vector s, s will contain pairs of strings and bools
    if(!connector.empty())												//if connector is not empty(able to retrieve connectors)
      s.push_back(pair<string, bool>(connector, true));						//push back connector to vector s, s will contain pairs of strings and bools
  }

  while((s.at(s.size() - 1).second && s.at(s.size() - 1).first == "&&")
	|| (s.at(s.size() - 1).second && s.at(s.size() - 1).first == "||")
	|| (s.at(s.size() - 1).second && s.at(s.size() - 1).first == "|")) {	//starting from the last element of s, while the boolean values for vector s are true, prompt user for extra input
    string line;
    
    cout << "> ";
    getline(cin, line);													//get more input from user as long as there is a connector at end of input

    vector<pair<char, bool> > v2 = hashAndSlash(line);					//call hash and slash on extra user input, store into vector v2

    while(!v2.empty()){													//while vector v2 is not empty do same stuff at beginning of parse function.......
      string command;														//...............................
      string connector;														//...............................
    
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

  return s;					//return vector s, s contains vector of pairs of strings and bools. the strings are the individual arguments from user input
								//parse function was able to seperate individual user input through a series of if and else statements and store user commands as strings in s.
}

void run(vector<pair<string, bool> > v) {		//vector v will be parsedVec.
  bool prevCom = true; 							//prevCom = previous command
  bool doIRun = true;							//boolean value to determine whether or not to run the next command
  
  if(v.at(0).second == 1) {
    cout << "syntax error near unexpected token `" << v.at(0).first << "'" << endl;
    return;
  }
  while(!v.empty()) {							//execute while loop while vector v is not empty
    if(v.at(0).second == 0 && doIRun) {		//if doIRun is true...
      unsigned lineSize = v.at(0).first.size() + 1;		//
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
	prevCom = execute(argv);			//prevCom true if execute succeeded. prevCom false if execute failed.
	v.erase(v.begin());					//erase beginning of vector, in order to move to next value
      }
    }  
    else if(v.at(0).second == 0 && !doIRun) //if doIRun is false
      v.erase(v.begin());					//erase first value of vector
    if(!v.empty()) {					//if values are present in v 
      if(v.at(0).first == "&&") {			//if present value is connector: &&
	if(prevCom)									//if prevCom was successful
	  doIRun = true;							//set doIRun to true
	else
	  doIRun = false;							//else set doIRun to false
      } 
      else if(v.at(0).first == "||" || v.at(0).first == "|") {	//if present value is connector: || or |
	if(prevCom)														//if prevcom was successful
	  doIRun = false;													//set doIRun to true
	else
	  doIRun = true;													//else set doIRun to false
      }
      else if(v.at(0).first == "&") {												//if present value is connector: &
	cout << "syntax error near unexpected token '" << v.at(0).first << "'" << endl;		//return from run
	return;
      }
      else if(v.at(0).first == ";") 			//if present value is connector: ;
	doIRun = true;									//set doIRun to true
      v.erase(v.begin());							//erase beginning of vector
    }
  }
}
