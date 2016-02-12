#include <string>
#include <vector>
#include <iostream>
#include <sys/types.h>
#include <pwd.h>

#include "parse.h"

int main()
{

  while(1) {			//begin program
    string line;

    struct passwd *passwd;
    passwd = getpwuid(getuid());
    char buf[32];								//extra credit portion of assignment
    gethostname(buf, sizeof buf);						//returns null-terminated hostname in the character array buf
    cout << passwd->pw_name << '@' << buf << " $ ";		//$ will be the first character of the command line

    getline(cin, line);	//take in user input via command line

    vector<pair<char, bool> > specVec = hashAndSlash(line);			//specVec will be a vector of pairs of characters and bools composed by the hashAndSlash function
																	//hashAndSlash will return vector of characters and bools of userinput, facilitating the process of denoting arguments/commands
    //cout << "specVec.first and specVec.second:" << endl;			
    //for(unsigned i = 0; i < specVec.size(); i++)							//for loop to output characters of commands from user input processed from hashAndSlash
	//cout << specVec.at(i).first;
    //cout << endl;
    //for(unsigned i = 0; i < specVec.size(); i++)							//for loop to output bool values of isAfterBackslash
	//cout<< specVec.at(i).second;
    //cout << endl << endl;
    if(!specVec.empty()) {
      vector<pair<string, bool> > parsedVec = parse(specVec);			//parsedVec will be a vector of pairs of strings and bools composed of parse function
																	//parse function should split up specVec into seperate strings. these strings being the user commands split up from connectors
    //cout << "parsedVec.second and parsedVec.first:" << endl;
    //for(unsigned i = 0; i < parsedVec.size(); i++)									//for loop to output boolean values and string values of newly "parsed" user input
	//cout << parsedVec.at(i).second << ' ' << parsedVec.at(i).first << endl;
    //cout << endl;

    //cout << "output:" << endl;
    
      run(parsedVec);											//call run function of parsedVec.
    }

    char *c = new char[line.size() + 1];						//series of lines used to execute user commands
    copy(line.begin(), line.end(), c);								
    c[line.size()] = '\0';
    
    //finalParse(c, argv);
    //if(strcmp(argv[0], "exit") == 0) {
     // exit(0);
      //}
    //execute(argv);
  }
  return 0;
}
