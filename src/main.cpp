#include <iostream>
#include <thread>
#include <atomic>
#include <vector>
#include <chrono>

#include "process.hpp"
#include "ready_buffer.hpp"

static Process make_sentinel() {
    // pid = -1 means "stop"
    return Process{-1, 0, 0, 0, {}};
}

int main() {
    ReadyBuffer buffer(5); // bounded buffer size = 5 (example)

    const int processes_per_producer = 5;
    const int producers_count = 2;

    std::atomic<int> next_pid{1};

    auto producer = [&](int producer_id) {
        for (int i = 0; i < processes_per_producer; i++) {
            int pid = next_pid.fetch_add(1);

            // Example values (later we will take user input / random generation)
            Process p(pid, /*arrival*/0, /*burst*/(pid % 7) + 1, /*priority*/(pid % 5) + 1,
                      /*resources*/ {pid % 3, (pid+1) % 3, (pid+2) % 3});

            std::cout << "[Producer " << producer_id << "] push PID=" << p.pid
                      << " burst=" << p.burst_time << " pr=" << p.priority << "\n";

            buffer.push(p);

            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    };

    auto consumer = [&]() {
        while (true) {
            Process p = buffer.pop();
            if (p.pid == -1) {
                std::cout << "[Consumer] got sentinel, stopping.\n";
                break;
            }
            std::cout << "  [Consumer] pop PID=" << p.pid
                      << " burst=" << p.burst_time << " pr=" << p.priority << "\n";
        }
    };

    // Start consumer
    std::thread t_consumer(consumer);

    // Start 2 producers
    std::vector<std::thread> producers;
    for (int i = 0; i < producers_count; i++)
        producers.emplace_back(producer, i+1);

    // Wait producers
    for (auto& t : producers) t.join();

    // Send sentinel(s) to stop consumer
    buffer.push(make_sentinel());

    t_consumer.join();

    std::cout << "Checkpoint OK: Producer-Consumer bounded buffer works.\n";
    return 0;
}

