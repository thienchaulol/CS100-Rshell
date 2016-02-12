#!/bin/bash

../bin/./rshell << EOF
echo A && #hi
#hi && echo A
echo A || echo B || #hi
exit
EOF
printf "\n"
