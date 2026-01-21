#include <iostream>
#include "process.hpp"

int main() {
    Process p(1, 0, 5, 2, {1,0,2});
    std::cout << "Mini OS Simulator skeleton. PID=" << p.pid
              << " burst=" << p.burst_time
              << " priority=" << p.priority
              << " resources=" << p.max_need.size()
              << "\n";
    return 0;
}
