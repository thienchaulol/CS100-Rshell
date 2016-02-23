#!/bin/bash
echo A && echo dog
echo A && ls
echo A || echo dog
echo A || ls
echo A; echo B; echo C; echo D; echo E
echo A & echo dog
echo A | echo dog
ls; ls -a; echo A; ls; echo C
echo C; ls; ls -a;
ls; ls; ls; ls; ls; ls; echo "A"; echo "BCDCDCDCDCDCDC"
echo "A"; echo "BCDCD" && echo "ECECE"
exit
EOF
printf "\n"


#error "echo C; ls; ls -a; || echo C" should output "bash: syntax error near unexpected token `||'"
#error "echo A |& echo dog". should output only dog. instead outputs A <newline> dog
#error "echo A &| echo dog". should output "bash: syntax error near unexpected token `|'"
