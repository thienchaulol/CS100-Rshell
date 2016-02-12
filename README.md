Thien Chau's and Christopher Pribyl's attempt at rshell.

Known Bugs:

Input: echo A && <ENTER> "A"
Experimental Rshell Output: No such file or directory
Actual output: -bash: A: command not found

10:33 PM 2/11/2016
Input: ls && echo A
Experimental Rshell Output: ls: cannot access : No such file or directory
Actual output: [files]
				A
		12:45 AM 2/12/2016
			"ls " a call to ls with any whitespace afterwards will lead to unexpected output
10:54 PM 2/11/2016
Input: echo A && #hi
Experimental Rshell Output: crash
Actual output: prompts user for more input " > "

 
