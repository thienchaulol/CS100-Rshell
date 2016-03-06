Thien Chau's and Christopher Pribyl's attempt at rshell. 
Our program will take user input and replicate the terminal on UCR's hammer server. 
The program is still in it's early stages so it may not handle all possible commands. 
A brief description of what our program does is that it takes user input, distinguishes between commands(ls, echo, cal, etc.), connectors(&&, ||, ;), and special symbols(#, \). 
Our program utilizes boolean values and vectors to store characters from the user input and organize these characters so that the program knows what to output and when. 
Here's a breakdown of the program's functions and a list of known bugs.

**Functionality:**

**Assignment 2 Functionality**

Rshell will run in a while loop. The while loop will continue to run while true.
The program will replicate the command line of a terminal, simulating the user's username via a `char buf`, and the host via the function `gethostname()`. 
The user's input is stored using the function `getline()` and stored as a string. 
A vector of pairs of characters and bool values named *specVec* is created using our *hashAndSlash()* function which takes in a string, 
the user input will be passed into this function. 
The *hashAndSlash()* function will return a vector of characters and bools. 
Each character will be paired up with a boolean value to determine whether or not the character is in the word, is after a backslash, or is a normal character. 
*hashAndSlash()* also deals with quotes from the user input by keeping track of the boolean value `allQuotesFound`. 
`allQuotesFound` is initially set to true and is set to false when the character '"' appears in the string. 
If the string's end has been reached while `allQuotesFound` is false, the user is prompted to enter more input.
 When *hashAndSlash()* is completed, *specVec* will be a vector of pairs of characters and bools. 
*specVec* will then be passed into the function *parse()*. *parse()* will return *parsedVec*, a vector of strings and bools. 
*parse()* will break down it's passed in vector of characters and booleans and distinguish command characters from connector characters, 
storing each in it's own seperate string. 
Once the characters are seperated into strings, the new parsed vector is put into the function *run()*. 
There are two boolean values in *run()* that determine whether or not to run the next command based on the result of the last command. 
All commands are put into the function *execute()*. 
*execute()* makes use of child and parent processes and returns a boolean value *suceeded* equal to true if the process(command) was a success, else false. 
There are multiple checks in the *run()* to deal with incorrect input such as a single ampersand.

**Assignment 3 Functionality**
To handle the "test" command, my partner and I put a check our *run()* function to check for the keyword "test" in a
 similar fashion to how we check for the keyword "exit" and "cd" for assignment 2. 
If the keyword "test" is found, the user's input is split up into two vectors of pairs of strings and booleans. 
The two vectors being: *Test* and *restOfCommand*. 
The Test vector will hold the portion of the user's input which contains the test command and 
the restOfCommand vector will hold the portion of the user's input which contains the other commands to be executed. 
Once we have these two vectors, we execute the Test portion via our function *runTest()* and the restOfCommand portion via *execvp()*.
Connector functionality was implemented through a series of checks to see if a connector exists, what type of connector it is and whether
or not to execute restOfCommand based on that information.
There are multiple possible bugs that can throw our implementation of *runTest()* off.

**Known Bugs:**

**-------More bugs commented in test cases**

Input: *test -f /home/csmajs/tchau006/rshell/rshell/src/test/corn.cpp*
Experimental Rshell Output:
**(True)**
Desired output:
(False)

Input: *[ -e /home/csmajs/tchau006/rshell/rshell/src/main.cpp ] && echo "path exists" && echo "path exists"*
Experimental Rshell Output:
**(True)
path exists
path exists
path exists**
Desired output:
(True)
path exists
path exists

Input: *echo (A) && echo C*
Experimental Rshell Output: A: No such file or directory
Desired Output: -bash: syntax error near unexpected token `A'

Input: *(echo A) && || echo (B)*
Experimental Rshell Output:
A
Desired Output: -bash: syntax error near unexpected token `||'

Input: *(echo A) || echo (B)*
Experimental Rshell Output: A
Desired Output: -bash syntax error near unexpected token `B'

Input: *echo A && <ENTER> "A"*
Experimental Rshell Output: **No such file or directory**
Actual output: -bash: A: command not found


Input: *ls && echo A*
Experimental Rshell Output: **ls: cannot access : No such file or directory**
Actual output: [files]
				A
			"ls " a call to ls with any whitespace afterwards will lead to unexpected output


Input: *echo A && #hi*
Experimental Rshell Output: **crash**
Actual output: prompts user for more input " > "

Input: *"echo A && echo B"*
Experimental Rshell Output: **A && echo B**
Actual output: -bash: echo A && echo B: command not found
