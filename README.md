# Mini Operating System Simulator (OS Lab CCP)

## Overview

This project is a **Mini Operating System Simulator** implemented in C++.  
It demonstrates how a real operating system manages:

- Process creation
- Synchronization
- CPU scheduling
- Resource management
- Deadlock prevention

The goal of this project is not just to implement algorithms, but to show how
multiple core OS concepts work together in a single integrated system.

---

## Motivation

In real operating systems like Linux or Windows:

- Many programs start at the same time
- Only one process can use the CPU at a time
- Resources like memory, files, and devices are limited
- Deadlocks can occur if processes wait for each other
- Race conditions can corrupt shared data

This simulator models all these problems and their solutions.

---

## System Architecture

The complete system flow is:

Producers  
→ Ready Buffer (bounded buffer)  
→ Consumer (CPU Dispatcher)  
→ Banker's Algorithm  
→ Ready List / Blocked List  
→ Scheduler  
→ Gantt Chart + WT + TAT  

This is very similar to how a real OS kernel works internally.

---

## Modules Explanation

### 1. Process Control Block (process.hpp)

Each process is represented using a structure called **Process**.

It contains:

- pid: Process ID
- arrival_time: When process enters system
- burst_time: CPU time required
- remaining_time: Used for Round Robin
- priority: Used for Priority scheduling
- max_need: Resource requirement vector
- start_time: When CPU starts
- finish_time: When CPU finishes

This structure simulates the **PCB (Process Control Block)** used in real OS.

---

### 2. Ready Buffer (ready_buffer.hpp / cpp)

This module implements the **Producer–Consumer problem**.

- Producers generate processes
- Consumer consumes processes
- Buffer has fixed size (bounded)

Synchronization is achieved using:

- semaphores (empty, full)
- mutex (critical section protection)

This avoids:
- race conditions
- busy waiting
- data corruption

---

### 3. Scheduler (scheduler.hpp / cpp)

This module decides **which process gets CPU**.

Rule implemented:

- If ready_list size ≤ 5 → Priority Scheduling
- If ready_list size > 5 → Round Robin (quantum = 4)

Priority Scheduling:
- Lower number = higher priority
- Non-preemptive

Round Robin:
- Time slicing
- Fair scheduling
- Prevents starvation

The scheduler outputs:

- Gantt chart
- Waiting time
- Turnaround time
- Average WT and TAT

---

### 4. Banker's Algorithm (bankers.hpp / cpp)

This module prevents **deadlock**.

Before granting resources to any process:

1. It checks if request ≤ available
2. Temporarily allocates
3. Runs safety algorithm
4. If safe sequence exists → grant
5. Else → block process

Blocked processes are stored in a **blocked queue**.

After processes finish and release resources,
blocked processes are retried.

This ensures the system **never enters unsafe state**.

---

### 5. Simulator (simulator.hpp / cpp)

This is the **core kernel logic**.

It integrates:

- Producer–Consumer
- Banker
- Scheduler
- Blocked queue
- Resource release
- Unblocking logic

It controls the entire lifecycle of processes.

---

### 6. Menu Interface (main.cpp)

User interacts using menu:

1. Start Simulation
2. Add Process
3. Display State
4. Exit

This simulates:
- process creation
- system monitoring
- CPU execution

---

## Algorithms Used

### Waiting Time:
WT = TAT - Burst Time

### Turnaround Time:
TAT = Finish Time - Arrival Time

### Round Robin:
run_time = min(quantum, remaining_time)

---

## Tools Used

### Compiler: Clang / G++

This project uses modern C++ compilers:

- clang (installed)
- g++ (used in Makefile)

Clang provides:
- better error messages
- faster compilation
- static analysis
- industry-standard tooling

---

### Makefile

Makefile automates:

- compilation
- linking
- rebuilding only changed files

Instead of compiling manually, we just use:

make

This is professional practice in real software projects.

---

### Git & GitHub

Git is used for:

- version control
- tracking progress
- sharing with instructor

Each major feature was committed separately.

---

## How to Build and Run

On Ubuntu:

```bash
make clean
make
./sim
