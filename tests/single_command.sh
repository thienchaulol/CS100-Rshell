#!/bin/bash

../bin/./rshell << EOF
clear
cal
date
df
ls
echo A
false
free
ls -a
hostname
ip
lsof
man ls
pwd
quota
who
exit
EOF
printf "\n"
