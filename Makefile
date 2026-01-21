CXX=g++
CXXFLAGS=-std=c++17 -O2 -Wall -Wextra -pthread

SRCS = src/main.cpp src/ready_buffer.cpp src/scheduler.cpp

OBJS = $(SRCS:.cpp=.o)

all: sim

sim: $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o sim

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f sim $(OBJS)

