CC = g++

CFLAGS = -g -Wall -Wvla

all:
	g++ 31.cc -lncursesw -o test
	./test

clean:
	rm ./test