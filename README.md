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

10:54 PM 2/11/2016
Input: echo A && #hi
Experimental Rshell Output: crash
Actual output: prompts user for more input " > " 
