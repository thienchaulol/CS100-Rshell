#!/bin/bash

../bin/./rshell << EOF
echo A && echo dog
ls && echo A
echo A && ls
exit
EOF
printf "\n"
