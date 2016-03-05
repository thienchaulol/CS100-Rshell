#!/bin/bash

#test script should be run in 
#/home/csmajs/USERNAME/rshell/rshell/src/ to perform cases
#correctly
../bin/./rshell << EOF
test /home/csmajs/tchau006/rshell/rshell/src/main.cpp
test /home/csmajs/tchau006/rshell/rshell/src/fake.cpp
test /home/csmajs/tchau006/rshell/rshell/src/test/dog.cpp
test /home/csmajs/tchau006/rshell/rshell/src/test/corn.cpp
test /home/csmajs/tchau006/rshell/rshell
test /home/csmajs/tchau006/rshell/cat
test -f /home/csmajs/tchau006/rshell/rshell/src/main.cpp
test -f /home/csmajs/tchau006/rshell/rshell/src/fake.cpp
test -f /home/csmajs/tchau006/rshell/rshell/src/test/dog.cpp
test -f /home/csmajs/tchau006/rshell/rshell/src/test/corn.cpp
test -f /home/csmajs/tchau006/VIMRC_SETTINGS
test -f /home/csmajs/tchau006/rshell/rshell
test -d /home/csmajs/tchau006/rshell/rshell/src/test
test -d /home/csmajs/tchau006/rshell/rshell/src/fake.cpp
test -d /home/csmajs/tchau006/rshell/rshell/src/test/test2
test -d /home/csmajs/tchau006/rshell/rshell/src/test/corn.cpp
test -d /home/csmajs/tchau006/rshell/rshell
test -d /home/csmajs/tchau006/rshell/cat
test -e /home/csmajs/tchau006/rshell/rshell/src/main.cpp
test -e /home/csmajs/tchau006/rshell/rshell/src/fake.cpp
test -e /home/csmajs/tchau006/rshell/rshell/src/test/dog.cpp
test -e /home/csmajs/tchau006/rshell/rshell/src/test/corn.cpp
test -e /home/csmajs/tchau006/rshell/rshell
test -e /home/csmajs/tchau006/rshell/cat
test /home/csmajs/tchau006/rshell/rshell/src/main.cpp && echo "path exists"
test /home/csmajs/tchau006/rshell/rshell/src/fake.cpp && echo "path doesn't exist"
exit
EOF
printf "\n"
