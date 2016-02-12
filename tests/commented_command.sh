#!/bin/bash

#"echo A && #hi"
#hi && echo A
#echo A || echo || #hi
../bin/./rshell << EOF
echo A #dogs
echo A && echo B #JFIELSJFLISEJFSILSEF
echo C && echo C #exit
echo P #&& 
thisisan#error
exit
EOF
printf "\n"

#error: "ls && # comment" vector m out of range check
