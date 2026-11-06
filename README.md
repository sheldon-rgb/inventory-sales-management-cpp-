# Inventory & Sales Management System (C++)

A console application for small-shop stock control and sales tracking.

## Features
- Add or update stock (item, quantity, price)
- View, search and delete inventory items
- Record sales (stock decreases automatically, timestamp saved)
- View total sales for a specific item
- Weekly report: daily sales for the last 7 days plus weekly total
- Data stored in `inventory.txt` and `sales.txt`

## Build & run
`main.cpp` uses `localtime_s`, so it builds with Visual Studio / MSVC on Windows.
For GCC/Linux, replace `localtime_s(&ltm, &now)` with `localtime_r(&now, &ltm)`.

## Tech
C++, file I/O, arrays and strings, `<ctime>`, `<iomanip>`

## Authors
Team project (BS-AI, Bahria University Islamabad): Ayan Aqeel, Muhammad Ahmed, Muhammad Faiq, Muhammad Huzaifa, Abdul Rehman.
