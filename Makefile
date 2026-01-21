CXX=g++
CXXFLAGS=-std=c++17 -O2 -Wall -Wextra -pthread

all: sim

sim: src/main.cpp
	$(CXX) $(CXXFLAGS) src/main.cpp -o sim

clean:
	rm -f sim

