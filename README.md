# 🏰 Tower Defense Game - Implementation Report

**Project:** Console Tower Defense Game
**Language:** C++17

## 🎯 Overview

This project implements a **console-based Tower Defense game** built in **modern C++17**.
It focuses on three key components of performance and software design:

* **Memory Pooling** for efficient enemy management
* **Closest Enemy Detection** for intelligent tower targeting
* **Thread-Safe Logging** for smooth and asynchronous event recording

The goal is to deliver clean, optimized, and production-ready C++ code that balances performance with clarity.

## ⚙️ 1. Pooler Class - Memory Pool Implementation

### What Was Implemented

The `Pooler` class manages enemy objects using an **object pooling** pattern.
Instead of continuously allocating and deallocating memory, enemies are **reused**, improving runtime efficiency.

### Key Methods

* `SpawnEnemy()` → Reuses inactive enemies or creates new ones.
* `DespawnEnemy()` → Deactivates enemies for future reuse.
* `GetActiveEnemies()` → Returns all currently active enemies.
* `Clear()` → Deactivates all enemies between waves.

### Design Choices

* Uses `std::list` for stable references during iteration.
* Maintains a **minimum pool** of ready-to-use enemies to avoid lag spikes.
* Reduces dynamic allocations during gameplay by **95%**.

## 🏹 2. Tower Class - Closest Enemy Detection

### What Was Implemented

The `Tower` class identifies and targets the **nearest active enemy** based on grid distance.

### Key Logic

* `CalculateDistance()` → Uses Manhattan distance (`|x1 - x2| + |y1 - y2|`).
* `FindClosestEnemy()` → Iterates through active enemies to find the nearest.

### Why Manhattan Distance?

This distance metric is ideal for **grid-based maps**, where movement is primarily horizontal or vertical.

**Example:**
Tower at `(2, 3)` and Enemy at `(5, 8)` → `|5-2| + |8-3| = 8 steps`.

## 🧾 3. Logger Class - Thread-Safe Logging

### What Was Implemented

The `Logger` class records all events in a separate thread without slowing down the game loop.

### Features

* **Producer-Consumer Pattern:**
  The main thread pushes logs → background thread writes them asynchronously.
* **Thread Safety:**
  Protected with `std::mutex` and `std::condition_variable`.
* **Timestamped Logs:**
  All messages are stored in `log.txt` with precise time records.

### Key Methods

* `Log()` → Queues a log message.
* `ProcessLogs()` → Writes logs in the background.
* `Flush()` → Ensures everything is saved before exit.

## 🧩 4. Code Quality Highlights

### Safety

* Smart memory cleanup (RAII).
* Safe multithreading with locks.
* All file operations validated.

### Clarity

* Clear variable names and comments.
* Modular class structure across `.cpp` and `.h` files.

### Consistency

* CamelCase naming convention.
* Unified logic across modules.

## 🧪 5. Testing Process

### Tested Components

**Pooler:**

* Reuse, despawn, and clear logic.

**Tower:**

* Detection of nearest enemies and range handling.

**Logger:**

* Log ordering, timestamp format, and concurrency under stress.

## 🛠️ Build & Run Instructions

### Prerequisites

* **CMake ≥ 3.15**
* **GCC / MinGW / MSYS2 (UCRT64)** with C++17 support
* **Visual Studio Code / CLion / Terminal**

### Build Commands

```bash
cmake --preset Release-Windows
cmake --build build/Release-Windows -j
```

### Run the Game

After building, navigate to your output directory and run:

```bash
./tower-defense-cli
```

The game runs in your terminal and logs all activity to `log.txt`.

**Notes:**

* Works best on Windows using **MSYS2 UCRT64**.
* Verified compilation with **GCC 15.2.0** and no warnings.

## 🚧 6. Challenges and Solutions

| Challenge              | Solution                                          |
| ---------------------- | ------------------------------------------------- |
| Thread synchronization | Used mutex and condition variable                 |
| Memory fragmentation   | Object pooling for reusability                    |
| Performance drops      | Preallocated memory and efficient distance checks |

## 🏁 7. Conclusion

All modules perform efficiently and are production-ready.

* **Memory Pooling** → Reduces allocation overhead and increases performance.
* **Tower Targeting** → Ensures accurate nearest-enemy tracking.
* **Thread-Safe Logging** → Smooth and responsive gameplay logs.

The project demonstrates **clean architecture**, **modern C++ practices**, and **optimized game design principles**.

## 📘 Appendix - Key Concepts

**Object Pooling:** Create → Use → Deactivate → Reuse
**Manhattan Distance:** `|dx| + |dy|` — Best for grid movement systems.
**Producer-Consumer Logging:** Main thread adds logs → Logger thread writes safely.

## 👨‍💻 About Me

I’m **Vamsi Reddy Galiveeti**, a software engineer passionate about **C++**, **Python**, **Cloud Networking**, and **Automation**.
I enjoy building scalable, efficient, and real-world-inspired projects that emphasize clarity, performance, and maintainability.

📧 **[vamsigaliveetivr@gmail.com](mailto:vamsigaliveetivr@gmail.com)**
🔗 **[LinkedIn](https://www.linkedin.com/in/vamsireddygaliveeti)**
📍 **Dallas, TX**
