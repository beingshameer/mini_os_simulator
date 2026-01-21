# Mini OS Simulator (CCP - OS Lab)

## Features
- Producer–Consumer bounded buffer using semaphores + mutex (no busy waiting)
- Scheduling:
  - If ready queue size ≤ 5 -> Priority Scheduling
  - If ready queue size > 5 -> Round Robin (quantum = 4)
  - Gantt chart + Waiting Time + Turnaround Time + averages
- Banker's Algorithm for deadlock prevention:
  - Safe sequence check before granting resources
  - Unsafe processes go to blocked queue
  - After resources release, blocked processes are retried
- Menu:
  - Start Simulation
  - Add Process
  - Display State
  - Exit

## Build & Run (Ubuntu)
```bash
make clean
make
./sim
