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

bool execute(char **argv) {
  pid_t pid;
  int status;
  bool succeeded = true; 

  if((pid = fork()) < 0) {
    cout << "Error: Forking child process failed" << endl;
    exit(1);
  }
  else if(pid == 0) {
    if(execvp(*argv, argv) < 0) {
      succeeded = false;      
      cout << "Error: exec failed" << endl;
      exit(1);
    }
  }
  else {
    while(wait(&status) != pid) {
    }
  }
  return succeeded;
}

vector<pair<char, bool> > hashAndSlash(string str) {					//hash and slash function used to "hash and slash" user input
  vector<pair<char, bool> > v;											//vector v will be made of pairs of characters and bools
  bool isInWord = false;												//isInWord used to determine if character is a part of "word"---argument/command
  bool isAfterBackslash = false;										//isAfterBackslash to determine if character is "special"---after a backslash

  for(unsigned i = 0; i < str.size(); i++) {									//for loop to iterate through user input
    if(str.at(i) == ' ') {												//if whitespace found, character cannot be part of argument/command
      isInWord = false;														//so set isInWord to false
      isAfterBackslash = false;												//and set isAfterBackslash to false
    }
    else if(str.at(i) != ' ' && str.at(i) != '#') {						//if no whitespace found and no "#" symbol found, character is part of argument/command
      isInWord = true;														//set isInWord to true
      if(str.at(i) == '\\' && !isAfterBackslash) {							//if character is "\\" and is not after a backslash
	v.push_back(pair<char, bool>(str.at(i), isAfterBackslash));				//push character and bool value to back of vector v
	isAfterBackslash = true;												//set isAfterBackslash to true
	if(i + 1 <= str.size() - 1)											//if before second to last character
	  i++;																	//increment i
      }
    }
    else if(str.at(i) == '#' && !isInWord)								//if character is "#" symbol and is not in word
      break;																//break out of the function because argument/command is actually a comment
    v.push_back(pair<char, bool>(str.at(i), isAfterBackslash));			//push back character and bool(isAfterBackslash) to v
    isAfterBackslash = false;											//set isAfterBackslash to false
  }
  return v;																//return vector v, v is composed of argument/command characters and isAfterBackslash bool values
}

vector<pair<string, bool> > parse(vector<pair<char, bool> > v) {		//parse function takes in specVec which is a vector of characters and bools created from user input
  vector<pair<string, bool> > s;										//vector s will be made of strings and booleans. will eventually store individual arguments/commands
  
  while(!v.empty()){													//while vector of characters (specVec), v, is not empty do......
    string command;															//command string to hold argument(s)/command(s)
    string connector;														//connector string to hold connector(s)
    
    while(v.at(0).first == ' ')											//if specVec's first character is whitespace....
      v.erase(v.begin());													//erase whitespace until you get to the "good stuff" (argument/command or connectors)

    unsigned i = 0;
    for(i = 0; i < v.size(); i++){										//for loop to iterate through specVec and check for single connectors
      if(v.at(i).first == '&' && v.at(i).second == false)					//if first character is connector: & and bool value is false---break out of while loop
	break;
      else if(v.at(i).first == '|' && v.at(i).second == false)				//if first character is connector: | and bool value is false---break out of while loop
	break;
      else if(v.at(i).first == ';' && v.at(i).second == false)				//if first character is connector: ; and bool value is false---break out of while loop
	break;
    }

    for(unsigned j = 0; j < i; j++) {										//for loop to iterate through vector again, this time using i as v.size	
      if(v.at(0).first == '\\' && v.at(0).second == false)					//if first character is: '\\' and bool value is false.....
	v.erase(v.begin());															//erase the first value
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

    if(!command.empty())												//if command is not empty(means we were able to retrieve commands from specVec which is userinput), do....
      s.push_back(pair<string, bool>(command, false));						//push back command to vector s, s will contain.........
    if(!connector.empty())												//if connector is not empty(able to retrieve connectors)
      s.push_back(pair<string, bool>(connector, true));						//push back connector to vector s, s will contain........
  }

  while((s.at(s.size() - 1).second == true && s.at(s.size() - 1).first == "&&")
	|| (s.at(s.size() - 1).second == true && s.at(s.size() - 1).first == "||")
	|| (s.at(s.size() - 1).second == true && s.at(s.size() - 1).first == "|")) {							//starting from the last element of s, while the boolean values for vector s are true, prompt user for extra input
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
	if(v2.at(i).first == '&' && v2.at(i).second == false)
	  break;
	else if(v2.at(i).first == '|' && v2.at(i).second == false)
	  break;
	else if(v2.at(i).first == ';' && v2.at(i).second == false)
	  break;
      }

      for(unsigned j = 0; j < i; j++) {
	if(v2.at(0).first == '\\' && v2.at(0).second == false)
	  v2.erase(v2.begin());
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
      
      if(!command.empty()) 
	s.push_back(pair<string, bool>(command, false));
      if(!connector.empty())
	s.push_back(pair<string, bool>(connector, true));      
    }
  }

  return s;					//return vector s, s contains vector of pairs of strings and bools. the strings are the individual arguments from user input
								//parse function was able to seperate individual user input through a series of if and else statements and store user commands as strings in s.
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
      if(strcmp(argv[0], "exit") == 0) {
	exit(0);
      }
      prevCom = execute(argv);
      v.erase(v.begin());
    }  
    else if(v.at(0).second == 0 && !doIRun) 
      v.erase(v.begin());
    if(!v.empty()) {
      if(v.at(0).first == "&&") {
	if(prevCom == true)
	  doIRun = true;
	else
	  doIRun = false;
      } 
      else if(v.at(0).first == "||" || v.at(0).first == "|") {
	if(prevCom == true)
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
  }
}

int main() {

  while(1) {			//begin program
    string line;

    cout << "$ ";		//$ will be the first character of the command line

    getline(cin, line);	//take in user input via command line

    //unsigned lineSize = line.size() + 1;
    //char **argv = new char*[lineSize];

    vector<pair<char, bool> > specVec = hashAndSlash(line);			//specVec will be a vector of pairs of characters and bools composed by the hashAndSlash function
																	//hashAndSlash will return vector of characters and bools of userinput, facilitating the process of denoting arguments/commands
    cout << "specVec.first and specVec.second:" << endl;			
    for(unsigned i = 0; i < specVec.size(); i++)							//for loop to output characters of commands from user input processed from hashAndSlash
      cout << specVec.at(i).first;
    cout << endl;
    for(unsigned i = 0; i < specVec.size(); i++)							//for loop to output bool values of isAfterBackslash
      cout<< specVec.at(i).second;
    cout << endl << endl;

    vector<pair<string, bool> > parsedVec = parse(specVec);			//parsedVec will be a vector of pairs of strings and bools composed of parse function
																	//parse function should split up specVec into seperate strings. these strings being the user commands split up from connectors
    cout << "parsedVec.second and parsedVec.first:" << endl;
    for(unsigned i = 0; i < parsedVec.size(); i++)									//for loop to output boolean values and string values of newly "parsed" user input
      cout << parsedVec.at(i).second << ' ' << parsedVec.at(i).first << endl;
    cout << endl;

    cout << "output:" << endl;
    
    run(parsedVec);

    //char *c = new char[line.size() + 1];						//series of lines used to execute user commands
    //copy(line.begin(), line.end(), c);								
    //c[line.size()] = '\0';
    
    //finalParse(c, argv);
    //if(strcmp(argv[0], "exit") == 0) {
      //exit(0);
      //}
    //execute(argv);
  }
  return 0;
}
