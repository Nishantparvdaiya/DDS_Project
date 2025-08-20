# Personal Finance Tracker (C++)

Track income/expenses, filter/sort transactions, and visualize monthly spending using an ASCII bar chart.

## Features
- Store transactions in an array of structures (date, type, category, note, amount)
- Add/list/search/filter transactions (e.g., expenses over a threshold)
- Sort by amount or date
- Save/Load from file (`finance_data.txt`)
- ASCII bar chart of monthly spending

## Build & Run
```bash
g++ -std=c++17 src/main.cpp -o finance
./finance
```

## Notes
- Date format: `YYYY-MM-DD`
- File format uses `|` as a delimiter to avoid comma issues in text.