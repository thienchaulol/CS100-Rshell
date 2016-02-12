#!/bin/bash

#change directory to bin and run rshell executable
#all text between EOF will be executed by rshell
../bin/./rshell << EOF
ls
ls -a
ls -al
date
cal
invalidCommand
git status
echo A
time
alias
uptime
who
exit
EOF
printf "\n"
