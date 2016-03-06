#!/bin/bash

../bin/./rshell << EOF
(echo A && echo B) || (echo C && echo D)
echo A && echo B || echo C && echo D
(echo A) && (echo B) || echo (D)
(echo D || echo B) && echo A
(echo D || echo B) || (echo A && echo B)
echo A && echoB) || echo C
echo A && )(echo B
echo A & echo B
exit
EOF
printf "\n"
