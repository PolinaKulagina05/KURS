target = main
sources = main.cpp calc.cpp interface.cpp
CC = g++
CCFlags = -Wall
LDLIBS = -lcrypto++
OPT = -Ofast
all: build

build:
	$(CC) $(CCFlags) $(OPT) $(sources) -o $(target) $(LDLIBS)

dbg:
	$(CC) -g $(sources) -o $(target)DBG

test:
	$(CC) -o test test.cpp interface.cpp calc.cpp -lUnitTest++ -lcryptopp

clean:
	rm -f $(target) $(target)DBG test
