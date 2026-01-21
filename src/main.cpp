#include <iostream>
#include <vector>
#include "process.hpp"
#include "scheduler.hpp"

static void print_result(const ScheduleResult& r) {
    std::cout << "\nGantt Chart:\n";
    for (auto& s : r.gantt) {
        std::cout << "| P" << s.pid << " (" << s.start << "-" << s.end << ") ";
    }
    std::cout << "|\n";

    std::cout << "\nPID  WT  TAT\n";
    for (auto& [pid, wt] : r.waiting) {
        std::cout << pid << "    " << wt << "   " << r.turnaround.at(pid) << "\n";
    }

    std::cout << "\nAverage WT=" << r.avg_waiting
              << "  Average TAT=" << r.avg_turnaround << "\n";
}

int main() {
    // Try 5 processes => Priority
    std::vector<Process> procs1 = {
        {1,0,6,2,{1,0,1}},
        {2,0,2,1,{0,1,0}},
        {3,0,8,4,{1,1,0}},
        {4,0,3,3,{0,0,1}},
        {5,0,4,2,{1,0,0}},
    };

    std::cout << "Case 1: size=5 => Priority\n";
    auto r1 = Scheduler::run(procs1, 4);
    print_result(r1);

    // Try 6 processes => RR(q=4)
    std::vector<Process> procs2 = procs1;
    procs2.push_back({6,0,5,5,{0,1,1}});

    std::cout << "\nCase 2: size=6 => Round Robin (q=4)\n";
    auto r2 = Scheduler::run(procs2, 4);
    print_result(r2);

    return 0;
}

