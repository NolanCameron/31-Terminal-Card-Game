CXX = g++

CXXFLAGS = -std=c++20 -g -Wall -Wvla -Werror -lncursesw

objects = board.o player.o 31Menu.o game.o 31.o 

all: $(objects)
	$(CXX) $(CXXFLAGS) $^ -o 31
	./31

$(objects): %.o: %.cc
	$(CXX) $(CXXFLAGS) -c $^ -o $@

clean:
	rm ./31