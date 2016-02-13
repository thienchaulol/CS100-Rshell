Thien Chau's and Christopher Pribyl's attempt at rshell. 
Our program will take user input and replicate the terminal on UCR's hammer server. 
The program is still in it's early stages so it may not handle all possible commands. 
A brief description of what our program does is that it takes user input, distinguishes between commands(ls, echo, cal, etc.), connectors(&&, ||, ;), and special symbols(#, \). 
Our program utilizes boolean values and vectors to store characters from the user input and organize these characters so that the program knows what to output and when. 
Here's a breakdown of the program's functions and a list of known bugs.

**Functionality:**
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


**Known Bugs:**

**----More bugs commented in test cases**

Input: *echo A && <ENTER> "A"*
Experimental Rshell Output: **No such file or directory**
Actual output: -bash: A: command not found


Input: *ls && echo A*
Experimental Rshell Output: **ls: cannot access : No such file or directory
Actual output: [files]**
				A
			"ls " a call to ls with any whitespace afterwards will lead to unexpected output


Input: *echo A && #hi*
Experimental Rshell Output: **crash**
Actual output: prompts user for more input " > "

Input: *"echo A && echo B"*
Experimental Rshell Output: **A && echo B**
Actual output: -bash: echo A && echo B: command not found
