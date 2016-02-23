#!/bin/bash

../bin/./rshell << EOF
echo A && exit
EOF

../bin/./rshell << EOF
exit && echo A
EOF
printf "\n"

../bin/./rshell << EOF
echo D || exit
echo "exit" || D
exit
EOF
printf "\n"
