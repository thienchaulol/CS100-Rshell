#include <string>
#include <vector>
#include <iostream>
#include <sys/types.h>
#include <pwd.h>

#include "parse.h"

int main() {

  while(1) {		
    string line;

    struct passwd *passwd;
    passwd = getpwuid(getuid());
    char buf[32];					
    gethostname(buf, sizeof buf);					
    cout << passwd->pw_name << '@' << buf << " $ ";	

    getline(cin, line);

    vector<pair<char, bool> > specVec = hashAndSlash(line);		
	//why is git so difficult!

    //cout << "specVec.first and specVec.second:" << endl;			
    //for(unsigned i = 0; i < specVec.size(); i++)						
	//cout << specVec.at(i).first;
    //cout << endl;
    //for(unsigned i = 0; i < specVec.size(); i++)							
	//cout<< specVec.at(i).second;
    //cout << endl << endl;
    if(!specVec.empty()) {
      vector<pair<string, bool> > parsedVec = parse(specVec);
      //cout << "parsedVec.second and parsedVec.first:" << endl;
      //for(unsigned i = 0; i < parsedVec.size(); i++)									
      //cout << parsedVec.at(i).second << ' ' << parsedVec.at(i).first.size() << ' ' << parsedVec.at(i).first << endl;
      //cout << endl;

      //cout << "output:" << endl;
    
      run(parsedVec);											
    }
    
    //char *c = new char[line.size() + 1];						
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
