Thien Chau's and Christopher Pribyl's attempt at rshell. Our program will take user input and replicate the terminal on UCR's hammer server. The program is still in it's early stages so it may not handle all possible commands. A brief description of what our program does is that it takes user input, distinguishes between commands(ls, echo, cal, etc.), connectors(&&, ||, ;), and special symbols(#, \). Our program utilizes boolean values and vectors to store characters from the user input and organize these characters so that the program knows what to output and when. Here's a list of known bugs.


**Known Bugs:**


Input: *echo A && <ENTER> "A"*
Experimental Rshell Output: **No such file or directory**
Actual output: -bash: A: command not found


10:33 PM 2/11/2016
Input: *ls && echo A*
Experimental Rshell Output: **ls: cannot access : No such file or directory
Actual output: [files]**
				A
		12:45 AM 2/12/2016
			"ls " a call to ls with any whitespace afterwards will lead to unexpected output


10:54 PM 2/11/2016
Input: *echo A && #hi*
Experimental Rshell Output: **crash**
Actual output: prompts user for more input " > "

 
