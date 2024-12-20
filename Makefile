target =  main
sources = main.cpp calc.cpp interface.cpp
CC = g++
CCFlags = -Wall
LDLIBS = -lcrypto++
OPT = -Ofast
all:build
build:
	$(CC) $(CCFlags) $(OPT) $(sources) -o $(target) $(LDLIBS)
dbg:
	$(CC) -g $(sources) -o $(target)DBG
clean:
	rm -f $(target) $(DEBUG_EXEC)
