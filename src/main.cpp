#include <iostream>
#include "bankers.hpp"
#include "process.hpp"
#include "simulator.hpp"

int main() {
    // buffer size, producers, processes per producer
    // available resources (3 types for now)
    Simulator sim(/*buffer*/5, /*producers*/2, /*each*/5, /*available*/{3,3,2});
    sim.start();
    return 0;
}
