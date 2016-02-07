CC = g++
CC_FLAGS = -Wall -ansi
EXEC = a.out
SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:/cpp=.0)

$(EXEC): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXEC)

%.o: %.cpp
	$(CC) -c $(CC_FLAGS) $< -o $@

clean:
	rm -f $(EXEC) $(OBJECTS)
