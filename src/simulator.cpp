#include "simulator.hpp"
#include <iostream>
#include <chrono>

static Process make_sentinel() {
    return Process{-1, 0, 0, 0, {}};
}

Simulator::Simulator(int buffer_size,
                     int producers_count,
                     int processes_per_producer,
                     std::vector<int> initial_available)
    : buffer_(buffer_size),
      banker_(std::move(initial_available)),
      producers_count_(producers_count),
      processes_per_producer_(processes_per_producer) {}

void Simulator::producer_thread(int id) {
    for (int i = 0; i < processes_per_producer_; i++) {
        int pid = next_pid_.fetch_add(1);

        // Example process generation (later menu will allow manual input)
        int burst = (pid % 7) + 1;
        int pr = (pid % 5) + 1;

        // same size as resource types
        std::vector<int> need = { pid % 3, (pid + 1) % 3, (pid + 2) % 3 };

        Process p(pid, 0, burst, pr, need);

        std::cout << "[Producer " << id << "] push PID=" << pid
                  << " burst=" << burst << " pr=" << pr << "\n";

        buffer_.push(p);
        std::this_thread::sleep_for(std::chrono::milliseconds(80));
    }
}

void Simulator::try_unblock() {
    // Try to move blocked -> ready if now safe
    std::vector<Process> still_blocked;

    for (auto &p : blocked_list_) {
        auto safe = banker_.request_resources(p.pid, p.max_need);
        if (safe) {
            std::cout << "[Unblock] PID=" << p.pid << " now SAFE. SafeSeq: ";
            for (int x : *safe) std::cout << x << " ";
            std::cout << "\n";
            ready_list_.push_back(p);
        } else {
            still_blocked.push_back(p);
        }
    }

    blocked_list_ = std::move(still_blocked);
}

void Simulator::consumer_dispatcher() {
    while (true) {
        Process p = buffer_.pop();
        if (p.pid == -1) {
            std::cout << "[Consumer] got sentinel, stopping intake.\n";
            break;
        }

        auto safe = banker_.request_resources(p.pid, p.max_need);
        std::lock_guard<std::mutex> lock(lists_mtx_);

        if (safe) {
            std::cout << "[Consumer] PID=" << p.pid << " SAFE -> ready. SafeSeq: ";
            for (int x : *safe) std::cout << x << " ";
            std::cout << "\n";
            ready_list_.push_back(p);
        } else {
            std::cout << "[Consumer] PID=" << p.pid << " UNSAFE -> blocked.\n";
            blocked_list_.push_back(p);
        }
    }
}

void Simulator::print_blocked() const {
    std::cout << "\nBlocked processes: ";
    if (blocked_list_.empty()) {
        std::cout << "(none)\n";
        return;
    }
    for (auto &p : blocked_list_) std::cout << "P" << p.pid << " ";
    std::cout << "\n";
}

void Simulator::start() {
    std::cout << "\n=== Simulation Start ===\n";
    std::cout << "Initial Available Resources: ";
    for (auto x : banker_.available()) std::cout << x << " ";
    std::cout << "\n";

    // Start consumer
    std::thread consumer(&Simulator::consumer_dispatcher, this);

    // Start producers
    std::vector<std::thread> producers;
    for (int i = 0; i < producers_count_; i++)
        producers.emplace_back(&Simulator::producer_thread, this, i + 1);

    for (auto &t : producers) t.join();

    // stop consumer
    buffer_.push(make_sentinel());
    consumer.join();

    // --- Scheduling phase ---
    {
        std::lock_guard<std::mutex> lock(lists_mtx_);

        std::cout << "\nReady list count = " << ready_list_.size()
                  << " -> " << (ready_list_.size() <= 5 ? "Priority" : "Round Robin")
                  << "\n";

        auto result = Scheduler::run(ready_list_, 4);

        std::cout << "\nGantt Chart:\n";
        for (auto &s : result.gantt) {
            std::cout << "| P" << s.pid << " (" << s.start << "-" << s.end << ") ";
        }
        std::cout << "|\n";

        std::cout << "\nPID  WT  TAT\n";
        for (auto &kv : result.waiting) {
            int pid = kv.first;
            std::cout << pid << "   " << result.waiting.at(pid)
                      << "   " << result.turnaround.at(pid) << "\n";
        }
        std::cout << "\nAverage WT=" << result.avg_waiting
                  << "  Average TAT=" << result.avg_turnaround << "\n";

        // Release resources for all finished processes
        for (auto &p : ready_list_) banker_.release_all(p.pid);

        // after release, try unblock
        try_unblock();
        print_blocked();
    }

    std::cout << "\n=== Simulation End ===\n";
}
