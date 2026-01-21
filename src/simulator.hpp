#pragma once
#include <vector>
#include <thread>
#include <atomic>
#include <mutex>

#include "ready_buffer.hpp"
#include "bankers.hpp"
#include "scheduler.hpp"

class Simulator {
public:
    Simulator(int buffer_size,
              int producers_count,
              int processes_per_producer,
              std::vector<int> initial_available);

    void start();   // runs full simulation once

private:
    ReadyBuffer buffer_;
    Bankers banker_;

    int producers_count_;
    int processes_per_producer_;

    std::atomic<int> next_pid_{1};

    std::vector<Process> ready_list_;
    std::vector<Process> blocked_list_;

    std::mutex lists_mtx_;

    void producer_thread(int id);
    void consumer_dispatcher();

    void try_unblock(); // retry blocked processes when resources released
    void print_blocked() const;
};
