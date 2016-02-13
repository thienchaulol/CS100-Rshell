COMPILE = g++
FLAGS = -Wall -Werror -ansi -pedantic
OBJS = src/parse.h

all: rshell

rshell:
	$(COMPILE) $(FLAGS) -o rshell src/main.cpp src/parse.h $(OBJS) &&\
	if [ ! -d bin ]; then mkdir bin; fi && mv rshell bin

clean:
	rm -rf *~ bin
