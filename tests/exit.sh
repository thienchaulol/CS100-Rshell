#!/bin/bash

../bin/./rshell << EOF
echo A && exit
EOF
printf "\n"
