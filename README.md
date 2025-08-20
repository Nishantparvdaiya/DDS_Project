# E-Library Book Management (C++)

Manage a library inventory using a **singly linked list** with **undo** support via a **stack**.

## Features
- Add/Delete books (linked list nodes)
- Borrow/Return (updates copies)
- Search by Title/Author (case-insensitive)
- Undo last action (add/delete/borrow/return)

## Build & Run
```bash
g++ -std=c++17 src/main.cpp -o elib
./elib
```