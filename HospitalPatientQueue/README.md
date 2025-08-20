# Hospital Patient Queue (C++)

Manage patients with a **priority queue** for critical cases and a **regular queue** for non-critical cases. 
The system estimates wait times and serves patients with critical priority first.

## Features
- Add patient: name, severity (1-5), type (Critical/Regular)
- Serve next patient (critical first, then FIFO for regulars)
- View queues
- Estimate wait time per patient

## Build & Run
```bash
g++ -std=c++17 src/main.cpp -o hospital
./hospital
```
