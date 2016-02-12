#!/bin/bash

../bin/./rshell << EOF
echo A; echo B; echo C; echo D; echo E; echo J; echo F; echo M; echo N
echo C && echo D
echo C || echo D
echo D | echo F
ls && echo D
ls || echo D
echo D && ls
exit
EOF
printf "\n" 
