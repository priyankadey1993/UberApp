# UberApp
A native desktop Uber simulation built completely from scratch using C++, Win32 API, and Windows GDI. Features interactive graph pathfinding (Dijkstra's algorithm), real-time driver allocation, and zero external dependencies.
🚖 Native C++ Uber & Ride-Hailing Desktop Simulation

A lightweight, high-performance desktop ride-booking simulation built completely from scratch using **Native C++ (Win32 API)** and **Windows GDI**. This project avoids heavy external UI frameworks to demonstrate low-level graphics rendering, custom data structures, and graph algorithms.

---

## 🚀 Key Features

* **Zero External Dependencies:** Built entirely using native Windows desktop application templates and GDI headers (`<windows.h>`).
* **Object-Oriented Architecture (`OOP`):** Structured cleanly using modular classes (`City`, `Driver`, and `MapGraph`).
* **Dynamic Shortest-Path Routing:** Implements **Dijkstra's Algorithm** with a priority queue (`std::priority_queue`) to compute optimal routes dynamically.
* **Interactive Ride Booking:** 
  * Click on map nodes to select **Pickup** and **Drop-off** locations via mouse events (`WM_LBUTTONDOWN`).
  * Automatically evaluates available drivers and assigns the **nearest driver** using path distance heuristics.
* **Custom GDI Rendering:** Renders road networks, connected nodes, active path highlights, and driver status markers natively inside a custom window procedure (`WndProc`).

## 🛠️ Tech Stack
* **Language:** C++ (Standard 17/20)
* **API / Framework:** Win32 API, Windows GDI
* **IDE:** Microsoft Visual Studio
  
## 💻 How to Run
1. Clone or download this repository.
2. Open the project solution (`.sln`) in **Visual Studio**.
3. Build the solution (`Ctrl + Shift + B`).
4. Run the application (`Ctrl + F5`) to launch the interactive desktop map.

---
